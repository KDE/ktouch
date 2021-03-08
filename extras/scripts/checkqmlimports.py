#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
#
#  SPDX-License-Identifier: GPL-2.0-or-later
#

import argparse
import re
from pathlib import Path
from collections import namedtuple

Import = namedtuple('Import', ['name', 'version', 'alias'])

ALLOWED_IMPORTS = {
    'common': [
        Import('QtQuick.Controls', '2.2', None),
        Import('QtQuick.Controls', '2.2', 'Controls'),
        Import('org.kde.kquickcontrolsaddons', '2.0', 'Addons'),
    ],
    '*': [
        Import('QtQuick', '2.9', alias=None),
        Import('QtQuick.Layouts', '1.3', alias=None),
        Import('ktouch', '1.0', alias=None),
        Import('QtGraphicalEffects', '1.0', alias=None),
        Import('org.kde.charts', '0.1', alias='Charts'),
    ],
}

IMPORT_RE = re.compile(r'^import ([\w\.]+) ([\d\.]+)( as ([\w\.]+))?$')
LOCAL_IMPORT = re.compile('^import ("|\')[\w\./]+("|\')')

def dir_arg(path_str):
    path = Path(path_str)
    if not path.exists():
        msg = "'{}' doesn't exist".format(path)
        raise argparse.ArgumentTypeError(msg)
    if not path.is_dir():
        msg = "'{}' has to be a directory".format(path)
        raise argparse.ArgumentTypeError(msg)
    return path

def check_imports(file, module, local_path):
    for line in file:
        line = line.strip()
        if not line.startswith('import'):
            continue
        if LOCAL_IMPORT.match(line) is not None:
            continue
        try:
            check_package_import(line, module, local_path)
        except ValueError:
            print(f"{local_path}: unrecognized import statement: '{line}'")


def check_package_import(line, module, local_path):
    matches = IMPORT_RE.match(line)
    if matches is None:
        raise ValueError('unrecognized import')
    current_import = Import(*matches.group(1, 2, 4))
    for allowed_module, allowed_imports in ALLOWED_IMPORTS.items():
        if allowed_module == '*' or allowed_module == module:
            for allowed_import in allowed_imports:
                if current_import == allowed_import:
                    return
    print(f"{local_path}: import not allowed here: '{line}'")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='check QML imports for KTouch'
    )
    parser.add_argument('path',
        type=dir_arg,
        metavar="PATH",
        default=""
    )
    args = parser.parse_args()
    for qml_file_path in args.path.glob('**/*.qml'):
        local_path = qml_file_path.relative_to(args.path)
        if len(local_path.parts) > 1:
            module = local_path.parts[0]
        else:
            module = None
        check_imports(qml_file_path.open(mode='r'), module, local_path)

