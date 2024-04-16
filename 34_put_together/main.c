#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t *countFile(const char *filename, kvarray_t *kvPairs) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Could not open file");
        return NULL;
    }
    counts_t *counts_arr = createCounts();
    char *line = NULL;
    size_t size = 0;
    char *value = NULL;
    while (getline(&line, &size, f) >= 0) {
        char *n = strchr(line, '\n');
        if (n != NULL) {
            *n = '\0';
        }
        value = lookupValue(kvPairs, line);
        addCount(counts_arr, value);
    }
    free(line);

    if (fclose(f) != 0) {
        perror("fail to close the file\n");
        return NULL;
    }

    return counts_arr;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "no enough arguments");
        return EXIT_FAILURE;
    }
    kvarray_t *kv_arr = readKVs(argv[1]);
    if (kv_arr == NULL) {
        fprintf(stderr, "fail to read the kv_pair file\n");
        return EXIT_FAILURE;
    }
    printf("read kv_pair_sucess!!!\n");
    for (int i = 2; i < argc; i++) {
        counts_t *c = countFile(argv[i], kv_arr);
        printf("count sucessful!!!\n");
        if (c == NULL) {
            return EXIT_FAILURE;
        }
        char *outName = computeOutputFileName(argv[i]);
        printf("make output file name\n");
        FILE *f = fopen(outName, "w");
        printCounts(c, f);
        printf("witre output file\n");
        if (fclose(f) != 0) {
            perror("fail to close the file\n");
            return EXIT_FAILURE;
        }

        freeCounts(c);
        free(outName);
    }
    freeKVs(kv_arr);

    return EXIT_SUCCESS;
}
