#include <stdio.h>

#define SIZE 20

void createPGM(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open file!\n");
        return;
    }

    // Header
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", SIZE, SIZE); // Width and height
    fprintf(file, "255\n"); // Maximum grayscale value

    // Pixel data
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fprintf(file, "%d ", (i + j) % 256); // Simple pattern for demonstration
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {
    createPGM("image.pgm");
    return 0;
}