#!/usr/bin/env python3
"""
compile-all.py

Recorre recursivamente un árbol de directorios (por defecto, el directorio
del script) y, para cada directorio que contenga un Makefile, ejecuta
`make -C <dir>` o `make clean -C <dir>` si se solicita.

Opciones:
  --root DIR    Directorio raíz desde donde empezar a buscar (por defecto: el
                 directorio donde está este script).
  --dry-run     No ejecuta `make`, solo actúa en modo prueba (retorna 0 sin ejecutar).
  --clean       Ejecuta `make clean` en lugar de `make`.
  --jobs N      Ejecuta N llamadas a make en serie (por compatibilidad futura;
                 no se paraleliza ahora).

El script no imprime nada durante la ejecución; al final imprime exactamente
"fin" y sale con código 0 si todos los `make` terminaron OK, o con el primer
código de error encontrado.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path


def find_makefiles(root: Path):
    """Generador que devuelve rutas de directorios que contienen Makefile."""
    # Ignorar directorios ocultos para evitar entrar en .git, .venv, etc.
    for dirpath, dirnames, filenames in os.walk(str(root), followlinks=False):
        # eliminar directorios ocultos de dirnames para no recorrerlos
        dirnames[:] = [d for d in dirnames if not d.startswith('.')]
        names = {f.lower() for f in filenames}
        if 'makefile' in names or 'gnu makefile' in names or 'gnumakefile' in names:
            yield Path(dirpath)


def run_make_in(dirpath: Path, dry_run: bool = False, target: str = None):
    """Ejecuta `make -C dirpath [target]`. Devuelve el código de salida.

    Si dry_run es True, no ejecuta nada y retorna 0.
    """
    if dry_run:
        return 0

    try:
        cmd = ['make', '-C', str(dirpath)]
        if target:
            cmd.append(target)
        proc = subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return proc.returncode
    except FileNotFoundError:
        # make no existe
        return 127
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

    make_dirs = list(find_makefiles(root))

    exit_codes = []
    target = 'clean' if args.clean else None
    for d in make_dirs:
        code = run_make_in(d, dry_run=args.dry_run, target=target)
        exit_codes.append(code)

    # Imprimir únicamente al final
    print('fin')

    # Salir con el primer código distinto de 0 si existe
    for c in exit_codes:
        if c != 0:
            sys.exit(c)


if __name__ == '__main__':
    main()
