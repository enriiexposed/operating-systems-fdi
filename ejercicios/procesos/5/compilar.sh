#!/bin/bash

for i in {1..8}; do
    archivo="p$i.c"
    salida="p$i"

    if [ -f "$archivo" ]; then
        gcc "$archivo" -o "$salida" && echo "Compilado: $salida"
    else
        echo "No existe: $archivo"
    fi
done