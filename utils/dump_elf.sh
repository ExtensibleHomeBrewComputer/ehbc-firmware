#!/bin/sh

if [ ! -f "$1" ]; then
    cat <<EOF
dump_elf: Invalid file name -- $1
usage: $0 elf_file

Dumps informations of the firmware elf binary specified.
EOF
else
    m68k-unknown-elf-objdump -s -S -x -r "$1"
fi
