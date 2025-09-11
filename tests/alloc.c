#include "Rastermate.h"
#include <stdio.h>
#include <stdint.h>

void print_col(uint8_t col) {
    if (col >= 100) {
        printf("%u ", col);
    } else if (col >= 10) {
        printf(" %u ", col);
    } else {
        printf("  %u ", col);
    }
}

int main(void) {
    RM_Buffer buf = rm_create(5 ,3);
    RM_Color white =rm_rgb(255, 255, 255);

    rm_set_pixel(&buf, (RM_Point){2,1}, white);
    for (int y = 0; y < buf.size.h; y++) {
        for (int x = 0; x < buf.size.w; x++) {
            int index = buf.pitch * y + 4 * x;
            print_col(buf.data[index]);
            print_col(buf.data[index + 1]);
            print_col(buf.data[index + 2]);
            print_col(buf.data[index + 3]);

            printf("| ");

        }
        printf("\n");
    }


    rm_destroy(&buf);
    return 0;
}