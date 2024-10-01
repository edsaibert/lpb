#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <dirent.h> // opendir, readdir

// mask size
#define MASKSIZE 8

// Estrutura de uma imagem PGM
typedef struct pgm PGM;
struct pgm {
    char type[3];
    char *path;
    int **matrix;
    int width;
    int height;
    int max_gray;
};

typedef struct lbp LBP;
struct lbp {
    char type[3];
    char *path;
    int **matrix;
    int *histogram;
    int width;
    int height;
    int max_gray;
};


typedef int MASK[MASKSIZE]; 

#endif