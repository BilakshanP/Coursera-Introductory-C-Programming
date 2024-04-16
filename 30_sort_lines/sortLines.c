#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stringOrder(const void *vp1, const void *vp2) {
    const char *const *p1 = vp1;
    const char *const *p2 = vp2;
    return strcmp(*p1, *p2);
}
void sortData(char **data, size_t count) {
    qsort(data, count, sizeof(char *), stringOrder);
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        for (int x = 1; x < argc; x++) {
            char *line = NULL;
            size_t line_size = 0;
            char **str_array = NULL;
            size_t idx = 0;
            FILE *f = fopen(argv[x], "r");
            if (f == NULL) {
                perror("could not open file");
                return EXIT_FAILURE;
            }
            while (getline(&line, &line_size, f) >= 0) {
                str_array = realloc(str_array, (idx + 1) * sizeof(*str_array));
                str_array[idx] = line;
                line = NULL;
                idx++;
            }
            free(line);
            sortData(str_array, idx);

            for (size_t i = 0; i < idx; i++) {
                printf("%s", str_array[i]);
                free(str_array[i]);
            }
            free(str_array);

            if (fclose(f) != 0) {
                perror("Failed to close the input file!");
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        char *line = NULL;
        size_t line_size = 0;
        char **str_array = NULL;
        size_t idx = 0;
        while (getline(&line, &line_size, stdin) >= 0) {
            str_array = realloc(str_array, (idx + 1) * sizeof(*str_array));
            str_array[idx] = line;
            line = NULL;
            idx++;
        }
        free(line);
        sortData(str_array, idx);

        for (size_t i = 0; i < idx; i++) {
            printf("%s", str_array[i]);
            free(str_array[i]);
        }
        free(str_array);
        return EXIT_SUCCESS;
    }

    else {
        fprintf(stderr, "not enough arguments\n");
        return EXIT_FAILURE;
    }
}
