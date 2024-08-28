#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "text_handler.h"
#include "settings.h"
#include "dynamic_list.h"


typedef struct SeparateLineIntoWords
{
    char* words[40];
    int words_counter;
}SeparateLineIntoWords;



SeparateLineIntoWords separate_line_into_words(char *line){
    SeparateLineIntoWords separate_line;
    char *line_copy = string_copy(line);
    char* token_ptr = strtok(line_copy, " \t\r\n");
    char* new_word;
    separate_line.words_counter = 0;

    while (token_ptr)
    {   
        new_word = string_copy(token_ptr);
        separate_line.words[separate_line.words_counter] = new_word;
        separate_line.words_counter++;
        token_ptr = strtok(NULL, " \t\r\n");
    }
    free(line_copy);
    return separate_line;
}
void free_separate_line(SeparateLineIntoWords* separate_line) {
    int i;
    for (i = 0; i< separate_line->words_counter; i++) {
        free(separate_line->words[i]);
    }
}

bool is_comment_or_empty_line(SeparateLineIntoWords separate_word){
    if(separate_word.words_counter == 0 || separate_word.words[0][0] == ';')
        return true;
    return false;
}
bool has_a_label(SeparateLineIntoWords separate_word){
    if(separate_word.words[0][strlen(separate_word.words[0]) - 1] == ':'){
        /** TODO: check if the label is valid:
         * p41
         * have ascii letters and numbers
         */
        return true;
    }
    return false;
}

/*bool is_directive_line(SeparateLineIntoWords separated_words, int word_ctr){
    if(word_ctr <= )
    if(separated_words.words[word_ctr])
    
}
bool is_a_valid_label(char* label){

}*/

bool string_contains_only_letters_and_numbers(char* string){
    char* letter_ptr = string;
    while (*letter_ptr != '\0'){
        if(!isalnum(*letter_ptr)){
            return false;
        }
        letter_ptr ++;
    }
    return true;
}

bool is_a_dup_label_name(char* label, DynamicList symbols_table){
    int i;
    for(i = 0; i< symbols_table.list_length; i++){
        if(strcmp(label, ((ParsedLine*)symbols_table.items[i])->label) == 0){
            return true;
        }
    }
    return false;
}

bool check_label_validation(char* label, DynamicList symbols_table){
    if(!string_contains_only_letters_and_numbers(label) || is_a_dup_label_name(label, symbols_table))
        return false;
    return true;
}
void error_line_instead_of_label_line(ParsedLine* parsed_line, int line_counter){
    parsed_line->line_type = ERROR_LINE;
    sprintf(parsed_line->LineTypes.error_str, "Invalid label at line %d.\n", line_counter);
}

bool check_validation_and_insert_label_data(ParsedLine* parsed_line, int* words_ctr, char* first_word_in_line, DynamicList *symbols_table, int line_counter){
    parsed_line->has_label = HAS_LABEL;
    /*Copy the label without the ':' */
    strncpy(parsed_line->label, first_word_in_line, strlen(first_word_in_line) - 1);
    (*parsed_line).label[strlen(first_word_in_line) - 1] = '\0';
    (*words_ctr) ++;
    if(!check_label_validation(parsed_line->label, *symbols_table)){
        error_line_instead_of_label_line(parsed_line, line_counter);
        return false;
    }
    insert_new_cell_into_dynamic_list(symbols_table, &parsed_line);
    return true;
}

ParsedLine parse_line(char* line, LineMetaData counters, DynamicList *symbols_table, DynamicList *errors_ptrs){
    ParsedLine parsed_line;
    SeparateLineIntoWords separated_words = separate_line_into_words(line);
    int words_ctr = 0;
    parsed_line.mete_data.instruction_counter = counters.instruction_counter;
    if (is_comment_or_empty_line(separated_words)){
        printf("%s : comment or empty\n", line);
        parsed_line.line_type = EMPTY_OR_COMMENT_LINE;
        goto finished_parsing_line;
    }

    if (has_a_label(separated_words)){
        printf("has a label:  %s\n", line);
        if(!check_validation_and_insert_label_data(&parsed_line, &words_ctr, separated_words.words[0], symbols_table, counters.line_counter)){
            goto finished_parsing_line;
        }

        
        /** TODO: check if label has more words after. i */
    }
    /*if (is_directive_line(separated_words, words_ctr)){
    }*/
finished_parsing_line:
    free_separate_line(&separated_words);
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
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];
    LineMetaData counters;
    DynamicList parsed_lines_list;
    DynamicList symbols_table;
    DynamicList errors_ptrs;
    initialize_dynamic_list(&parsed_lines_list, sizeof(ParsedLine));
    initialize_dynamic_list(&symbols_table, sizeof(ParsedLine *));
    initialize_dynamic_list(&errors_ptrs, sizeof(ParsedLine *));

    counters.instruction_counter = 100;
    counters.line_counter = 1;

    if(!check_if_file_opened_successfully(input_file)){
        result = false;
        goto cleanup;}
    

    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, input_file) != NULL){
        ParsedLine parsed_line = parse_line(line, counters, &symbols_table, &errors_ptrs);
        counters.line_counter ++;
        printf("%d\t", parsed_line.mete_data.instruction_counter);
        insert_new_cell_into_dynamic_list(&parsed_lines_list, &parsed_line);
        /*insert_line_into_lines_list(parsed_line, &result, &lines_list);*/

    }
    result = true; 
cleanup: 
    fclose(input_file);
    free_dynamic_list(&parsed_lines_list);
    free_dynamic_list(&symbols_table);
    free_dynamic_list(&errors_ptrs);
    return result;
}

bool assembler(const char *input_file_name){
    bool ans = first_pass(input_file_name);
    return ans;
}

