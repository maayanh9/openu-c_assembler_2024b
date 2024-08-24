#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "settings.h"

#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')
#define CHECK_ALLOCATION(ptr) do { \
    if ((ptr) == NULL) { \
        printf("Failed in allocating memory\n"); \
        exit(1); \
    } \
} while (0)

typedef struct node{
    char *value;
    struct node* next;
} node;

typedef struct macro{
    char macro_name[MAX_LEN_MACRO_NAME];
    node *first_line;
    node *last_line;
} macro;


bool preprocess_macro(const char *input_file_name);
#endif