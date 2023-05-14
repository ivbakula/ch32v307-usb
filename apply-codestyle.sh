#!/bin/sh

find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\|h\|hxx\)' -exec clang-format -style=file -i {} \;
