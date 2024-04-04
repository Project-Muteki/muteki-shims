#!/usr/bin/env python3
# Copyright 2024 dogtopus
# SPDX-License-Identifier: MIT

import argparse
import pathlib


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument('output', help='Output PRF file.')
    p.add_argument('-I', '--include-path', nargs='*', help='Extra include path.')
    p.add_argument('--no-default-include', help='Exclude muteki-shims path from PRF.')
    return p, p.parse_args()


def main():
    p, args = parse_args()

    include_files = []
    scan_path = pathlib.Path(__file__).absolute().parent / '../include'
    for root, dirs, files in scan_path.walk():
        root_relative = root.relative_to(scan_path)
        for file in files:
            include_files.append(root_relative / file)

    # Emit PRF
    with open(args.output, 'w') as prf:
        # Files
        for file in include_files:
            prf.write(f'{str(file)}\n')
        prf.write('\n')

        # Macros
        prf.write('-D__GHIDRA__\n')
        prf.write('\n')

        # Scan paths
        prf.write(f'{str(scan_path)}\n')
        prf.write('\n')

        # Architecture
        prf.write('ARM:LE:32:v4t\n')
        prf.write('\n')

        # Compiler
        prf.write('default\n')
        prf.write('\n')


if __name__ == '__main__':
    main()
