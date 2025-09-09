#include "Rastermate.h"
#include <stdlib.h>


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



