#!/usr/bin/env bash

/usr/bin/env llc 2>/dev/null "$1" -o "intermediate.s"
code=$?
if [ $code != 0 ]; then
	/usr/local/bin/llc "$1" -o "intermediate.s"
fi
/usr/bin/env clang intermediate.s lib/runtime.bc -o "$2"
rm intermediate.s
