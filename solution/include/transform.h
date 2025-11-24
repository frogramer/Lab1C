
#ifndef ASSIGNMENT_1_IMAGE_TRANSFORM_MASTER_TRANSFORM_H
#define ASSIGNMENT_1_IMAGE_TRANSFORM_MASTER_TRANSFORM_H

#include "image.h"


struct image rotate_90_cw(const struct image* source);
struct image rotate_90_ccw(const struct image* source);
struct image flip_horizontal(const struct image* source);
struct image flip_vertical(const struct image* source);


struct image copy_image(const struct image* source);

#endif //ASSIGNMENT_1_IMAGE_TRANSFORM_MASTER_TRANSFORM_H