#ifndef INCLUDES_H
#define INCLUDES_H

/* 
    Trabalho de Programação II - UFPR
    Eduarda Saibert
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt
#include <dirent.h> // opendir, readdir
#include <math.h> 
#include <float.h>

// mask size
#define MASKSIZE 8

// Estrutura de uma imagem PGM
typedef struct pgm PGM;
struct pgm {
    char type[3]; // P2 ou P5
    char *path; // Caminho ou nome do arquivo
    int **matrix; // Matriz de pixels
    int width; // Largura
    int height; // Altura
    int max_gray; // Máximo de tons de cinza
};

// Estrutura de um LBP
typedef struct lbp LBP;
struct lbp {
    char type[3]; // P2 ou P5
    char *path; // Caminho ou nome do arquivo
    int **matrix; // Matriz de pixels
    int *histogram; // Histograma
    int width;  // Largura
    int height; // Altura
    int max_gray; // Máximo de tons de cinza
};

typedef int MASK[MASKSIZE]; 

#endif