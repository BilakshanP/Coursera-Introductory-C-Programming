#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvpair_t* splitKV(char* line) {
    char* value_temp = strchr(line, '=');
    *value_temp = '\0';
    value_temp++;
    char* end = strchr(value_temp, '\n');
    if (end != NULL) {
        *end = '\0';
    }

    kvpair_t* kvpair = malloc(sizeof(*kvpair));
    kvpair->key = line;
    kvpair->value = value_temp;

    return kvpair;
}

kvarray_t* readKVs(const char* fname) {
    /*In readKVs, you will want to open the file, read the lines of text, split them into
    key/value pairs, add the resulting pairs to an array (hint: realloc it to make it larger
    each time), close the file, and return the kvarray_t * that has your array.*/
    FILE* f = fopen(fname, "r");
    if (f == NULL) {
        perror("could not open file");
        return NULL;
    }
    kvarray_t* kv_pair_array = malloc(sizeof(*kv_pair_array));
    kv_pair_array->length = 0;
    kv_pair_array->kv_array = NULL;

    char* line = NULL;
    size_t line_size = 0;
    kvpair_t* current_pair = NULL;
    int idx = 0;
    while (getline(&line, &line_size, f) >= 0) {
        current_pair = splitKV(line);
        kv_pair_array->kv_array = realloc(kv_pair_array->kv_array, (idx + 1) * (sizeof(*(kv_pair_array->kv_array))));
        kv_pair_array->kv_array[idx] = current_pair;
        current_pair = NULL;
        line = NULL;
        idx++;
    }
    kv_pair_array->length = idx;

    free(line);
    if (fclose(f) != 0) {
        perror("Failed to close the input file!");
        return NULL;
    }
    return kv_pair_array;
}

void freeKVs(kvarray_t* pairs) {
    int len = pairs->length;
    for (int i = 0; i < len; i++) {
        free(pairs->kv_array[i]->key);
        free(pairs->kv_array[i]);
    }
    free(pairs->kv_array);
    free(pairs);
}

void printKVs(kvarray_t* pairs) {
    /*Third, write printKVs which should take a kvarray_t *, and print out

     "key = '%s' value = '%s'\n"
    for each key/value pair in the array, with the first %s being whatever the key is,
    and the second being whatever value (e.g., key = 'Jean Luc Picard' value = 'Captain').*/
    int len = pairs->length;
    kvpair_t** kvarray = pairs->kv_array;

    for (int i = 0; i < len; i++) {
        printf("key = '%s' value = '%s'\n", kvarray[i]->key, kvarray[i]->value);
    }
}

char* lookupValue(kvarray_t* pairs, const char* key) {
    /* Finally, write the lookupValue function, which takes a kvarray_t * and a char * (string).
   The char * that is passed in represents a key.  This function searches the kvarray_t
   for a matching key, and if found, returns the corresponding value.  If no match is found,
   this function returns NULL.*/
    int len = pairs->length;
    kvpair_t** kvarray = pairs->kv_array;

    for (int i = 0; i < len; i++) {
        if (strcmp(key, kvarray[i]->key) == 0) {
            return kvarray[i]->value;
        }
    }
    return NULL;
}
