#include <stdio.h>
#include <stdlib.h>

int isInRange(int coord, int offset, int size) {
    return coord >= offset && coord < offset + size;
}

int isAtBorder(int coord, int offset, int size) {
    return coord == offset || coord == offset + size - 1;
}

void squares(int size1, int x_offset, int y_offset, int size2) {
    int w = size1 > x_offset + size2 ? size1 : x_offset + size2;
    int h = size1 > y_offset + size2 ? size1 : y_offset + size2;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if ((isInRange(x, x_offset, size2) && isAtBorder(y, y_offset, size2)) ||
                (isInRange(y, y_offset, size2) && isAtBorder(x, x_offset, size2))) {
                printf("*");
            } else if ((x < size1 && (y == 0 || y == size1 - 1)) ||
                       (y < size1 && (x == 0 || x == size1 - 1))) {
                printf("#");
            } else {
                printf(" ");
            }
        }

        printf("\n");
    }
}
