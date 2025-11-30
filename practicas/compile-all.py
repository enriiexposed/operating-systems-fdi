#!/usr/bin/env python3
"""
compile-all.py

Recorre recursivamente un árbol de directorios (por defecto, el directorio
del script) y:
  - Si hay Makefile: ejecuta `make -C <dir>`
  - Si NO hay Makefile pero hay archivos .c: compila con gcc

Opciones:
  --root DIR    Directorio raíz desde donde empezar a buscar (por defecto: el
                 directorio donde está este script).
  --dry-run     No ejecuta comandos, solo actúa en modo prueba.
  --clean       Ejecuta `make clean` (solo para directorios con Makefile).
  --jobs N      Número de trabajos en serie (sin paralelización por ahora).

El script imprime "fin" al terminar y sale con código 0 si todo OK.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path


def find_directories_to_compile(root: Path):
    """Generador que devuelve tuplas (dirpath, has_makefile, c_files)."""
    for dirpath, dirnames, filenames in os.walk(str(root), followlinks=False):
        # Ignorar directorios ocultos
        dirnames[:] = [d for d in dirnames if not d.startswith('.')]
        
        names_lower = {f.lower() for f in filenames}
        has_makefile = 'makefile' in names_lower or 'gnu makefile' in names_lower or 'gnumakefile' in names_lower
        
        c_files = [f for f in filenames if f.endswith('.c')]
        
        if has_makefile or c_files:
            yield Path(dirpath), has_makefile, c_files


def run_make_in(dirpath: Path, dry_run: bool = False, target: str = None):
    """Ejecuta `make -C dirpath [target]`. Devuelve el código de salida."""
    if dry_run:
        return 0

    try:
        cmd = ['make', '-C', str(dirpath)]
        if target:
            cmd.append(target)
        proc = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return proc.returncode
    except FileNotFoundError:
        return 127
    except Exception:
        return 1


def compile_c_files(dirpath: Path, c_files: list, dry_run: bool = False):
    """Compila archivos .c con gcc directamente. Devuelve el código de salida."""
    if dry_run:
        return 0

    try:
        for c_file in c_files:
            output_name = Path(c_file).stem  # nombre sin extensión
            cmd = ['gcc', '-g', '-Wall', '-o', output_name, c_file]
            proc = subprocess.run(cmd, cwd=str(dirpath), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            if proc.returncode != 0:
                return proc.returncode
        return 0
    except FileNotFoundError:
        return 127
    except Exception:
        return 1


def clean_c_files(dirpath: Path, c_files: list, dry_run: bool = False):
    """Elimina ejecutables generados de archivos .c. Devuelve el código de salida."""
    if dry_run:
        return 0

    try:
        for c_file in c_files:
            output_name = Path(c_file).stem
            exe_path = dirpath / output_name
            if exe_path.exists() and exe_path.is_file():
                exe_path.unlink()
        return 0
    except Exception:
        return 1


def parse_args():
    p = argparse.ArgumentParser(description='Recorre directorios y ejecuta Makefiles.')
    p.add_argument('--root', '-r', type=Path,
                   default=Path(__file__).resolve().parent,
                   help='Directorio raíz donde empezar a buscar (por defecto: carpeta del script)')
    p.add_argument('--dry-run', action='store_true', help='No ejecutar make; solo actuar como prueba')
    p.add_argument('--clean', action='store_true', help='Ejecutar `make clean` en lugar de `make`')
    p.add_argument('--jobs', '-j', type=int, default=1, help='Número de trabajos en serie (por ahora no paraleliza)')
    return p.parse_args()


def main():
    args = parse_args()
    root = args.root

    if not root.exists():
        sys.exit(2)

    dirs_to_compile = list(find_directories_to_compile(root))

    exit_codes = []
    target = 'clean' if args.clean else None
    
    for dirpath, has_makefile, c_files in dirs_to_compile:
        if has_makefile:
            code = run_make_in(dirpath, dry_run=args.dry_run, target=target)
            exit_codes.append(code)
        elif c_files:
            if args.clean:
                # Limpiar ejecutables generados de archivos .c
                code = clean_c_files(dirpath, c_files, dry_run=args.dry_run)
                exit_codes.append(code)
            else:
                # Compilar archivos .c
                code = compile_c_files(dirpath, c_files, dry_run=args.dry_run)
                exit_codes.append(code)

    # Imprimir únicamente al final
    print('fin')

    # Salir con el primer código distinto de 0 si existe
    for c in exit_codes:
        if c != 0:
            sys.exit(c)


if __name__ == '__main__':
    main()
