#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* computeOutputFileName(const char* inputName) {
    char* outname = malloc((strlen(inputName) + 10) * sizeof(*outname));
    strcpy(outname, inputName);
    strcat(outname, ".counts");
    return outname;
}
