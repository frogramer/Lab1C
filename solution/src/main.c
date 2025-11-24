#include "../include/image.h"
#include "../include/bmp_io.h"
#include "../include/transform.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <transformed-image> <transformation>\n", argv[0]);
        fprintf(stderr, "Transformations: none, cw90, ccw90, fliph, flipv\n");
        return EINVAL;
    }

    const char* source_file = argv[1];
    const char* output_file = argv[2];
    const char* transformation = argv[3];


    struct image (*transform_func)(const struct image*) = NULL;

    if (strcmp(transformation, "none") == 0) {
        transform_func = copy_image;
    } else if (strcmp(transformation, "cw90") == 0) {
        transform_func = rotate_90_cw;
    } else if (strcmp(transformation, "ccw90") == 0) {
        transform_func = rotate_90_ccw;
    } else if (strcmp(transformation, "fliph") == 0) {
        transform_func = flip_horizontal;
    } else if (strcmp(transformation, "flipv") == 0) {
        transform_func = flip_vertical;
    } else {
        fprintf(stderr, "Error: Unknown transformation '%s'\n", transformation);
        fprintf(stderr, "Available transformations: none, cw90, ccw90, fliph, flipv\n");
        return EINVAL;
    }


    struct image source_img = {0};
    enum read_status read_status = from_bmp_file(source_file, &source_img);

    if (read_status != READ_OK) {
        fprintf(stderr, "Error reading BMP file: ");
        switch (read_status) {
            case READ_INVALID_SIGNATURE:
                fprintf(stderr, "Invalid BMP signature\n");
                break;
            case READ_INVALID_BITS:
                fprintf(stderr, "Invalid bit count (only 24-bit BMP supported)\n");
                break;
            case READ_INVALID_HEADER:
                fprintf(stderr, "Invalid BMP header\n");
                break;
            case READ_INVALID_SIZE:
                fprintf(stderr, "Invalid image size\n");
                break;
            case READ_IO_ERROR:
                fprintf(stderr, "I/O error while reading\n");
                break;
            default:
                fprintf(stderr, "Unknown error\n");
        }
        return EIO;
    }


    struct image transformed_img = transform_func(&source_img);
    if (transformed_img.data == NULL) {
        fprintf(stderr, "Error: Transformation failed - memory allocation error\n");
        image_destroy(&source_img);
        return ENOMEM;
    }


    enum write_status write_status = to_bmp_file(output_file, &transformed_img);

    if (write_status != WRITE_OK) {
        fprintf(stderr, "Error writing BMP file: ");
        switch (write_status) {
            case WRITE_IO_ERROR:
                fprintf(stderr, "I/O error while writing\n");
                break;
            case WRITE_INVALID_IMAGE:
                fprintf(stderr, "Invalid image data\n");
                break;
            default:
                fprintf(stderr, "Unknown error\n");
        }
        image_destroy(&source_img);
        image_destroy(&transformed_img);
        return EIO;
    }


    image_destroy(&source_img);
    image_destroy(&transformed_img);

    printf("Successfully transformed image: %s -> %s (%s)\n",
           source_file, output_file, transformation);
    return 0;
}