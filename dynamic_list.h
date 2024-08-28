#ifndef PREPROCESS_H
#define PREPROCESS_H


typedef struct DynamicList{
    void* items;
    int item_size;
    int list_length;
    int max_capacity;
} DynamicList;

#endif
