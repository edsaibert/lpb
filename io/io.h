#ifndef IO_H
#define IO_H

/*
    Trabalho de Programação II - UFPR
    Eduarda Saibert
*/

#include "../includes.h"
#include "../descriptor/descriptor.h"

/* Abre o arquivo PGM */
PGM* openPGM(char *f);

/*  Checa se o arquivo está completamente vazio */
short isFileEmpty(FILE *file);

/* Verifica se o arquivo é compatível (PGM) */
short isPGM(const char *f);

/*  Abre um diretório e verifica os arquivos dentro dele */
void openDirectory(char* input, char* directory);

/*  Auxiliar    */
void printMatrix(int** m, int width, int height);

/*  Retorna o segmento de string após o ponto (dot '.') se existir. 
    Se não existir, retorna uma duplicata da string original
*/
char *getNameBeforeDot(char *f);

/*  Retorna o segmento de string após a última barra '/' se existir.
    Se não existir, retorna uma duplicata da string original
*/
char *getNameAfterSlash(char *f);

/*  Cria o path dado um nome, um prefixo do nome e um sufixo do nome    */
char *createPath(char *name, char *prefix, char *sufix);

#endif