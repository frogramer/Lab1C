#include "../include/bmp_io.h"
#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


static uint32_t get_padding(uint32_t width) {

    if (width > UINT32_MAX / sizeof(struct pixel)) {
        return 0;
    }
    return (4 - (width * sizeof(struct pixel)) % 4) % 4;
}


static enum read_status validate_bmp_header(const struct bmp_header* header) {
    if (header->bfType != 0x4D42) { // 'BM' в little-endian
        return READ_INVALID_SIGNATURE;
    }
    if (header->biBitCount != 24) {
        return READ_INVALID_BITS;
    }
    if (header->biCompression != 0) {
        return READ_INVALID_HEADER;
    }
    if (header->biWidth == 0 || header->biHeight == 0) {
        return READ_INVALID_SIZE;
    }

    if (header->biWidth > 100000 || header->biHeight > 100000) {
        return READ_INVALID_SIZE;
    }
    return READ_OK;
}

enum read_status from_bmp(FILE* in, struct image* img) {
    if (in == NULL || img == NULL) {
        return READ_INVALID_HEADER;
    }


    struct bmp_header header;
    if (fread(&header, sizeof(header), 1, in) != 1) {
        return READ_IO_ERROR;
    }


    enum read_status validation_status = validate_bmp_header(&header);
    if (validation_status != READ_OK) {
        return validation_status;
    }


    *img = image_create(header.biWidth, header.biHeight);
    if (img->data == NULL) {
        return READ_INVALID_SIZE;
    }


    if (fseek(in, header.bOffBits, SEEK_SET) != 0) {
        image_destroy(img);
        return READ_IO_ERROR;
    }


    uint32_t padding = get_padding(header.biWidth);
    for (uint32_t y = 0; y < header.biHeight; y++) {

        for (uint32_t x = 0; x < header.biWidth; x++) {
            struct pixel p;
            if (fread(&p, sizeof(struct pixel), 1, in) != 1) {
                image_destroy(img);
                return READ_IO_ERROR;
            }
            set_pixel(img, x, header.biHeight - y - 1, p); // BMP хранится снизу вверх
        }

        if (padding > 0 && fseek(in, padding, SEEK_CUR) != 0) {
            image_destroy(img);
            return READ_IO_ERROR;
        }
    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img) {
    if (out == NULL || img == NULL || img->data == NULL) {
        return WRITE_INVALID_IMAGE;
    }


    if (img->width == 0 || img->height == 0) {
        return WRITE_INVALID_IMAGE;
    }


    struct bmp_header header = {0};
    uint32_t padding = get_padding(img->width);


    if (img->width > UINT32_MAX / sizeof(struct pixel) - padding) {
        return WRITE_INVALID_IMAGE;
    }

    uint32_t row_size = img->width * sizeof(struct pixel) + padding;

    if (img->height > UINT32_MAX / row_size) {
        return WRITE_INVALID_IMAGE;
    }

    uint32_t image_size = row_size * img->height;

    header.bfType = 0x4D42; // 'BM'
    header.bfileSize = sizeof(struct bmp_header) + image_size;
    header.bOffBits = sizeof(struct bmp_header);
    header.biSize = 40; // Размер INFO header
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;
    header.biSizeImage = image_size;
    header.biXPelsPerMeter = 2835; // 72 DPI
    header.biYPelsPerMeter = 2835; // 72 DPI
    header.biClrUsed = 0;
    header.biClrImportant = 0;


    if (fwrite(&header, sizeof(header), 1, out) != 1) {
        return WRITE_IO_ERROR;
    }


    uint8_t padding_bytes[3] = {0, 0, 0};
    for (uint32_t y = 0; y < img->height; y++) {

        uint32_t row_index = img->height - y - 1;
        for (uint32_t x = 0; x < img->width; x++) {
            struct pixel* p = get_pixel(img, x, row_index);
            if (p == NULL || fwrite(p, sizeof(struct pixel), 1, out) != 1) {
                return WRITE_IO_ERROR;
            }
        }

        if (padding > 0 && fwrite(padding_bytes, padding, 1, out) != 1) {
            return WRITE_IO_ERROR;
        }
    }

    return WRITE_OK;
}

enum read_status from_bmp_file(const char* filename, struct image* img) {
    if (filename == NULL || img == NULL) {
        return READ_INVALID_HEADER;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return READ_IO_ERROR;
    }

    enum read_status status = from_bmp(file, img);

    if (fclose(file) != 0 && status == READ_OK) {
        image_destroy(img);
        return READ_IO_ERROR;
    }

    return status;
}

enum write_status to_bmp_file(const char* filename, struct image const* img) {
    if (filename == NULL || img == NULL) {
        return WRITE_INVALID_IMAGE;
    }

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return WRITE_IO_ERROR;
    }

    enum write_status status = to_bmp(file, img);

    if (fclose(file) != 0 && status == WRITE_OK) {
        return WRITE_IO_ERROR;
    }

    return status;
}