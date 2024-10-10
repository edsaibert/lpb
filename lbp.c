#include "includes.h"
// Own
#include "io/io.h"
#include "descriptor/descriptor.h"

int main(int argc, char *argv[])
{
    if (argc < 5)
        return 1;

    char next_option;
    char *directory = NULL, *input = NULL, *output = NULL;

    // Processa os argumentos passados por linha de comando
    while ((next_option = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (next_option)
        {
        case 'd':
            directory = malloc(strlen(optarg) + 1);
            if (!directory)
            {
                // Libera os recursos já alocados antes de retornar
                if (input)
                    free(input);
                if (output)
                    free(output);
                return 1;
            }
            strcpy(directory, optarg);
            break;
        case 'i':
            input = malloc(strlen(optarg) + 1);
            if (!input)
            {
                if (directory)
                    free(directory);
                if (output)
                    free(output);
                return 1;
            }
            strcpy(input, optarg);
            break;
        case 'o':
            output = malloc(strlen(optarg) + 1);
            if (!output)
            {
                if (directory)
                    free(directory);
                if (input)
                    free(input);
                return 1;
            }
            strcpy(output, optarg);
            break;
        default:
            break;
        }
    }

    PGM *pgm = NULL;
    LBP *lbp = NULL;

    if (!input)
    {
        if (directory) free(directory);
        if (output) free(output);
        return 1;
    }

    // Se o arquivo de entrada é PGM
    if (isPGM(input))
    {
        // Abre o arquivo PGM, caso exista
        if (!doesLpbExist(input))
        {
            pgm = openPGM(input);
            lbp = createLbp(pgm);

            if (!lbp && !pgm)
            {
                free(input);

                if (directory)
                    free(directory);
                if (output)
                    free(output);

                return 1;
            }
            createLbpFile(lbp);
        }
        // Se o diretório foi passado, abre o diretório e verifica os arquivos dentro dele
        if (directory)
        {
            openDirectory(input, directory);
            moreSimilar(input, directory);
        }
        // Se o arquivo de saída foi passado, cria a imagem após a transformação pela máscara LBP
        if (output)
        {
            createLbpImage(lbp, output);
        }
    }

    // free
    if (lbp)
        freeLbp(lbp);
    if (pgm)
        freePgm(pgm);

    if (directory)
        free(directory);
    if (input)
        free(input);
    if (output)
        free(output);

    return 0;
}