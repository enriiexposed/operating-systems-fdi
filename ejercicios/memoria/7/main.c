#include <stdio.h>

int len;

int getNumberIter() {
    return 200;
}

int main() {
    int i;
    int M[128];  // Inicializa todos los elementos a 0
    int x, y;

    x = M[0];  // M[0] es 0 al estar inicializado
    y = 0;
    len = getNumberIter();

    for (i = 1; i < len; i++) {  // Evita desbordamiento de M
        y = x + M[i];
        M[i] = y;
    }

    return y;
}
