#!/bin/sh

find . -regex '.*\.\(cpp\|hpp\|c\|cxx\|h\|hxx\)' -exec clang-format -style=file -i {} \;
