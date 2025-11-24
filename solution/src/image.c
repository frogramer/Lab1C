#include "../include/image.h"
#include <stdlib.h>
#include <limits.h>

struct image image_create(uint64_t width, uint64_t height) {
    struct image img = {0, 0, NULL};

    // Проверка на нулевые размеры
    if (width == 0 || height == 0) {
        return img;
    }


    if (width > SIZE_MAX / height / sizeof(struct pixel)) {
        return img;
    }

    img.width = width;
    img.height = height;
    img.data = malloc(width * height * sizeof(struct pixel));

    return img;
}

void image_destroy(struct image* img) {
    if (img != NULL && img->data != NULL) {
        free(img->data);
        img->data = NULL;
        img->width = 0;
        img->height = 0;
    }
}

struct pixel* get_pixel(const struct image* img, uint64_t x, uint64_t y) {
    if (img == NULL || img->data == NULL || x >= img->width || y >= img->height) {
        return NULL;
    }
    return &img->data[y * img->width + x];
}

void set_pixel(struct image* img, uint64_t x, uint64_t y, struct pixel pixel) {
    struct pixel* target = get_pixel(img, x, y);
    if (target != NULL) {
        *target = pixel;
    }
}