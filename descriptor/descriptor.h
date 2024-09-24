#ifndef DESC_H
#define DESC_H

#include "../includes.h"

char *getNameBeforeDot(char *f);

short doesLpbExist(char *f);

void translateBinary(MASK mask, int** binary, int i, int j);

int** createMask(PGM* pgm);

LBP* createLbp(PGM *pgm);

LBP* createLbpFile(PGM *pgm);

#endif 
