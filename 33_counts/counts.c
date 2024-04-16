#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

counts_t* createCounts(void) {
    /*The first, createCounts should allocate memory for a counts_t structure, and initialize
  it to represent that nothing has been counted yet.*/
    counts_t* counts_list = malloc(sizeof(*counts_list));
    counts_list->count_array = NULL;
    counts_list->length = 0;
    counts_list->un_known_count = 0;
    return counts_list;
}

void addCount(counts_t* c, const char* name) {
    /*The next function, addCount, should increment the count for the corresponding name.  Note
  that name will be NULL in the case of something that is unknown, so your code must account
  for this case.   */
    if (name == NULL) {
        c->un_known_count++;
    } else {
        int size = c->length;
        int status = 0;  // 0 means no name found in current
        for (int i = 0; i < size; i++) {
            one_count_t* current_count = c->count_array[i];
            if (strcmp(name, current_count->str) == 0) {
                current_count->count++;
                status = 1;
                break;
            }
        }
        if (status == 0) {
            c->count_array = realloc(c->count_array, (c->length + 1) * sizeof(*c->count_array));
            one_count_t* one_count = malloc(sizeof(*one_count));
            one_count->count = 1;
            one_count->str = malloc(strlen(name) + 1);
            strcpy(one_count->str, name);
            c->count_array[c->length] = one_count;
            c->length++;
        }
    }
}

void printCounts(counts_t* c, FILE* outFile) {
    /*The third function, printCounts takes a counts_t and prints that information to
  the FILE outFile. Recall from the description of the entire problem, that this
  function should print in the format:

  Captain: 1
  Commander: 2
  Lt. Commander: 1
  <unknown> : 1*/
    int len = c->length;
    int unkown_size = c->un_known_count;
    one_count_t** counts_arr = c->count_array;

    for (int i = 0; i < len; i++) {
        fprintf(outFile, "%s: %d\n", counts_arr[i]->str, counts_arr[i]->count);
    }
    if (unkown_size > 0) {
        fprintf(outFile, "<unknown> : %d\n", unkown_size);
    }
}

void freeCounts(counts_t* c) {
    int len = c->length;

    for (int i = 0; i < len; i++) {
        free(c->count_array[i]->str);
        free(c->count_array[i]);
    }
    free(c->count_array);
    free(c);
}
