#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "text_handler.h"
#include "settings.h"

typedef struct SeparateLineIntoWords
{
    char* words[40];
    int words_counter;
}SeparateLineIntoWords;

SeparateLineIntoWords separate_line_into_words(const char *line){
    SeparateLineIntoWords separate_line;
    char *line_copy = string_copy(line);
    char* token_ptr = strtok(line_copy, " \t\r\n");
    separate_line.words_counter = 0;

    while (token_ptr)
    {   
        separate_line.words[separate_line.words_counter] = token_ptr;
        separate_line.words_counter++;
        printf("%s\n", token_ptr);
        token_ptr = strtok(NULL, " \t\r\n");
    }
    return separate_line;
}


ParsedLine parse_line(char* line, LineMetaData counters){
    ParsedLine parsed_line;
    parsed_line.mete_data.instruction_counter = counters.instruction_counter;

    return parsed_line;
}

/** TODO: finish 
void insert_line_into_lines_list(ParsedLine parsed_line, bool* result, DynamicList* lines_list){
    if(lines_list->max_capacity == lines_list->item_size){
        add_more_space(&lines_list);
    }
}*/

bool first_pass(const char *input_file_name){
    bool result = false;
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file;
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];
    LineMetaData counters;
    /*DynamicList lines_list;*/

    char *output_file_name = change_file_extention(input_file_name, FILE_EXTENTION_PREPROCESSOR);
    counters.instruction_counter = 100;
    
    output_file = fopen(output_file_name, "w");

    if(!check_if_file_opened_successfully(input_file) || !check_if_file_opened_successfully(output_file)){
        result = false;
        goto cleanup;}
    

    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, input_file) != NULL){
        ParsedLine parsed_line = parse_line(line, counters);
        printf("%d", parsed_line.mete_data.instruction_counter);
        separate_line_into_words(line);
        /*insert_line_into_lines_list(parsed_line, &result, &lines_list);*/
    }
    result = true; 
cleanup: 
    fclose(input_file);
    fclose(output_file);
    free(output_file_name);
    return result;
}

bool assembler(const char *input_file_name){
    bool ans = first_pass(input_file_name);
    return ans;
}

