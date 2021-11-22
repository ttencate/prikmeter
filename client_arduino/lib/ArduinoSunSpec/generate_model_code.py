#!/usr/bin/env python3

import argparse
import collections
import glob
import json
import logging
import os.path
import re
import textwrap


SCRIPT_DIR = os.path.dirname(__file__)

TYPE_MAP = {
    'int16': 'int16_t',
    'int32': 'int32_t',
    'int64': 'int64_t',
    'raw16': 'uint16_t',
    'uint16': 'uint16_t',
    'uint32': 'uint32_t',
    'uint64': 'uint64_t',
    'acc16': 'uint16_t',
    'acc32': 'uint32_t',
    'acc64': 'uint64_t',
    'bitfield16': 'uint16_t',
    'bitfield32': 'uint32_t',
    'bitfield64': 'uint64_t',
    'enum16': 'uint16_t',
    'enum32': 'uint32_t',
    'float32': 'float',
    'float64': 'double',
    'string': 'String',
    'sunssf': None, # Handled specially.
    'pad': None,
    'ipaddr': 'uint32_t',
    # 'ipv6addr': 'uint128_t',
    'ipv6addr': None,
    'eui48': 'uint64_t',

    # Synthetic types with scale factor. We implement only those that occur in
    # practice.
    'int16_sunssf': 'float',
    'uint16_sunssf': 'float',
    'uint32_sunssf': 'double',
    'uint64_sunssf': 'double',
    'acc32_sunssf': 'double',
}


class DocComment:
    def __init__(self, doc):
        self.doc = doc

    def __str__(self):
        if self.doc:
            return f'''\
/**
 * {self.doc}
 */
'''
        else:
            return ''


class Skip(Exception):
    pass


def label_to_identifier(label, capitalizeFirst):
    words = re.sub(r'[^a-zA-Z0-9_]+', ' ', label).split()
    for i in range(len(words)):
        word = words[i]
        if capitalizeFirst or i > 0:
            word = word[0].upper() + word[1:]
        elif word.isupper():
            pass
        else:
            word = word[0].lower() + word[1:]
        words[i] = word
    ident = ''.join(words)
    if ident in ['int', 'long', 'float']:
        ident += '_'
    return ident


class Class:
    def __init__(self, root):
        group = root['group']
        self.id = root['id']
        # Some models have no descriptive name (just something like
        # `model_17`), but the label is always somewhat brief and descriptive.
        self.name = label_to_identifier(group['label'], True)
        if 'groups' in group:
            logging.warning(f'Repeating groups not yet supported; skipping {self.name}')
            raise Skip()
        self.doc = DocComment(group.get('desc', None))
        points = group['points']
        self.methods = []
        scale_factor_offsets = {}
        offset = 0
        assert(points[0]['name'] == 'ID')
        assert(points[0]['size'] == 1)
        assert(points[1]['name'] == 'L')
        assert(points[1]['size'] == 1)
        for point in points[2:]:
            curr_offset = offset
            offset += point['size']
            try:
                method = Method(point, curr_offset)
            except Skip:
                if point['type'] == 'sunssf':
                    scale_factor_offsets[point['name']] = curr_offset
                continue
            self.methods.append(method)
        self.size = offset

        # Uniquify method names. There are two named `SpecifiedOverExcitedPF`
        # in model 702, for example.
        name_counts = collections.Counter(method.name for method in self.methods)
        for method in self.methods:
            if name_counts[method.name] > 1:
                method.append_offset_to_name()

        for method in self.methods:
            if isinstance(method.scale_factor, str):
                method.parse_args.append(scale_factor_offsets[method.scale_factor])

    def append_id_to_name(self):
        self.name += f'_{self.id}'

    def __str__(self):
        assert(self.name)
        methods = '\n'.join(map(str, self.methods))
        return f'''\
{self.doc}class {self.name} : public SunSpecModel<{self.id}, {self.size}> {{
  public:
{textwrap.indent(methods, ' ' * 4)}
}};
'''


class Method:
    def __init__(self, point, offset):
        self.point_name = point['name']
        self.name = label_to_identifier(point.get('label', self.point_name), False)
        doc = point.get('desc', '')
        units = point.get('units', None)
        if units:
            doc += f' [{units}]'
        self.doc = DocComment(doc.strip())
        self.type = point['type']
        if not self.return_type:
            raise Skip()
        self.offset = offset
        self.parse_args = [offset]
        if self.type == 'string':
            self.parse_args.append(point['size'])
        self.scale_factor = point.get('sf', None)
        if isinstance(self.scale_factor, str):
            self.type += '_sunssf'

    @property
    def return_type(self):
        return TYPE_MAP[self.type]

    def append_offset_to_name(self):
        self.name += f'_{self.offset}'

    def __str__(self):
        assert(self.name)
        assert(self.type)
        return f'''\
{self.doc}inline {self.return_type} {self.name}() const {{ return parse_{self.type}({', '.join(map(str, self.parse_args))}); }}
'''


def natural_sort_key(key):
    def convert(part):
        if part.isdigit():
            return int(part)
        else:
            return part.lower()
    return [convert(part) for part in re.split('([0-9]+)', key)]


def main():
    parser = argparse.ArgumentParser(description='Generates C++ code from SunSpec models')
    parser.add_argument('--models_dir',
                        help='Directory where model .json files can be found',
                        default=os.path.join(SCRIPT_DIR, 'models', 'json'))
    parser.add_argument('--out_file',
                        help='Filename of .h file to be written',
                        default=os.path.join(SCRIPT_DIR, 'src', 'SunSpecModels.h'))
    parser.add_argument('--verbose', '-v',
                        help='Increase verbosity',
                        action='count', default=0)
    args = parser.parse_args()

    logging.basicConfig(level=[
        logging.ERROR,
        logging.WARNING,
        logging.INFO,
        logging.DEBUG,
    ][min(args.verbose, 3)])

    file_names = glob.glob(os.path.join(args.models_dir, 'model_*.json'))
    file_names.sort(key=natural_sort_key)
    classes = []
    for file_name in file_names:
        logging.debug(f'Loading {file_name}')
        with open(file_name, 'rt') as f:
            root = json.load(f)
        try:
            class_ = Class(root)
        except Skip:
            continue
        classes.append(class_)

    # Uniquify class names. There are two named `SolarModule`, for example.
    name_counts = collections.Counter(class_.name for class_ in classes)
    for class_ in classes:
        if name_counts[class_.name] > 1:
            class_.append_id_to_name()

    with open(args.out_file, 'wt') as out:
        classes_str = '\n'.join(map(str, classes))
        out.write(f'''\
#pragma once

#include "SunSpecModel.h"

namespace SunSpecModels {{

{classes_str}
}}
''')


if __name__ == '__main__':
    main()
