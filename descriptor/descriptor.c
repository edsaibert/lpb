#include "descriptor.h"

void translateBinary(MASK mask, int **binary, int i, int j)
{
    int pow = 1;
    binary[i][j] = 0;
    for (int k = 0; k < MASKSIZE; k++)
    {
        binary[i][j] += mask[k] * pow;
        pow *= 2;
    }
}

int **createMask(PGM *pgm)
{
    MASK mask;

    int **binary = malloc((pgm->height) * sizeof(int *));
    for (int i = 0; i < pgm->height; i++)
    {
        binary[i] = malloc((pgm->width) * sizeof(int));
        if (!binary[i])
            return NULL;
    }

    if (!binary)
        return NULL;

    for (int i = 1; i < pgm->height + 1; i++)
    {
        for (int j = 1; j < pgm->width + 1; j++)
        {
            int l = 0;
            for (int k = j - 1; k <= j + 1; k++)
            {
                if (pgm->matrix[i - 1][k] > pgm->matrix[i][j])
                    mask[l] = 1;
                else
                    mask[l] = 0;
                l++;
            }

            if (pgm->matrix[i][j + 1] > pgm->matrix[i][j])
                mask[l] = 1;
            else
                mask[l] = 0;
            l++;

            for (int k = j + 1; k >= j - 1; k--)
            {

                if (pgm->matrix[i + 1][k] > pgm->matrix[i][j])
                    mask[l] = 1;
                else
                    mask[l] = 0;
                l++;
            }

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
    LBP *lbp;
    lbp = malloc(sizeof(LBP));
    if (!lbp)
        return NULL;

    strcpy(lbp->type, pgm->type);
    lbp->width = pgm->width;
    lbp->height = pgm->height;
    lbp->max_gray = 255;
    lbp->matrix = createMask(pgm);

    // changing path
    lbp->path = malloc(strlen(pgm->path) + 1);
    if (!lbp->path)
        return NULL;
    snprintf(lbp->path, strlen(pgm->path) + 1, "%s.lbp", getNameBeforeDot(pgm->path));
    printf("%s\n", pgm->path);

    return lbp;
}

void createLbpImage(PGM *pgm, char *output)
{
    LBP *lbp;
    lbp = createLbp(pgm);

    char path[1024];
    snprintf(path, sizeof(path), "./lbp_img/%s", output);

    printf("%s\n", path);
    FILE *file = fopen(path, "w");
    if (!file)
    {
        perror("Failed to open file");
        return;
    }

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

        for (int i = 1; i < lbp->height + 1; i++)
            for (int j = 1; j < lbp->width + 1; j++)
                fscanf(file, "%d", &lbp->matrix[i][j]);
    }

    fclose(file);
}

char *getNameBeforeDot(char *f)
{
    char *dot = strrchr(f, '.');
    size_t length = dot - f;
    char *newf = (char *)malloc((length + 1));
    if (newf)
    {
        strncpy(newf, f, length);
        newf[length] = '\0';
    }
    return newf;
}

short doesLpbExist(char *f)
{
    f = getNameBeforeDot(f);

    struct dirent *i;
    DIR *dir = opendir("./bin");

    if (dir == NULL)
        return 0;

    while ((i = readdir(dir)))
    {
        char *path;
        path = getNameBeforeDot(i->d_name);
        if (strcmp(path, f) == 0)
        {
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}