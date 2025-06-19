#!/bin/bash
if [ -d $1 ]; then
    rm -r $1
fi
mkdir $1
cd $1
mkdir subdir
touch fichero1
echo "Hola mundo!" > fichero2
ln -s fichero2 enlaceS
ln fichero2 enlaceH
for file in *; do
    stat "$file"
done
