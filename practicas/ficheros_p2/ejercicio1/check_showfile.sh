#!/bin/bash

function usage {
	echo Usage: $0
}

if [ $# -gt 0 ]; then
	usage && exit -1
fi

if [ ! -f show_file.c ]; then
	echo "error: no show_file.c file"
	exit -1;
fi

if ! grep fread show_file.c > /dev/null; then
	echo "error: show_file not using fread"
	exit -1
fi

if ! grep fwrite show_file.c > /dev/null; then
	echo "error: show_file not using fwrite"
	exit -1
fi

if ! make > /dev/null; then
	echo "error: compiling errors"
	exit -1;
fi

./show_file show_file.c > output.txt

if ! diff output.txt show_file.c; then
	echo "error: output differs from input"
	exit -1
fi

echo "Everything seems ok!"
exit 0

