import argparse
from lxml import etree

class Course(object):
    def __init__(self, title, description, keyboard_layout_name, lessons=None):
        self.title = title
        self.description = description
        self.keyboard_layout_name = keyboard_layout_name
        self.lessons = lessons or []
    def xml_tree(self):
        root = etree.Element('course')
        title = etree.SubElement(root, "title")
        title.text = self.title
        description = etree.SubElement(root, "description")
        description.text = self.description
        keyboard_layout_name = etree.SubElement(root, "keyboardLayout")
        keyboard_layout_name.text = self.keyboard_layout_name
        lessons = etree.SubElement(root, "lessons")
        for lesson in self.lessons:
            lesson.build_xml(lessons)
        return root
    def __repr__(self):
        return "<Course {!r}, {!r}, {!r}, {!r}>".format(
            self.title,
            self.description,
            self.keyboard_layout_name,
            self.lessons
        )

class Lesson(object):
    def __init__(self, title, new_characters, text):
        self.title = title
        self.new_characters = new_characters
        self.text = text
    def build_xml(self, parent_node):
        node = etree.SubElement(parent_node, "lesson")
        title = etree.SubElement(node, "title")
        title.text = self.title
        new_characters = etree.SubElement(node, "newCharacters")
        new_characters.text = self.new_characters
        text = etree.SubElement(node, "text")
        text.text = self.text
    def __repr__(self):
        return "<Lesson {!r}, {!r}, {!r}>".format(
            self.title,
            self.new_characters,
            self.text
        )
        
def read(f):
    tree = etree.parse(f)
    root = tree.getroot()
    title, = root.xpath("//KTouchLecture/Title[1]/text()")
    description, = root.xpath("//KTouchLecture/Comment[1]/text()")
    lesson_nodes = root.xpath("//KTouchLecture/Levels/Level")
    lessons = [parse_lesson(node) for node in lesson_nodes]
    return Course(title, description, '', lessons)

def parse_lesson(lesson_node):
    title, = lesson_node.xpath("./NewCharsLabel[1]/text()")
    new_characters, = lesson_node.xpath("./NewCharacters[1]/text()") or ('',)
    text = u"\n".join(lesson_node.xpath("./Line/text()"))
    return Lesson(title, new_characters, text)
    
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
    parser.add_argument('input_file',
        type=argparse.FileType('r'),
        metavar="INPUTFILE",
    )
    args = parser.parse_args()
    course = read(args.input_file)
    print etree.tostring(course.xml_tree())    
    