#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "dynamic_list.h"

/* Assemble a pre-processed input file name
 * @input_file_name should be pre-processed
 */
bool assembler(const char *input_file_name);


typedef struct FirstPassOutput{
    bool success;
    DynamicList parsed_lines_list;
    DynamicList symbols_table;
    DynamicList errors_ptrs;
    DynamicList entry_ptrs;
    DynamicList external_ptrs;
    DynamicList direct_labels_ptrs;
    int data_section_begin_address;
    int capacity_needed_for_object_file;
} FirstPassOutput;

typedef enum{
    DESTINATION = 1,
    SOURCE
}SourceOrDest;

/* takes a given input file name and parses all the lines inside the file into a list of ParsedLine structs
 * also keep pointers to lines for symbols table, entry table, extern table and labels from direct addressing method
 * move those data sturctures to the second pass
 */
FirstPassOutput first_pass(const char *input_file_name);

#endif