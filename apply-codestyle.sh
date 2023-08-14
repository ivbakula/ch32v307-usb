#!/bin/sh

find . -path ./unit-test/mocks -prune -o -regex '.*\.\(cpp\|hpp\|c\|cxx\|h\|hxx\)' -exec clang-format -style=file -i {} \;
