#include "Rastermate.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


RM_Buffer rm_create(uint32_t w, uint32_t h) {
    RM_Buffer out = {0};

    if (w == 0 || h == 0) {
        return out; // empty buffer
    }

    out.size.w = w;
    out.size.h = h;
    out.pitch  = 4 * w;

    out.data = calloc((size_t)w * h, 4);
    if (!out.data) {
        
        out.size.w = out.size.h = 0;
        out.pitch  = 0;
    }

    return out;
}

void rm_destroy(RM_Buffer *buf) {
    if (!buf) return;
    free(buf->data);
    *buf = (RM_Buffer){0}; // reset to safe empty state
}

void rm_clear(RM_Buffer *buf, RM_Color col) {
    if (!buf || !buf->data) return;

    uint32_t pixel = col.value;
    uint32_t *first_row = (uint32_t *)buf->data;

    // fill first row
    for (uint32_t x = 0; x < buf->size.w; x++) {
        first_row[x] = pixel;
    }

    // copy to all other rows
    for (uint32_t y = 1; y < buf->size.h; y++) {
        memcpy(buf->data + y * buf->pitch, buf->data, buf->pitch);
    }
}

bool rm_set_pixel(RM_Buffer *buf, RM_Point p, RM_Color col) {
    if (!buf || !buf->data) return false;

    if (p.x < 0 || p.y < 0 ||
        p.x >= (int32_t)buf->size.w ||
        p.y >= (int32_t)buf->size.h) {
        return false; // outside bounds
    }

    // compute row start
    uint8_t *row = buf->data + (size_t)p.y * buf->pitch;
    uint32_t *dst = (uint32_t *)row;
    dst[p.x] = col.value;

    return true;
}


bool rm_blend_pixel(RM_Buffer *buf, RM_Point p, RM_Color src) {
    if (!buf || !buf->data) return false;

    if (p.x < 0 || p.y < 0 ||
        p.x >= (int32_t)buf->size.w ||
        p.y >= (int32_t)buf->size.h) {
        return false; // outside bounds
    }

    uint8_t *row = buf->data + (size_t)p.y * buf->pitch;
    uint32_t *pixels = (uint32_t *)row;

    RM_Color dst = { .value = pixels[p.x] };
    RM_Color out;

    // Each term fits in 16 bits
    out.r = (uint8_t)(((uint16_t)src.r * src.a + (uint16_t)dst.r * (255 - src.a)) / 255);
    out.g = (uint8_t)(((uint16_t)src.g * src.a + (uint16_t)dst.g * (255 - src.a)) / 255);
    out.b = (uint8_t)(((uint16_t)src.b * src.a + (uint16_t)dst.b * (255 - src.a)) / 255);

    out.a = (uint8_t)(((uint16_t)src.a * 255 + (uint16_t)dst.a * (255 - src.a)) / 255);

    pixels[p.x] = out.value;
    return true;
}

void rm_printf_color(uint8_t col) {
    if (col >= 100) {
        printf("%u ", col);
    } else if (col >= 10) {
        printf(" %u ", col);
    } else {
        printf("  %u ", col);
    }
}

void rm_printf_buffer(RM_Buffer buf) {

    for (int y = 0; y < buf.size.h; y++) {
            for (int x = 0; x < buf.size.w; x++) {
                int index = buf.pitch * y + 4 * x;
                rm_printf_color(buf.data[index]);
                rm_printf_color(buf.data[index + 1]);
                rm_printf_color(buf.data[index + 2]);
                rm_printf_color(buf.data[index + 3]);

                printf("| ");

            }
            printf("\n");
        }

    }

