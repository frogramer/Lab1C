#include "../include/transform.h"
#include "../include/image.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct image copy_image(const struct image* source) {
    if (source == NULL || source->data == NULL) {
        struct image empty = {0, 0, NULL};
        return empty;
    }


    if (source->width > SIZE_MAX / source->height / sizeof(struct pixel)) {
        struct image empty = {0, 0, NULL};
        return empty;
    }

    struct image copy = image_create(source->width, source->height);
    if (copy.data != NULL) {
        memcpy(copy.data, source->data, source->width * source->height * sizeof(struct pixel));
    }
    return copy;
}

struct image flip_horizontal(const struct image* source) {
    if (source == NULL || source->data == NULL) {
        struct image empty = {0, 0, NULL};
        return empty;
    }

    struct image flipped = image_create(source->width, source->height);
    if (flipped.data == NULL) {
        return flipped;
    }

    for (uint64_t y = 0; y < source->height; y++) {
        for (uint64_t x = 0; x < source->width; x++) {
            struct pixel* src_pixel = get_pixel(source, x, y);
            struct pixel* dst_pixel = get_pixel(&flipped, source->width - x - 1, y);
            if (src_pixel != NULL && dst_pixel != NULL) {
                *dst_pixel = *src_pixel;
            }
        }
    }

    return flipped;
}

struct image flip_vertical(const struct image* source) {
    if (source == NULL || source->data == NULL) {
        struct image empty = {0, 0, NULL};
        return empty;
    }

    struct image flipped = image_create(source->width, source->height);
    if (flipped.data == NULL) {
        return flipped;
    }

    for (uint64_t y = 0; y < source->height; y++) {
        for (uint64_t x = 0; x < source->width; x++) {
            struct pixel* src_pixel = get_pixel(source, x, y);
            struct pixel* dst_pixel = get_pixel(&flipped, x, source->height - y - 1);
            if (src_pixel != NULL && dst_pixel != NULL) {
                *dst_pixel = *src_pixel;
            }
        }
    }

    return flipped;
}

struct image rotate_90_cw(const struct image* source) {
    if (source == NULL || source->data == NULL) {
        struct image empty = {0, 0, NULL};
        return empty;
    }


    struct image rotated = image_create(source->height, source->width);
    if (rotated.data == NULL) {
        return rotated;
    }

    for (uint64_t y = 0; y < source->height; y++) {
        for (uint64_t x = 0; x < source->width; x++) {
            struct pixel* src_pixel = get_pixel(source, x, y);
            struct pixel* dst_pixel = get_pixel(&rotated, source->height - y - 1, x);
            if (src_pixel != NULL && dst_pixel != NULL) {
                *dst_pixel = *src_pixel;
            }
        }
    }

    return rotated;
}

struct image rotate_90_ccw(const struct image* source) {
    if (source == NULL || source->data == NULL) {
        struct image empty = {0, 0, NULL};
        return empty;
    }


    struct image rotated = image_create(source->height, source->width);
    if (rotated.data == NULL) {
        return rotated;
    }

    for (uint64_t y = 0; y < source->height; y++) {
        for (uint64_t x = 0; x < source->width; x++) {
            struct pixel* src_pixel = get_pixel(source, x, y);
            struct pixel* dst_pixel = get_pixel(&rotated, y, source->width - x - 1);
            if (src_pixel != NULL && dst_pixel != NULL) {
                *dst_pixel = *src_pixel;
            }
        }
    }

    return rotated;
}