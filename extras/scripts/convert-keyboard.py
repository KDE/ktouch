#
#  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
#
#  SPDX-License-Identifier: GPL-2.0-or-later
#

import argparse
import uuid
from lxml import etree

class KeyboardLayout(object):
    def __init__(self, id, title, name, keys=None):
        self.id = id
        self.title = title
        self.name = name
        self.keys = keys or []
    @property
    def keys(self):
        return self._keys
    @keys.setter
    def keys(self, keys):
        self._keys = keys
        self.width = 0
        self.height = 0
        min_left = None
        min_top = None
        for key in keys:
            if key.left < min_left or min_left is None:
                min_left = key.left
            if key.top < min_top or min_top is None:
                min_top = key.top
        for key in keys:
            key.left = key.left - min_left
            key.top = key.top - min_top
            if key.left + key.width > self.width:
                self.width = key.left + key.width
            if key.top + key.height > self.height:
                self.height = key.top + key.height
    def xml_tree(self):
        root = etree.Element('keyboardLayout')
        id = etree.SubElement(root, "id")
        id.text = self.id
        title = etree.SubElement(root, "title")
        title.text = self.title
        name = etree.SubElement(root, "name")
        name.text = self.name
        width = etree.SubElement(root, "width")
        width.text = str(self.width)
        height = etree.SubElement(root, "height")
        height.text = str(self.height)
        keys = etree.SubElement(root, "keys")
        for key in self.keys:
            if isinstance(key, Key):
                key.build_xml(keys)
        for key in self.keys:
            if isinstance(key, SpecialKey):
                key.build_xml(keys)
        return root
    def __repr__(self):
        return "<KeyboardLayout {!r}, {!r}, {!r}, {!r}, {!r}>".format(
            self.title,
            self.name,
            self.keys,
            self.width,
            self.height
        )

class AbstractKey(object):
    def __init__(self, left, top, width, height):
        self.left = left
        self.top = top
        self.width = width
        self.height = height
        
class Key(AbstractKey):
    def __init__(self, left, top, width, height, finger_index, chars=None):
        super(Key, self).__init__(left, top, width, height)
        self.finger_index = finger_index
        self.chars = chars or []
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
    id = make_id()
    title, = etree.XPath("//KTouchKeyboard/Title[1]/text()")(root)
    name, = etree.XPath("//KTouchKeyboard/Language[1]/text()")(root)
    key_nodes = etree.XPath("//KTouchKeyboard/Keys/Key")(root)
    keys = [parse_key(node) for node in key_nodes]
    return KeyboardLayout(id, title, name, keys)
    
def parse_key(key_node):
    get_char_nodes = etree.XPath("./Char")
    type = key_node.attrib['Type']
    left = int(key_node.attrib['X'])
    top = int(key_node.attrib['Y'])
    width = int(key_node.attrib['Width'])
    height = int(key_node.attrib['Height'])
    if type in ['NORMAL', 'FINGER']:
        has_haptic_marker = False
        if type == 'FINGER':
            finger_index = key_node.getparent().index(key_node)
            if finger_index in [3, 4]:
                has_haptic_marker = True
        else:
            finger_index = key_node.attrib['FingerKeyIndex']
        
        chars = parse_chars(get_char_nodes(key_node), finger_index)
        key = Key(left, top, width, height, finger_index, chars)
        key.has_haptic_marker = has_haptic_marker
    elif type == "TAB":
        key = SpecialKey(left, top, width, height, 'tab')
    elif type == "CAPSLOCK":
        key = SpecialKey(left, top, width, height, 'capslock')
    elif type == "SHIFT":
        key = SpecialKey(left, top, width, height, 'shift')
        key.modifier_id = left < 700 and 'left_shift' or 'right_shift'
    elif type == "BACKSPACE":
        key = SpecialKey(left, top, width, height, 'backspace')
    elif type == "ENTER":
        key = SpecialKey(left, top, width, height, 'return')
    elif type == "SPACE":
        key = SpecialKey(left, top, width, height, 'space')
    else:
        key = SpecialKey(left, top, width, height, 'other')
        key.label = key_node.attrib['KeyText']
        if key.label == "AltGr":
            key.label = "Alt Gr"
        if key.label == "Alt Gr":
            key.modifier_id = 'altgr'
    return key

def parse_chars(char_nodes, finger_index):
    chars = []
    for char_node in char_nodes:
        value = char_node.text
        position = POSITION_MAP[char_node.attrib['Position']]
        modifier = MODIFIER_MAP[position]
        if modifier == 'shift':
            modifier = finger_index <= 4 and "right_shift" or "left_shift"
        char = Char(value, position, modifier)
        chars.append(char)
        if value.lower() != value:
            hiddenChar = Char(value.lower(), 'hidden')
            char.modifier = finger_index <= 4 and "right_shift" or "left_shift"
            chars.append(hiddenChar)
    return chars

def make_id():
    return "{{{}}}".format(uuid.uuid4())

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='convert ktouch keyboard files'
    )
    parser.add_argument('input_file',
        type=argparse.FileType('r'),
        metavar="INPUTFILE"
    )
    args = parser.parse_args()
    layout = read(args.input_file)
    print etree.tostring(layout.xml_tree())    
