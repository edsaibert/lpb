#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <dirent.h> // opendir, readdir

// Estrutura de uma imagem PGM
typedef struct pgm PGM;
struct pgm {
    char type[3];
    int **matrix;
    int width;
    int height;
    int max_gray;
};


#endif