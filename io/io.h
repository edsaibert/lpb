#ifndef IO_H
#define IO_H

#include "../includes.h"

// Estrutura de uma imagem PGM
typedef struct pgm PGM;
struct pgm {
    char type[3];
    int **matrix;
    int width;
    int height;
    int max_gray;
};

/* Abre o arquivo PGM */
PGM* openPGM(char *f);

/* Verifica se o arquivo é compatível (PGM) */
short isPGM(const char *f);

/*  Abre um diretório e verifica os arquivos dentro dele */
void openDirectory(char* directory);

void printMatrix(int** m, int width, int height);

void compareLpbImages(char* directory, char* input);

void makeLpbImage(char* input, char* output);

#endif