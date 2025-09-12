#include "Rastermate.h"
#include <stdio.h>
#include <stdint.h> 
int main(void) { 

    RM_Buffer buf = rm_create(5, 5);
    RM_Color white =rm_rgb(255, 255, 255);
    RM_Color red = rm_rgb(255, 0, 0);
    
    rm_printf_buffer(buf);
    printf("\n");
    rm_clear(&buf, red);
    rm_printf_buffer(buf);
    rm_destroy(&buf);
    return 0;
}