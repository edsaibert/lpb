#ifndef IO_H
#define IO_H

#include "../includes.h"
#include "../descriptor/descriptor.h"

/* Abre o arquivo PGM */
PGM* openPGM(char *f);

/* Verifica se o arquivo é compatível (PGM) */
short isPGM(const char *f);

/*  Abre um diretório e verifica os arquivos dentro dele */
void openDirectory(char* input, char* directory);

void printMatrix(int** m, int width, int height);

#endif