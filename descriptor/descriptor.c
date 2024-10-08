#include "descriptor.h"

void freeLbp(LBP *lbp)
{
    for (int i = 0; i < lbp->height; i++)
        if (lbp->matrix[i])
            free(lbp->matrix[i]);

    if (lbp->matrix)
        free(lbp->matrix);
    if (lbp->histogram)
        free(lbp->histogram);
    if (lbp->path)
        free(lbp->path);
    free(lbp);
}

void freePgm(PGM *pgm)
{
    for (int i = 0; i <= pgm->height + 1; i++)
        if (pgm->matrix[i])
            free(pgm->matrix[i]);

    if (pgm->matrix)
        free(pgm->matrix);
    if (pgm->path)
        free(pgm->path);
    free(pgm);
}

void moreSimilar(char *input, char *diretorio)
{
    float dist = -1;
    float min = FLT_MAX; // Inicializar min com FLT_MAX
    char *minName = malloc(1023);
    char *aux = NULL;
    struct dirent *i;
    DIR *dir = opendir(diretorio);

    if (dir == NULL)
    {
        return;
    }

    // Enquanto o diretório estiver aberto, lê todos os arquivos do diretório
    while ((i = readdir(dir)))
    {
        if (!isPGM(i->d_name))
            continue;

        char path[1023];
        snprintf(path, sizeof(path), "%s/%s", diretorio, i->d_name);

        // Calcula a distância euclidiana entre o input e o arquivo atual
        aux = getNameAfterSlash(input);
        dist = eucDistance(aux, path);


        // Se a distância for menor que a mínima, atualiza a distância mínima e o nome do arquivo
        if (dist < min)
        {
            min = dist;
            strcpy(minName, i->d_name);
        }

        free(aux);
    }

    aux = getNameBeforeDot(minName);

    // Imprime o nome do arquivo mais similar e a distância euclidiana
    printf("%s %.6f\n", aux, min);

    free(minName);
    free(aux);
    closedir(dir);
}

/*
    Calcula a distância euclidiana entre dois vetores
*/
float eucDistance(char *nome1, char *nome2)
{
    if (!doesLpbExist(nome1) && !doesLpbExist(nome2))
    {
        return -1;
    }

    char* aux1 = getNameAfterSlash(nome1);
    char* aux2 = getNameAfterSlash(nome2);

    char *path1 = createPath(aux1, "./bin/", ".lbp");
    char *path2 = createPath(aux2, "./bin/", ".lbp");

    // Abre os arquivos binários
    FILE *file1 = fopen(path1, "r");
    FILE *file2 = fopen(path2, "r");

    if (!file1 || !file2)
        return -1;


    // Calcula a distância euclidiana entre os dois vetores
    int dist = 0;
    unsigned char h1, h2;
    for (int i = 0; i < 256; i++)
    {
        fread(&h1, sizeof(unsigned char), 1, file1);
        fread(&h2, sizeof(unsigned char), 1, file2);
        dist += (h1 - h2) * (h1 - h2);
    }

    fclose(file1);
    fclose(file2);
    free(path1);
    free(path2);
    free(aux1);
    free(aux2);

    return sqrt(dist);
}

int *createHistogram(int height, int width, int **matrix)
{
    int *histogram = malloc(sizeof(int) * 256);

    // Inicializa o histograma com 0
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    // Calcula o histograma
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            histogram[matrix[i][j]]++;
        }
    }

    return histogram;
}

void translateBinary(MASK mask, int **binary, int i, int j)
{
    int pow = 1;
    binary[i][j] = 0;

    // Traduz a máscara binária para um número decimal entre 0 e 255
    for (int k = 0; k < MASKSIZE; k++)
    {
        binary[i][j] += mask[k] * pow;
        pow *= 2;
    }
}

int **createMask(PGM *pgm)
{
    MASK mask;

    // Aloca memória para a matriz binária
    int **binary = malloc((pgm->height) * sizeof(int *));
    for (int i = 0; i < pgm->height; i++)
    {
        binary[i] = malloc((pgm->width) * sizeof(int));
        if (!binary[i])
            return NULL;
    }

    if (!binary)
        return NULL;

    // Cria a máscara LBP 
    // (calcula os vizinhos como um vetor, começando pelo pixel superior esquerdo em sentido horário)
    for (int i = 1; i < pgm->height + 1; i++)
    {
        for (int j = 1; j < pgm->width + 1; j++)
        {
            int l = 0;

            // vizinhos da linha de cima
            for (int k = j - 1; k <= j + 1; k++) 
            {
                if (pgm->matrix[i - 1][k] > pgm->matrix[i][j])
                    mask[l] = 1;
                else
                    mask[l] = 0;
                l++;
            }

            /// vizinho direito da linha atual
            if (pgm->matrix[i][j + 1] > pgm->matrix[i][j])
                mask[l] = 1;
            else
                mask[l] = 0;
            l++;

            /// vizinhos da linha de baixo
            for (int k = j + 1; k >= j - 1; k--)
            {

                if (pgm->matrix[i + 1][k] > pgm->matrix[i][j])
                    mask[l] = 1;
                else
                    mask[l] = 0;
                l++;
            }

            // vizinho esquerdo da linha atual
            if (pgm->matrix[i][j - 1] > pgm->matrix[i][j])
                mask[l] = 1;
            else
                mask[l] = 0;
            l++;

            translateBinary(mask, binary, i - 1, j - 1);
        }
    }

    return binary;
}

LBP *createLbp(PGM *pgm)
{
    LBP *lbp = NULL;
    lbp = malloc(sizeof(LBP));
    if (!lbp)
        return NULL;

    strcpy(lbp->type, pgm->type);
    lbp->width = pgm->width;
    lbp->height = pgm->height;
    lbp->max_gray = 255;
    lbp->matrix = createMask(pgm);
    lbp->histogram = createHistogram(lbp->height, lbp->width, lbp->matrix);

    // Garante que o nome do arquivo não tenha a extensão .pgm e nem o caminho 
    char *nBeforeDot = getNameBeforeDot(pgm->path);
    char *nAfterSlash = getNameAfterSlash(nBeforeDot);

    lbp->path = malloc(strlen(nAfterSlash) + 1);
    if (!lbp->path){
        free(nAfterSlash);
        free(nBeforeDot);
        freeLbp(lbp);
        return NULL;
    }

    snprintf(lbp->path, strlen(nAfterSlash) + 1, "%s", nAfterSlash);

    free(nAfterSlash);
    free(nBeforeDot);
    return lbp;
}

/*
    Função que cria um arquivo binário .lbp com um vetor LBP a partir de uma imagem PGM
*/
void createLbpFile(LBP *lbp)
{
    if (!lbp && !lbp->path)
        return;

    char *path = createPath(lbp->path, "./bin/", ".lbp");

    if (!path)
        return;

    FILE *file = fopen(path, "w");

    // Escreve o histograma no arquivo binário
    for (int i = 0; i < 256; i++)
    {
        unsigned char h = (unsigned char)lbp->histogram[i];
        fwrite(&h, sizeof(unsigned char), 1, file);
    }

    fclose(file);
    free(path);
}

void createLbpImage(LBP *lbp, char *output)
{
    if (!lbp)
        return;

    char *path = createPath(output, "./lbp_img/", ".pgm");
    FILE *file = fopen(path, "w");

    if (!file)
    {
        return;
    }

    // Escreve o cabeçalho do arquivo PGM
    fprintf(file, "%s\n%d %d\n%d\n", lbp->type, lbp->width, lbp->height, lbp->max_gray);

    // Se é do tipo binário (P5)
    if (strcmp(lbp->type, "P5") == 0)
    {
        for (int i = 0; i < lbp->height; i++)
            for (int j = 0; j < lbp->width; j++)
            {
                unsigned char pixel = (unsigned char)lbp->matrix[i][j];
                fwrite(&pixel, sizeof(unsigned char), 1, file);
            }
    }
    // Se é do tipo ASCII (P2)
    else if (strcmp(lbp->type, "P2") == 0)
    {

        for (int i = 0; i < lbp->height; i++)
        {
            for (int j = 0; j < lbp->width; j++)
                fprintf(file, "%d ", lbp->matrix[i][j]);
            fprintf(file, "\n");
        }
    }

    fclose(file);
    free(path);
}

short doesLpbExist(char *f)
{
    f = getNameBeforeDot(f);

    if (!f)
        return 0;

    char *path = NULL;
    struct dirent *i;
    DIR *dir = opendir("./bin");

    if (dir == NULL)
    {
        free(f);
        closedir(dir);
        return 0;
    }

    while ((i = readdir(dir)))
    {
        path = getNameBeforeDot(i->d_name);

        if (!path)
            continue;

        if (strcmp(path, f) == 0)
        {
            free(f);
            free(path);
            closedir(dir);
            return 1;
        }
        free(path);
    }

    free(f);
    closedir(dir);
    return 0;
}