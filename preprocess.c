#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"
#include "text_handler.h"

typedef enum {
    REGULAR_LINE,
    MACR_CREATION_CALL,
    INSIDE_THE_MACRO,
    ENDMACR_LINE,
    CALLED_A_MACRO
} LineMacroState;

typedef struct Macro{
    char macro_name[MAX_LEN_MACRO_NAME];
    Node *first_line;
    Node *last_line;
} Macro;

typedef struct PreprocessParsedLine
{
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];
    int how_many_elements_in_line;
    char first_word_in_line[MAX_LEN_OF_A_SINGLE_WORD];
} PreprocessParsedLine;


bool is_macro_declaration_line(PreprocessParsedLine parsed_line){
    if(strcmp(parsed_line.first_word_in_line, "macr") == 0)
        return true;
    return false;
}

bool is_macro_end_statement_line(PreprocessParsedLine parsed_line){
    if(strcmp(parsed_line.first_word_in_line, "endmacr") == 0){
        if (parsed_line.how_many_elements_in_line == 1)
            return true;
        else{
            printf("invalid line at an end macro statement \"endmacr\"\n");
            exit(1);
        }
    }
    return false;
}


char* find_macro_name(char* line){
    char *line_copy = string_copy(line);
    char* macr_name_ptr = strtok(line_copy, " \t\r\n");
    char* macro_name = (char*)malloc(MAX_LEN_MACRO_NAME*sizeof(char));
    CHECK_ALLOCATION(macro_name);

    macr_name_ptr = strtok(NULL, " \t\r\n");
    if(macr_name_ptr != NULL){
        strcpy(macro_name, macr_name_ptr);
    }
    else{
        /*TODO: check if you need to not exit and return all errors*/
        printf("Missing of macro name\n");
        exit(1);
    }
    free(line_copy);
    return macro_name;
}

int check_how_many_elements_in_line(const char *line){
    char *line_copy = string_copy(line);
    int c = 0;
    char* token_ptr = strtok(line_copy, " \t\r\n");
    while (token_ptr)
    {
        c++;
        token_ptr = strtok(NULL, " \t\r\n");
    }
    free(line_copy);
    return c;
}

bool validation_check_of_macro_line(PreprocessParsedLine parsed_line){
    char* line_copy = string_copy(parsed_line.line);
    char* macr_name_ptr = strtok(line_copy, " \t\r\n");

    if (parsed_line.how_many_elements_in_line != 2){
        printf("Check validation of macro statement line and try again\n");
        free(line_copy);
        return false;
    }

    if (strncmp("macr", macr_name_ptr, 4) != 0){
        printf("Error occurred while parsing macro name\n");
        free(line_copy);
        return false;
    }
    free(line_copy);
    return true;
}

void initialize_macro_list_values(Macro** macro_list, int last_index_inserted_to_macro_list){

    int macro_list_len = last_index_inserted_to_macro_list + 1;
    *macro_list = realloc(*macro_list, (macro_list_len + 1) * sizeof(Macro));
    CHECK_ALLOCATION(*macro_list);

    (*macro_list)[last_index_inserted_to_macro_list].first_line = NULL;
    (*macro_list)[last_index_inserted_to_macro_list].last_line = NULL;
    memset((*macro_list)[last_index_inserted_to_macro_list].macro_name, 0, MAX_LEN_MACRO_NAME);
}

void insert_new_macro_to_the_macro_list(char* macro_name, Macro** macro_list, int *last_index_inserted_to_macro_list){
    *last_index_inserted_to_macro_list += 1;
    initialize_macro_list_values(macro_list, *last_index_inserted_to_macro_list);
    strcpy((*macro_list)[*last_index_inserted_to_macro_list].macro_name, macro_name);
}

int search_index_in_macr_names(char* word, Macro** macro_list, int last_index_inserted_to_macro_list){
    /*return the index of "word" the macro statement or -1 if "word" is not in macro_list*/
    int i;
    for(i = 0; i <= last_index_inserted_to_macro_list; i++){
        if(strcmp((*macro_list)[i].macro_name, word) == 0){
            return i;
            }
    }
    return -1;
}

void add_macro_to_macros_list(PreprocessParsedLine parsed_line, Macro** macro_list, int *last_index_inserted_to_macro_list){
    char* macro_name;
    if(!validation_check_of_macro_line(parsed_line)){
        printf("invalid macro line, exiting\n");
        exit(1);
    }
    macro_name = find_macro_name(parsed_line.line);
    if(search_index_in_macr_names(macro_name, macro_list, *last_index_inserted_to_macro_list) != -1){
        printf("Duplicate macro name, change the name and try again.\n");
        exit(1);
    }
    insert_new_macro_to_the_macro_list(macro_name, macro_list, last_index_inserted_to_macro_list);
    
    free(macro_name);
}

bool is_it_a_macro_call(Macro** macro_list, int last_index_inserted_to_macro_list, PreprocessParsedLine parsed_line){
    /*TODO: separate to valid macro call and check macro statement*/
    if(parsed_line.how_many_elements_in_line != 1)
        /*according to the course forum, there should not be more word after macro call */
        return false;
    if (search_index_in_macr_names(parsed_line.first_word_in_line, macro_list, last_index_inserted_to_macro_list) >= 0){
        return true;
    }
    return false;
}


void insert_macro_lines_instead_of_the_macro_call(char* macro_name, Macro** macro_list, int last_index_inserted_to_macro_list, FILE* output_file){
    Node *macr_line;
    int macro_name_index = search_index_in_macr_names(macro_name, macro_list, last_index_inserted_to_macro_list);
    if (macro_name_index < 0){
        printf("unexpected error while parsing the macro.\n");
        exit(1);
    }
    macr_line = (*macro_list)[macro_name_index].first_line;
    while (macr_line != NULL && macr_line->value != NULL)
    {
        fprintf(output_file, "%s", macr_line->value);
        macr_line = macr_line->next;
    }
}

Node *create_new_line_node(char* line){
    Node *line_node = (Node *)malloc(sizeof(Node));
    CHECK_ALLOCATION(line_node);

    line_node->value = string_copy(line);

    line_node->next = NULL;
    return line_node;
}


bool is_in_first_macro_line(Macro** macro_list, int last_index_inserted_to_macro_list){
    return (*macro_list)[last_index_inserted_to_macro_list].first_line == NULL;
}
/*TODO: remove macro_list and last index inserted, pass last macro pointer directly*/
void insert_line_into_the_latest_macro(char* line, Macro** macro_list, int last_index_inserted_to_macro_list){
        Node *line_node = create_new_line_node(line);
        if(is_in_first_macro_line(macro_list, last_index_inserted_to_macro_list)){
            /*insert the first_line pointer and the last_line pointer to point at the node*/
            (*macro_list)[last_index_inserted_to_macro_list].first_line = line_node;
            (*macro_list)[last_index_inserted_to_macro_list].last_line = line_node;
        }
        else{
            (*macro_list)[last_index_inserted_to_macro_list].last_line->next = line_node;
            (*macro_list)[last_index_inserted_to_macro_list].last_line = line_node;
        }

}

void insert_first_word_in_line_into_parsed_line_struct(const char* line, PreprocessParsedLine* parsed_line){
    char* line_copy = string_copy(line);
    char* first_word_token = strtok(line_copy, " \t\r\n");
    if(first_word_token != NULL)
        strcpy(parsed_line->first_word_in_line, first_word_token);
    free(line_copy);
}

PreprocessParsedLine parse_line(const char* line){
    PreprocessParsedLine parsed_line;
    insert_first_word_in_line_into_parsed_line_struct(line, &parsed_line);
    strcpy(parsed_line.line, line);
    parsed_line.how_many_elements_in_line = check_how_many_elements_in_line(line);
    return parsed_line;
}


LineMacroState transition_line_state(LineMacroState last_line_state, PreprocessParsedLine parsed_line, Macro** macro_list, int last_index_inserted_to_macro_list){
    if(last_line_state == INSIDE_THE_MACRO || last_line_state == MACR_CREATION_CALL){
        if (is_macro_end_statement_line(parsed_line))
            return ENDMACR_LINE;
        else
            return INSIDE_THE_MACRO;
    }
    
    if(is_macro_declaration_line(parsed_line))
        return MACR_CREATION_CALL;
    
    if(is_it_a_macro_call(macro_list, last_index_inserted_to_macro_list, parsed_line))
        return CALLED_A_MACRO;

    if (is_macro_end_statement_line(parsed_line)){
        printf("invalid line:\t%s\nfix it and try again later\n", parsed_line.line);
        exit(1);
    }
    return REGULAR_LINE;
    
}
void free_one_macro_statement(Macro* one_macro) {
    Node* next_macr;
    Node* current_macr= one_macro-> first_line;

    while (current_macr != NULL) {
        next_macr = current_macr->next;

        free(current_macr->value);
        free(current_macr);
        current_macr =next_macr;
    }
}

void free_macro_list(Macro* macro_list, size_t macro_count) {
    int i;
    for (i = 0; i < macro_count; i++) {
        free_one_macro_statement(&macro_list[i]);
    }
    free(macro_list);
}

bool check_if_file_opened_successfully(FILE *file){
    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        return false;
    }
    return true;
}


bool parse_file_with_macros(const char *input_file_name){
    bool result = false;
    FILE *input_file = fopen(input_file_name, "r");
    FILE *output_file;
    LineMacroState line_state = REGULAR_LINE;
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];

    int last_index_inserted_to_macro_list = -1;
    Macro* macro_list = NULL;
    char *output_file_name = change_file_extention(input_file_name, FILE_EXTENTION_PREPROCESSOR);
    /*char *output_file_name = (char *)malloc(strlen(input_file_name) + 4);
    CHECK_ALLOCATION(output_file_name);
    strcpy(output_file_name, input_file_name);
    strcat(output_file_name, ".am");*/
    
    output_file = fopen(output_file_name, "w");

    if(!check_if_file_opened_successfully(input_file) || !check_if_file_opened_successfully(output_file)){
        result = false;
        goto cleanup;}
    

    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, input_file) != NULL){
        PreprocessParsedLine parsed_line = parse_line(line);
        /** TODO: simplify last macro usage, pass around a pointer to the last macro only */
        line_state = transition_line_state(line_state, parsed_line, &macro_list, last_index_inserted_to_macro_list);
        switch (line_state){
            case REGULAR_LINE:
                fprintf(output_file, "%s", line);
                break;
            case MACR_CREATION_CALL:
                add_macro_to_macros_list(parsed_line, &macro_list, &last_index_inserted_to_macro_list);
                break;
            case INSIDE_THE_MACRO:
                insert_line_into_the_latest_macro(line, &macro_list, last_index_inserted_to_macro_list);
                break;
            case ENDMACR_LINE:
                break;
            case CALLED_A_MACRO:
                insert_macro_lines_instead_of_the_macro_call(parsed_line.first_word_in_line, &macro_list, last_index_inserted_to_macro_list, output_file);
                break;
            default:
                break;
        }
    }
    result = true; 
cleanup: 
    fclose(input_file);
    fclose(output_file);
    free_macro_list(macro_list, last_index_inserted_to_macro_list + 1);
    free(output_file_name);
    return result;
}

bool preprocess_macro(const char *input_file_name){
    /* The main function of the preprocessor, used in main.c
    error_flag will change to "true" if there are errors in the input*/
    /*TODO: handle the error flag instead of the exit(1)*/
    bool ans = parse_file_with_macros(input_file_name);
    return ans;
}