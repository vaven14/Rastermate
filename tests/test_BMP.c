#include "Rastermate.h"
#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

static void saveBMP(const char *filename, RM_Buffer *buf) {
    int width  = buf->size.w;
    int height = buf->size.h;
    int rowSize = width * 4;
    uint32_t dataSize = rowSize * height;
    uint32_t fileSize = 14 + 40 + dataSize;

    BITMAPFILEHEADER fileHeader = {
        .bfType = 0x4D42,   // 'BM'
        .bfSize = fileSize,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = 54
    };

    BITMAPINFOHEADER infoHeader = {
        .biSize = 40,
        .biWidth = width,
        .biHeight = height,
        .biPlanes = 1,
        .biBitCount = 32,   // RGBA
        .biCompression = 0, // BI_RGB
        .biSizeImage = dataSize,
        .biXPelsPerMeter = 2835,
        .biYPelsPerMeter = 2835,
        .biClrUsed = 0,
        .biClrImportant = 0
    };

    FILE *f = fopen(filename, "wb");
    if (!f) return;

    fwrite(&fileHeader, sizeof(fileHeader), 1, f);
    fwrite(&infoHeader, sizeof(infoHeader), 1, f);

    // Write pixel data (BMP is bottom-to-top)
    for (int y = height - 1; y >= 0; y--) {
    RM_Color *row = (RM_Color *)(buf->data + (size_t)y * buf->pitch);
    for (int x = 0; x < width; x++) {
        RM_Color c = row[x];
        uint8_t bgra[4] = { c.b, c.g, c.r, c.a };
        fwrite(bgra, 4, 1, f);
    }
}

    fclose(f);
}

int main(void) {
    // 👇 pick your resolution here
    int W = 256;
    int H = 255;

    RM_Buffer buf = rm_create(W, H);

    // Mandelbrot bounds
    double xmin = -2.0, xmax = 1.0;
    double ymin = -1.5, ymax = 1.5;

    int maxIter = 10000;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            // Map pixel (x,y) to complex plane
            double cr = xmin + (x / (double)W) * (xmax - xmin);
            double ci = ymax - (y / (double)H) * (ymax - ymin); // top→bottom

            double zr = 0.0, zi = 0.0;
            int iter = 0;

            while (zr*zr + zi*zi <= 4.0 && iter < maxIter) {
                double zr_new = zr*zr - zi*zi + cr;
                zi = 2.0*zr*zi + ci;
                zr = zr_new;
                iter++;
            }

            // Color mapping
            uint8_t r, g, b;
            if (iter == maxIter) {
                r = g = b = 0; // black (inside set)
            } else {
                double t = (double)iter / maxIter;
                r = (uint8_t)(9 * (1 - t) * t * t * t * 255);
                g = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
                b = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
            }

            RM_Color col = rm_rgba(r, g, b, 255);
            rm_set_pixel(&buf, (RM_Point){ .x = x, .y = y }, col);
        }
        if (y % 128 == 0) {
            printf("Row %d / %d\n", y, H);
        }
    }

    saveBMP("mandelbrot.bmp", &buf);

    rm_destroy(&buf);
    return 0;
}
