#ifndef DESC_H
#define DESC_H

#include "../includes.h"
#include "../io/io.h"

char *getNameBeforeDot(char *f);

char *getNameAfterSlash(char *f);

void moreSimilar(char *input, char *diretorio);

short doesLpbExist(char *f);

void translateBinary(MASK mask, int** binary, int i, int j);

int** createMask(PGM* pgm);

LBP* createLbp(PGM *pgm);

void createLbpFile(LBP* lbp);

void createLbpImage(LBP* lbp, char *output);

float eucDistance(char* nome1, char* nome2);

void freeLbp(LBP *lbp);

void freePgm(PGM *pgm);

#endif 
