#include "descriptor.h"


char *getNameBeforeDot(char *f) {
    char *dot = strrchr(f, '.');
        size_t length = dot - f; 
        char *newf = (char*) malloc((length + 1));
        if (newf) {
            strncpy(newf, f, length);
            newf[length] = '\0'; 
        }
        return newf;
}

short doesLpbExist(char *f){
    f = getNameBeforeDot(f);

    struct dirent *i;
    DIR *dir = opendir("./bin");

    if (dir == NULL) return 0;

    while ((i = readdir(dir))){
        char *path;
        path = getNameBeforeDot(i->d_name);
        if (strcmp(path, f) == 0)
            return 1;
    }    

    return 0;
}