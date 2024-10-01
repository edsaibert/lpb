#include "includes.h"
// Own
#include "io/io.h"
#include "descriptor/descriptor.h"

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Comando inválido");
    }

    char next_option;
    char *directory = NULL, *input = NULL, *output = NULL;
    while ((next_option = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (next_option)
        {
        case 'd':
            directory = malloc(strlen(optarg) + 1);
            if (!directory)
                return 1;
            strcpy(directory, optarg);
            break;
        case 'i':
            input = malloc(strlen(optarg) + 1);
            if (!input)
                return 1;
            strcpy(input, optarg);
            break;
        case 'o':
            output = malloc(strlen(optarg) + 1);
            if (!output)
                return 1;
            strcpy(output, optarg);
            break;
        default:
            printf("Argumentos: -d ou -i [entrada ou diretório] -i ou -o [entrada ou saída de imagem]");
            break;
        }
    }

    PGM* pgm;
    LBP* lbp;
    if (input){
        if (isPGM(input)){
            pgm = openPGM(input);
            lbp = createLbp(pgm, NULL);
            createLbpFile(lbp);
        }
    }
    if (directory){
        openDirectory(directory);
    }
    if (output){
        createLbpImage(lbp, output);
    }

    // free
    free(directory);
    free(input);
    free(output);

    return 0;
}