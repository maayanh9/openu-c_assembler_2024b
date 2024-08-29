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
        return true;
    }
    return false;
}
bool is_a_directive(char* next_unparsed_word){
    /* check if the word starts with a dot, does not check if its a valid directive*/
    if(next_unparsed_word[0] == '.'){
        return true;
    }
    return false;
}

int which_directive(char* next_unparsed_word){
    /*directive is the string after the '.' */
    char * directive = next_unparsed_word + sizeof(char);
    int i;
    for(i = 0; i < LEN_OF_DIRECTIVE_LIST; i++){
        if(strcmp(directives_list[i], directive) == 0){
            return i;
            printf("-");
        }
    }
    return -1;
}

bool is_the_last_word_in_this_line(int parsed_words_counter, int how_many_words_in_line){
    return (parsed_words_counter == how_many_words_in_line);
}

void error_line(ParsedLine* parsed_line, int line_counter, int parsed_words_ctr, int how_many_words_in_line, char* type, char* invalid_data, DynamicList *errors_ptrs){
    /*type: directive, label or command*/
    parsed_line->line_type = ERROR_LINE;
    if(is_the_last_word_in_this_line(parsed_words_ctr, how_many_words_in_line)){
        sprintf(parsed_line->LineTypes.error_str, "%s with no context after: %s, at line %d.\n", type, parsed_line->label, line_counter);
    }
    else{
        sprintf(parsed_line->LineTypes.error_str, "Invalid %s: %s, at line %d.\n", type, invalid_data, line_counter);
    }
    insert_new_cell_into_dynamic_list(errors_ptrs, parsed_line);
    printf("%s\t**********", parsed_line->LineTypes.error_str);
    
}
bool is_valid_directive_call(int directive_num, int parsed_words_ctr, int how_many_words_in_line){
    return (directive_num != -1 && !is_the_last_word_in_this_line(parsed_words_ctr, how_many_words_in_line));
}

bool does_not_end_with_comma(char* data_parameters){
    return data_parameters[sizeof(data_parameters) - 1] != ',';
}
bool has_only_numbers_and_commas(char* data_parameters){
    /*code*/
}
bool does_not_have_sequence_of_commas(char* data_parameters){
    /*code*/
}
bool valid_data_num_parameters(char* data_parameters){
    /*code*/
}

bool insert_data_directive_into_parsed_line(ParsedLine* parsed_line, int* parsed_words_ctr, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs){
    /*Connect again the separate words of the .data parameters, and then parse it (check if its valid) 
    and insert it into the parsed_line struct*/
    char data_parameters[MAX_LEN_LINE_ASSEMBLY_FILE - 5];
    int connected_words_counter = parsed_words_ctr;
    while (separated_words.words_counter > connected_words_counter){
        strcat(data_parameters, separated_words.words[connected_words_counter]);
        connected_words_counter ++;
    }
    
}

bool insert_directive_parameters(ParsedLine* parsed_line, int* parsed_words_ctr, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs){
    AssemblyDirective directive_type = parsed_line->LineTypes.Directive.directive_type;
    switch (directive_type)
    {
    case DATA:
        /* code */
        break;
    case STRING:
        /* code */
        break;
    case EXTERN:
        /* code */
        break;
    case ENTRY:
        /* code */
        break;
    
    default:
        break;
    }
    return true;
}

bool check_validation_and_insert_directive_parameters(ParsedLine* parsed_line, int* parsed_words_ctr, DynamicList *symbols_table, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs){
    int directive_num = which_directive(separated_words.words[*parsed_words_ctr]);
    parsed_line->line_type = DIRECTIVE_LINE;
    if(is_valid_directive_call(directive_num, *parsed_words_ctr, separated_words.words_counter)){
        parsed_line->LineTypes.Directive.directive_type = directive_num;
        (*parsed_words_ctr) ++;
        if(insert_directive_parameters(parsed_line, parsed_words_ctr, line_counter, separated_words))
            return true;
        else{
            /** TODO: error line  */
            return false;
        }
    }
    else{
        error_line(parsed_line, line_counter, *parsed_words_ctr, separated_words.words_counter, "directive", separated_words.words[*parsed_words_ctr], errors_ptrs);
        return false;
    }
}


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
        ParsedLine* parsed_line = (ParsedLine*)symbols_table.items[i];
        
        if(strcmp(label, parsed_line->label) == 0){
            return true;
        }
    }
    return false;
}

bool is_valid_label(char* label, DynamicList symbols_table, int parsed_words_ctr, int how_many_words_in_line){
    if(!string_contains_only_letters_and_numbers(label) || is_a_dup_label_name(label, symbols_table) || is_the_last_word_in_this_line(parsed_words_ctr, how_many_words_in_line))
        return false;
    return true;
}


bool check_validation_and_insert_label_data(ParsedLine* parsed_line, int* parsed_words_ctr, char* first_word_in_line, DynamicList *symbols_table, int line_counter, int words_in_line_counter, DynamicList *errors_ptrs){
    parsed_line->has_label = HAS_LABEL;
    /*Copy the label without the ':' */
    strncpy(parsed_line->label, first_word_in_line, strlen(first_word_in_line) - 1);
    (*parsed_line).label[strlen(first_word_in_line) - 1] = '\0';
    (*parsed_words_ctr) ++;
    if(!is_valid_label(parsed_line->label, *symbols_table, *parsed_words_ctr, words_in_line_counter)){
        error_line(parsed_line, line_counter, *parsed_words_ctr, words_in_line_counter, "label", parsed_line->label, errors_ptrs);
        return false;
    }
    insert_new_cell_into_dynamic_list(symbols_table, parsed_line);
    return true;
}

ParsedLine* parse_line(char* line, LineMetaData counters, DynamicList *symbols_table, DynamicList *errors_ptrs, ParsedLine *parsed_line){
    SeparateLineIntoWords separated_words = separate_line_into_words(line);
    int parsed_words_ctr = 0;
    parsed_line->mete_data.instruction_counter = counters.instruction_counter;
    if (is_comment_or_empty_line(separated_words)){
        printf("%s : comment or empty\n", line);
        parsed_line->line_type = EMPTY_OR_COMMENT_LINE;
        goto finished_parsing_line;
    }

    if (has_a_label(separated_words)){
        if(!check_validation_and_insert_label_data(parsed_line, &parsed_words_ctr, separated_words.words[0], symbols_table, counters.line_counter, separated_words.words_counter, errors_ptrs)){
            goto finished_parsing_line;
        }
    }
    if(is_a_directive(separated_words.words[parsed_words_ctr])){
        check_validation_and_insert_directive_parameters(parsed_line, &parsed_words_ctr, symbols_table, counters.line_counter, separated_words, errors_ptrs);
    }

finished_parsing_line:
    free_separate_line(&separated_words);
    return parsed_line;
}


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
        ParsedLine *parsed_line = malloc(sizeof(ParsedLine));
        CHECK_ALLOCATION(parsed_line);
        parse_line(line, counters, &symbols_table, &errors_ptrs, parsed_line);
        counters.line_counter ++;
        printf("%d\t", parsed_line->mete_data.instruction_counter);
        insert_new_cell_into_dynamic_list(&parsed_lines_list, parsed_line);
        parsed_lines_list.is_allocated = true;
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

