#!/bin/bash

function usage {
	echo Usage: $0
}

if [ $# -gt 0 ]; then
	usage && exit -1
fi

if [ ! -f copy.c ]; then
	echo "error: no copy.c file"
	exit -1;
fi

for f in fread fwrite fopen fclose;
do
	if grep $f copy.c > /dev/null; then
		echo "error: using $f"
		exit -1
	fi
done

for f in "read" write open close;
do
	if ! grep $f copy.c > /dev/null; then
		echo "error: not using $f"
		exit -1
	fi
done

if ! make > /dev/null; then
	echo "error: compiling errors"
	exit -1;
fi

./copy copy.c copy_copy.c

if ! diff copy.c copy_copy.c; then
	echo "error: copy file differs from original"
	exit -1
fi

head -c 8000 /dev/urandom > data_file.bin
./copy data_file.bin data_file_copy.bin
if ! diff data_file.bin data_file_copy.bin; then
	echo "error: copy large file differs from original"
	exit -1;
fi

echo "Everything seems ok!"
rm data_file.bin data_file_copy.bin
rm copy_copy.c
make clean > /dev/null

exit 0


