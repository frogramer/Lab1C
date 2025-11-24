#ifndef ASSIGNMENT_1_IMAGE_TRANSFORM_MASTER_IMAGE_H
#define ASSIGNMENT_1_IMAGE_TRANSFORM_MASTER_IMAGE_H

#include <stdint.h>
#include <stddef.h>

#pragma pack(push, 1)
struct pixel {
    uint8_t b, g, r;
};
#pragma pack(pop)

struct image {
    uint64_t width, height;
    struct pixel* data;
};


struct image image_create(uint64_t width, uint64_t height);
void image_destroy(struct image* img);


struct pixel* get_pixel(const struct image* img, uint64_t x, uint64_t y);
void set_pixel(struct image* img, uint64_t x, uint64_t y, struct pixel pixel);

#endif