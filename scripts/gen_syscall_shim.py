#!/usr/bin/env python3
# Copyright 2021-present dogtopus
# SPDX-License-Identifier: MIT

import argparse
import json
import os
import sys

HEADER_GAS = r'''
    .cpu arm7tdmi
    .section .text
    .macro define_syscall num, name
    .arm
\name:
    push {r0}
    push {lr}
    svc \num

    .global \name
    .endm
'''.strip('\n')

HEADER_GAS_EABI = r'''
    .cpu arm7tdmi
    .section .text
    .macro define_syscall num, name
    .arm
    .type	\name, %function
\name:
    push {r0}
    push {lr}
    svc \num

    .global \name
    .endm
'''.strip('\n')

CRT_STUB_GAS = r'''
    .global DllMainCRTStartup
DllMainCRTStartup:
    bx lr
'''.strip('\n')

HEADERS = {
    'gas': (HEADER_GAS, CRT_STUB_GAS),
    'gas-eabi': (HEADER_GAS_EABI, CRT_STUB_GAS),
}

def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument('-t', '--assembler-type', choices=HEADERS.keys(), default='gas', help='Assembler type.')
    p.add_argument('-s', '--standalone', action='store_true', default=False, help='Generate standalone object (i.e. with empty DllMainCRTStartup included).')
    p.add_argument('-S', '--split', action='store_true', default=False, help='Split each shim into its own file. (Ignores -s)')
    p.add_argument('-d', '--scandeps', action='store_true', default=False, help='Scan dependencies for meson. (Only makes sense when running with -S)')
    p.add_argument('-p', '--scandeps-prefix', help='Prefix added to results of -d.')
    p.add_argument('-o', '--output', help='Path to output.')
    p.add_argument('syscall_mapping', nargs='+', help='Path to JSON-formatted syscall mapping file(s).')
    return p, p.parse_args()

def generate_syscall_shim(mappings, output, assembler_type, type_='normal', scandeps=False, scandeps_prefix=None):
    mapping = []
    known_functions = set()
    sub_mappings = []
    sub_mapping_entries = 0
    for mapping_path in mappings:
        with open(mapping_path, 'r') as fmapping:
            sub_mapping = json.load(fmapping)
            sub_mappings.append(sub_mapping)
            sub_mapping_entries += len(sub_mapping)
            for num, name in sub_mapping.items():
                if name in known_functions:
                    raise RuntimeError(f'Function {name} was redefined.')
                known_functions.add(name)
            mapping.extend(sub_mapping.items())

    if type_ in ('normal', 'standalone'):
        with open(output, 'w') as fout:
            fout.write(HEADERS[assembler_type][0])
            fout.write('\n\n')
            if type_ == 'standalone':
                fout.write(HEADERS[assembler_type][1])
                fout.write('\n\n')
            for num, name in mapping:
                fout.write(f'define_syscall {num} {name}\n')
    elif type_ == 'split':
        if output is not None:
            os.makedirs(output, exist_ok=True)
        for num, name in mapping:
            subfile_name = os.path.join(scandeps_prefix, f'{name}.s') if scandeps_prefix is not None else f'{name}.s'
            if scandeps:
                print(subfile_name)
            if output is not None:
                subfile_path = os.path.join(output, f'{name}.s')
                with open(subfile_path, 'w') as fout:
                    fout.write(HEADERS[assembler_type][0])
                    fout.write('\n\n')
                    fout.write(f'define_syscall {num} {name}\n')
    else:
        assert False, f'Unknown type {type_}'

if __name__ == '__main__':
    p, args = parse_args()
    if not args.scandeps and args.output is None:
        p.error('--output is required in this configuration.')
    generate_syscall_shim(args.syscall_mapping, args.output, args.assembler_type, 'split' if args.split else ('standalone' if args.standalone else 'normal'), args.scandeps, args.scandeps_prefix)
