#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamic_list.h"
#include "text_and_digits_handler.h"

/* Initialize the minimum resources for a gien dynamic list*/
void initialize_dynamic_list(DynamicList* list, int item_size, int num_of_elements){
    list->item_size = item_size;
    list->list_length = 0;
    list->is_allocated = false;
    /*Allocate space for 2 cells*/
    list->items = malloc(list->item_size * num_of_elements);
    CHECK_ALLOCATION(list->items);
    list->max_capacity = num_of_elements;
}

/* increase the capacity of the list*/
void bigger_capacity(DynamicList* list){
    /*Double the capacity of the dynamic list by 2*/
    list->max_capacity *= 2;
    list->items = realloc(list->items, list->max_capacity * list->item_size);
    CHECK_ALLOCATION(list->items);
}

/* insert given cell value to the end of the list */
void insert_new_cell_into_dynamic_list(DynamicList* list, void* new_cell){
    /* insert value at the end of the list*/
    if( list->list_length == list->max_capacity)
        bigger_capacity(list);
    ((void**)list->items)[list->list_length] = new_cell;
    list->list_length++;
}

/* Free all resources allocated to given list */
void free_dynamic_list(DynamicList* list){
    if((*list).is_allocated == true){
        int i;
        for(i = 0; i<(*list).list_length; i++){
            free(list->items[i]);
        }
    }
    free(list->items);
    list->items = NULL;
}
