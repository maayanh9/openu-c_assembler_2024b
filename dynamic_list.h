#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H


typedef struct DynamicList{
    void** items;
    int item_size;
    int list_length;
    int max_capacity;
    bool is_allocated;
} DynamicList;

void initialize_dynamic_list(DynamicList* list, int item_size, int num_of_elements);

void bigger_capacity(DynamicList* list);

void insert_new_cell_into_dynamic_list(DynamicList* list, void* new_cell);

void free_dynamic_list(DynamicList* list);

#endif
