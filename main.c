#include <stdio.h> // fopen, fread, ...
#include <stdlib.h> // malloc, free, ...
#include <string.h> // for memcmp
#include <stdint.h> // for uint8_t

typedef uint8_t ubyte;

struct pixel_data{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Pixel_arr {
    ubyte* data;
    size_t width;
    size_t height;
    ubyte format;
};

typedef struct Pixel_arr PixelArray;

void function(int input, unsigned long size, struct pixel_data *pixel, ubyte buffer[]) {

    FILE *outfile;
    outfile = fopen("../edited.tga", "wb");
    fwrite(buffer, 1, 18, outfile);

    if (input == 1) {
        for (int z = 0; z < size; z++) {
            putc(pixel[z].g, outfile);
            putc(pixel[z].b, outfile);
            putc(pixel[z].r, outfile);
        }
    } else if (input == 2) {
        for (int z = 0; z < size; z++) {
            putc(pixel[z].g, outfile);
            putc(pixel[z].r, outfile);
            putc(pixel[z].b, outfile);
        }
    } else if (input == 3) {
        for (int z = 0; z < size; z++) {
            putc(pixel[z].r, outfile);
            putc(pixel[z].g, outfile);
            putc(pixel[z].b, outfile);
        }
    }
}

int main() {

    int i = 0;
    char c; // 1 byte
    unsigned char u; // 1 byte

    FILE *fp = fopen("../earth.tga", "rb");
    FILE *outfile = NULL;

    if (fp == NULL) {
        fprintf(stderr, "cannot open file\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    // errno;

    fseek(fp, 0, SEEK_SET);
    ubyte *data = (ubyte *) malloc(fsize);

    const ubyte tga_sig[7] = {0, 0, 2, 0, 0, 0, 0};
    const size_t tga_header_size = 18;
    PixelArray pix;
    pix.data = NULL;

    int res = fread(data, 1, fsize, fp);

    if (res < fsize) {
        fprintf(stderr, "invalid tga file\n");
        goto done;
    }

    res = memcmp(tga_sig, data, sizeof(tga_sig));
    if (res != 0) {
        fprintf(stderr, "invalid tga file\n");
        goto done;
    }

    pix.width = data[12] + (data[13] << 8);
    pix.height = data[14] + (data[15] << 8);
    pix.format = data[16];

    if (pix.format != 24) {
        fprintf(stderr, "unsupported tga format\n");
        goto done;
    }

    size_t image_size = pix.width * pix.height * pix.format / 8;

    if ((tga_header_size + image_size) > fsize) {
        fprintf(stderr, "invalid tga file size\n");
        goto done;
    }

    struct pixel_data *pixel;
    pixel = (struct pixel_data *)malloc(pix.width * pix.height * sizeof(struct pixel_data));
    unsigned char component[5];

    int bytes = 3;
    fseek(fp, 0, SEEK_SET);

    while (i < pix.width * pix.height) {
        fread(component, 1, bytes, fp);
        pixel[i].r = component[2];
        pixel[i].g = component[1];
        pixel[i].b = component[0];
        i++;
    }

    pix.data = (ubyte *) malloc(image_size);
    memcpy(pix.data, data + 18, image_size);

    ubyte buffer[18] = {};
    memcpy(buffer, tga_sig, sizeof(tga_sig));
    buffer[12] = pix.width;
    buffer[13] = pix.width >> 8;
    buffer[14] = pix.height;
    buffer[15] = pix.height >> 8;
    buffer[16] = pix.format;

    unsigned long size = pix.height * pix.width;
    int choice;
    printf("Please enter your choice: ");
    scanf("%d", &choice);
    function(choice, size, pixel, buffer);

    done:
    fclose(outfile);

    free(pix.data);
    free(pixel);
    free(data);
    fclose(fp);

    return 0;

}