#include "Rastermate.h"
#include <stdio.h>
#include <assert.h>

static void test_create_and_destroy(void) {
    RM_Buffer buf = rm_create(10, 10);
    assert(buf.size.w == 10);
    assert(buf.size.h == 10);
    assert(buf.data != NULL);

    rm_destroy(&buf);
    assert(buf.data == NULL); // if you implement that in destroy
}

static void test_clear(void) {
    RM_Buffer buf = rm_create(3, 3);
    RM_Color red = rm_rgb(255, 0, 0);
    rm_clear(&buf, red);

    for (uint32_t y = 0; y < buf.size.h; y++) {
        for (uint32_t x = 0; x < buf.size.w; x++) {
            uint32_t val = ((uint32_t*)(buf.data + y * buf.pitch))[x];
            assert(val == red.value);
        }
    }

    rm_destroy(&buf);
}

static void test_set_pixel(void) {
    RM_Buffer buf = rm_create(3, 3);
    RM_Color green = rm_rgb(0, 255, 0);

    bool ok = rm_set_pixel(&buf, (RM_Point){1,1}, green);
    assert(ok);

    uint32_t val = ((uint32_t*)(buf.data + 1 * buf.pitch))[1];
    assert(val == green.value);

    // out of bounds
    ok = rm_set_pixel(&buf, (RM_Point){-1,0}, green);
    assert(!ok);

    rm_destroy(&buf);
}

int main(void) {
    test_create_and_destroy();
    test_clear();
    test_set_pixel();
    printf("All tests passed!\n");
    return 0;
}