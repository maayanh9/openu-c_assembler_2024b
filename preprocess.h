#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "settings.h"
#include "text_handler.h"


#define IS_SPACE_OR_TAB(char1) (char1 == ' ' || char1 == '\t')


typedef struct Node{
    char *value;
    struct Node* next;
} Node;

typedef struct DynamicList{
    void* items;
    int item_size;
    int list_length;
    int max_capacity;
} DynamicList;


bool preprocess_macro(const char *input_file_name);
#endif