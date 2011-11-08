import argparse
from lxml import etree

class Keyboard(object):
    def __init__(self, title, font_suggestion, language, keys=[]):
        self.title = title
        self.font_suggestion = font_suggestion
        self.language = language
        self.keys = keys
    def xml_tree(self):
        root = etree.Element('keyboard')
        title = etree.SubElement(root, "title")
        title.text = self.title
        language = etree.SubElement(root, "language")
        language.text = self.language
        font_suggestion = etree.SubElement(root, "fontSuggestion")
        font_suggestion.text = self.font_suggestion
        keys = etree.SubElement(root, "keys")
        for key in self.keys:
            if isinstance(key, Key):
                key.build_xml(keys)
        for key in self.keys:
            if isinstance(key, SpecialKey):
                key.build_xml(keys)
        return root
        
    def __repr__(self):
        return "<Keyboard {!r}, {!r}, {!r}, {!r}>".format(
            self.title,
            self.font_suggestion,
            self.language,
            self.keys
        )
        

class AbstractKey(object):
    def __init__(self, left, top, width, height):
        self.left = left
        self.top = top
        self.width = width
        self.height = height
        
class Key(AbstractKey):
    def __init__(self, left, top, width, height, finger_index, chars=[]):
        super(Key, self).__init__(left, top, width, height)
        self.finger_index = finger_index
        self.chars = chars
        self.has_haptic_marker = False
    def build_xml(self, parent_node):
        node = etree.SubElement(parent_node, "key")
        node.attrib['left'] = str(self.left)
        node.attrib['top'] = str(self.top)
        node.attrib['width'] = str(self.width)
        node.attrib['height'] = str(self.height)
        node.attrib['fingerIndex'] = str(self.finger_index)
        if self.has_haptic_marker:
            node.attrib['hasHapticMarker'] = "true"
        for char in self.chars:
            char.build_xml(node)
    def __repr__(self):
        return "<Key {!r}, {!r}, {!r}, {!r}, {!r}, {!r}, {!r}>".format(
            self.left,
            self.top,
            self.width,
            self.height,
            self.finger_index,
            self.chars,
            self.has_haptic_marker
        )

class SpecialKey(AbstractKey):
    def __init__(self, left, top, width, height, type):
        super(SpecialKey, self).__init__(left, top, width, height)
        self.type = type
        self.modifier_id = None
        self.label = None
    def build_xml(self, parent_node):
        node = etree.SubElement(parent_node, "specialKey")
        node.attrib['left'] = str(self.left)
        node.attrib['top'] = str(self.top)
        node.attrib['width'] = str(self.width)
        node.attrib['height'] = str(self.height)
        node.attrib['type'] = str(self.type)
        if self.modifier_id is not None:
            node.attrib['modifierId'] = self.modifier_id
        if self.label is not None:
            node.attrib['label'] = self.label
    def __repr__(self):
        return "<SpecialKey {!r}, {!r}, {!r}, {!r}, {!r}, {!r}, {!r}>".format(
            self.left,
            self.top,
            self.width,
            self.height,
            self.type,
            self.modifier_id,
            self.label
        )

class Char(object):
    def __init__(self, value, position, modifier=None):
        self.value = value
        self.position = position
        self.modifier = modifier
    def build_xml(self, parent_node):
        node = etree.SubElement(parent_node, "char")
        node.text = self.value
        node.attrib['position'] = self.position
        if self.modifier is not None:
            node.attrib['modifier'] = str(self.modifier)
    def __repr__(self):
        return u"<Char {!r}, {!r}, {!r}>".format(
            self.value,
            self.position,
            self.modifier
        )

POSITION_MAP = {
    "TopLeft": "topLeft",
    "TopRight": "topRight",
    "BottomLeft": "bottomLeft",
    "BottomRight": "bottomRight"
}

MODIFIER_MAP = {
    "topLeft": "shift",
    "topRight": None,
    "bottomLeft": None,
    "bottomRight": "altgr"
}

def read(f):
    tree = etree.parse(f)
    root = tree.getroot()
    title, = etree.XPath("//KTouchKeyboard/Title[1]/text()")(root)
    font_suggestion, = etree.XPath("//KTouchKeyboard/FontSuggestions[1]/text()")(root)
    language, = etree.XPath("//KTouchKeyboard/Language[1]/text()")(root)
    key_nodes = etree.XPath("//KTouchKeyboard/Keys/Key")(root)
    keys = [parse_key(node) for node in key_nodes]
    return Keyboard(title, font_suggestion, language, keys)

    
def parse_key(key_node):
    get_char_nodes = etree.XPath("./Char")
    type = key_node.attrib['Type']
    left = key_node.attrib['X']
    top = key_node.attrib['Y']
    width = key_node.attrib['Width']
    height = key_node.attrib['Height']
    if type in ['NORMAL', 'FINGER']:
        has_haptic_marker = False
        if type == 'FINGER':
            finger_index = key_node.getparent().index(key_node)
            if finger_index in [3, 4]:
                has_haptic_marker = True
        else:
            finger_index = key_node.attrib['FingerKeyIndex']
        
        chars = parse_chars(get_char_nodes(key_node))
        key = Key(left, top, width, height, finger_index, chars)
        key.has_haptic_marker = has_haptic_marker
    elif type == "TAB":
        key = SpecialKey(left, top, width, height, 'tab')
    elif type == "CAPSLOCK":
        key = SpecialKey(left, top, width, height, 'capslock')
    elif type == "SHIFT":
        key = SpecialKey(left, top, width, height, 'shift')
        key.modifier_id = 'shift'
    elif type == "BACKSPACE":
        key = SpecialKey(left, top, width, height, 'backspace')
    elif type == "ENTER":
        key = SpecialKey(left, top, width, height, 'enter')
    elif type == "SPACE":
        key = SpecialKey(left, top, width, height, 'space')
    else:
        key = SpecialKey(left, top, width, height, 'other')
        key.label = key_node.attrib['KeyText']
        if key.label == "Alt Gr":
            key.modifier_id = 'altgr'
    return key

def parse_chars(char_nodes):
    chars = []
    for char_node in char_nodes:
        value = char_node.text
        position = POSITION_MAP[char_node.attrib['Position']]
        modifier = MODIFIER_MAP[position]
        char = Char(value, position, modifier)
        chars.append(char)
        if value.lower() != value:
            hiddenChar = Char(value.lower(), 'hidden')
            char.modifier = 'shift'
            chars.append(hiddenChar)
    return chars
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='convert ktouch keyboard files'
    )
    parser.add_argument('-i', '--in',
        type=argparse.FileType('r'),
        metavar="INPUTFILE",
        dest='input_file',
        required=True
    )
    args = parser.parse_args()
    keyboard = read(args.input_file)
    print etree.tostring(keyboard.xml_tree())    
    