#
#  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of
#  the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

import argparse, os
from lxml import etree

def find_courses(data_dir, rel_path, root):
    path = os.path.join(data_dir, rel_path)
    course_elems = []
    for filename in os.listdir(path):
        if filename.endswith('.xml'):
            course_tree = etree.parse(file(os.path.join(path, filename), 'r'))
            elem = etree.Element("course")
            title = etree.SubElement(elem, "title")
            title.text, = course_tree.xpath("//course/title/text()")
            description = etree.SubElement(elem, "description")
            description.text, = course_tree.xpath("//course/description/text()")
            keyboard_layout_name = etree.SubElement(elem, "keyboardLayout")
            keyboard_layout_name.text, = course_tree.xpath("//course/keyboardLayout/text()")
            id_ele = etree.SubElement(elem, "id")
            id_ele.text, = course_tree.xpath("//course/id/text()")
            path_ele = etree.SubElement(elem, "path")
            path_ele.text = os.path.join(rel_path, filename)
            course_elems.append(elem)
    course_elems.sort(key=lambda elem: elem.xpath("path/text()"))
    for elem in course_elems:
        root.append(elem)

def find_keyboard_layouts(data_dir, rel_path, root):
    path = os.path.join(data_dir, rel_path)
    keyboard_layout_elems = []
    for filename in os.listdir(path):
        if filename.endswith('.xml'):
            keyboard_tree = etree.parse(file(os.path.join(path, filename), 'r'))
            elem = etree.Element("keyboardLayout")
            title = etree.SubElement(elem, "title")
            title.text, = keyboard_tree.xpath("//keyboardLayout/title/text()")
            name = etree.SubElement(elem, "name")
            name.text, = keyboard_tree.xpath("//keyboardLayout/name/text()")
            id_ele = etree.SubElement(elem, "id")
            id_ele.text, = keyboard_tree.xpath("//keyboardLayout/id/text()")
            path_ele = etree.SubElement(elem, "path")
            path_ele.text = os.path.join(rel_path, filename)
            keyboard_layout_elems.append(elem)
    keyboard_layout_elems.sort(key=lambda elem: elem.xpath("path/text()"))
    for elem in keyboard_layout_elems:
        root.append(elem)
            

if __name__ == '__main__':
    def dir_arg(path):
        if not os.path.exists(path):
            msg = "{!r} doesn't exist".format(path)
            raise argparse.ArgumentTypeError(msg)
        if not os.path.isdir(path):
            msg = "{!r} has to be a directory".format(path)
            raise argparse.ArgumentTypeError(msg)
        return path
    parser = argparse.ArgumentParser(
        description='builds data.xml index file'
    )
    parser.add_argument("datadir",
        type=dir_arg,
        help="the directory containing the data files",
        metavar="DIR"
    )
    args = parser.parse_args()
    root = etree.Element('data')
    find_courses(args.datadir, 'courses', root)
    find_keyboard_layouts(args.datadir, 'keyboardlayouts', root)
    print etree.tostring(root)
