#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_list.h"
#include "text_handler.h"


void initialize_dynamic_list(DynamicList* list, int item_size){
    list->item_size = item_size;
    list->list_length = 0;
    /*Allocate space for 2 cells*/
    list->items = malloc(list->item_size * 2);
    CHECK_ALLOCATION(list->items);
    list->max_capacity = 2;
}

void bigger_capacity(DynamicList* list){
    /*Double the capacity of the dynamic list by 2*/
    list->max_capacity *= 2;
    list->items = realloc(list->items, list->max_capacity * list->item_size);
    CHECK_ALLOCATION(list->items);
}

void insert_new_cell_into_dynamic_list(DynamicList* list, void* new_cell){
    if( list->list_length == list->max_capacity)
        bigger_capacity(list);
    ((void**)list->items)[list->item_size] = new_cell;
    list->list_length++;
}
