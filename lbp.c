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
            break;
        }
    }

    PGM *pgm;
    LBP *lbp;

    if (!input)
        return 1;

    if (isPGM(input))
    {
        if (!doesLpbExist(input))
        {
            pgm = openPGM(input);
            lbp = createLbp(pgm);
            createLbpFile(lbp);
        }
        if (directory)
        {
            openDirectory(input, directory);
            moreSimilar(input, directory);
        }
        if (output)
        {
            createLbpImage(lbp, output);
        }
    }
    // free
    free(directory);
    free(input);
    free(output);

    return 0;
}