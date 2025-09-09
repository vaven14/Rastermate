
#ifndef RASTERMATE_H
#define RASTERMATE_H


#include <stdbool.h>
#include <stdint.h>



typedef struct { int32_t x,y; } RM_Point; // position on Canvas
typedef struct { uint32_t w,h; } RM_Size; // Size of buffer

typedef union {
    struct { uint8_t r,g,b,a; };
    uint32_t value;
} RM_Color;

static inline RM_Color rm_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    RM_Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

static inline RM_Color rm_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return rm_rgba(r, g, b, 255);
}

typedef struct {
    RM_Size size;
    size_t pitch;
    uint8_t *data;
} RM_Buffer;


RM_Buffer rm_create(uint32_t w, uint32_t h);
void      rm_destroy(RM_Buffer *buf);

void rm_clear(RM_Buffer *buf, RM_Color col);

bool rm_set_pixel(RM_Buffer *buf, RM_Point p, RM_Color col);

static inline void rm_set_pixel_fast(RM_Buffer *buf, int x, int y, RM_Color col) {
    ((uint32_t *)(buf->data + (size_t)y * buf->pitch))[x] = col.value;
}

#endif