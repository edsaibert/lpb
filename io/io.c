#include "io.h"

void printMatrix(int **m, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

short isFileEmpty(FILE *file)
{
    if (file == NULL)
    {
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return (size == 0);
}

PGM *openPGM(char *fname)
{
    // Inicialmente, não sabemos se o arquivo é binário ou ASCII
    FILE *file = fopen(fname, "rb");
    if (!file)
    {
        printf("Erro ao abrir o arquivo: %s\n", fname);
        return NULL;
    }

    // Cria a estrutura PGM
    PGM *pgm = malloc(sizeof(PGM));
    if (!pgm)
    {
        fclose(file);
        return NULL;
    }

    // Checa se o arquivo está vazio
    if (isFileEmpty(file))
    {
        printf("O arquivo está vazio: %s\n", fname);
        fclose(file);
        free(pgm);
        return NULL;
    }

    // Se o arquivo for do tipo P2, necessário reabrir o arquivo
    fscanf(file, "%2s", pgm->type);
    if (strcmp(pgm->type, "P2") == 0)
    {
        fclose(file);
        file = fopen(fname, "r");
        fscanf(file, "%2s", pgm->type);
    }

    // Leitura do cabeçalho
    fscanf(file, "%d %d", &pgm->width, &pgm->height);
    fscanf(file, "%d", &pgm->max_gray);

    pgm->path = malloc(strlen(fname) + 1);
    if (!pgm->path)
    {
        fclose(file);
        return NULL;
    }
    strcpy(pgm->path, fname);

    pgm->matrix = malloc((pgm->height + 2) * sizeof(int *));
    if (!pgm->matrix)
    {
        printf("Compre mais memória\n");
        fclose(file);
        return NULL;
    }

    // aloca memória para todas as linhas da matriz e faz o padding
    for (int i = 0; i < pgm->height + 2; i++)
    {
        pgm->matrix[i] = malloc((pgm->width + 2) * sizeof(int));
        if (!pgm->matrix[i])
        {
            printf("Compre mais memória\n");
            fclose(file);
            return NULL;
        }
        memset(pgm->matrix[i], 0, (pgm->width + 2) * sizeof(int)); // padding
    }
    // Ler dados para a matriz

    // Se é do tipo binário (P5)
    if (strcmp(pgm->type, "P5") == 0)
    {
        for (int i = 1; i < pgm->height + 1; i++)
            for (int j = 1; j < pgm->width + 1; j++)
                fread(&pgm->matrix[i][j], sizeof(unsigned char), 1, file);
    }
    // Se é do tipo ASCII (P2)
    else if (strcmp(pgm->type, "P2") == 0)
    {

        for (int i = 1; i < pgm->height + 1; i++)
            for (int j = 1; j < pgm->width + 1; j++)
                fscanf(file, "%d", &pgm->matrix[i][j]);
    }

    return pgm;
};

short isPGM(const char *fname)
{
    const char *dot = strrchr(fname, '.');
    if (dot != NULL)
    {
        if (strcmp(dot, ".pgm") == 0)
            return 1;
    }
    printf("Arquivo não é PGM\n");
    return 0;
}

void openDirectory(char* input, char *directory)
{
    PGM *pgm;
    LBP *lbp;
    struct dirent *i;
    DIR *dir = opendir(directory);

    if (dir == NULL)
    {
        printf("Erro ao abrir diretório\n");
        return;
    }


    // Lê todos os arquivos do diretório e cria todos os LBP's, exceto se já existir
    while ((i = readdir(dir)))
    {
        if (isPGM(i->d_name))
        {
            if (doesLpbExist(i->d_name))
                continue;

            char path[1024];
            snprintf(path, sizeof(path), "%s%s", directory, i->d_name);
            
            pgm = openPGM(path);
            lbp = createLbp(pgm);
            createLbpFile(lbp);

            freeLbp(lbp);
            freePgm(pgm);
        }
    }

    

    closedir(dir);
}

