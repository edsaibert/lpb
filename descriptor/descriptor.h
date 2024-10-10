#ifndef DESC_H
#define DESC_H

/*
    Trabalho de Programação II - UFPR
    Eduarda Saibert
*/

#include "../includes.h"
#include "../io/io.h"

/*  Calcula qual a imagem mais similar ao input dado    */
void moreSimilar(char *input, char *diretorio);

/*  Checa se o arquivo binário da LBP já existe */
short doesLpbExist(char *f);

/*  Traduz a máscara binária para um número decimal entre 0 e 255   */
void translateBinary(MASK mask, int** binary, int i, int j);

/*  Cria a máscara LBP  */
int** createMask(PGM* pgm);

/*  Cria o objeto LBP   */
LBP* createLbp(PGM *pgm);

/*  Cria o arquivo binário LBP  */
void createLbpFile(LBP* lbp);

/*  Cria a imagem após a transformação pela máscara LBP */
void createLbpImage(LBP* lbp, char *output);

/*  Calcula a distância euclidiana entre duas imagens   */
float eucDistance(char* nome1, char* nome2);

/*  Realiza o free do objeto LBP    */
void freeLbp(LBP *lbp);

/*  Realiza o free do objeto PGM    */
void freePgm(PGM *pgm);

#endif 
