#ifndef PREPROCESS_H
#define PREPROCESS_H


typedef struct DynamicList{
    void* items;
    int item_size;
    int list_length;
    int max_capacity;
} DynamicList;

void initialize_dynamic_list(DynamicList* list, int item_size);

void bigger_capacity(DynamicList* list);

void insert_new_cell_into_dynamic_list(DynamicList* list, void* new_cell);

#endif
