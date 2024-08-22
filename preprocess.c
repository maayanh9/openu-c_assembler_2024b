#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"

typedef enum {
    SEARCHING_FOR_MACRO_CREATION,
    INSIDE_MACRO,
    END_MACRO,
    CALLED_MACRO,
    OTHER
} line_macro_state;


char* string_copy(const char* str_input) {
    char* str_copy = (char *)malloc(strlen(str_input) + 1);
    CHECK_ALLOCATION(str_copy);
    strcpy(str_copy, str_input);
    return str_copy;
}

bool is_it_a_macro_statement_line(char *line){
    /*TODO: see edge cases for macr string*/
    /*TODO: use first line parameter instead*/
    const char *macro_call = "macr";
    while (IS_SPACE_OR_TAB(*line)){
        line++;
    }
    if(strncmp(line, macro_call, 4) == 0){
        if(IS_SPACE_OR_TAB(line[4])){
            return true;
            /*TODO: make sure the line[4] doesn't do problems.
            I dont think it should*/
        }
    }
    
    return false;
}


char* find_macro_name(char* line){
    char *line_copy = string_copy(line);
    char* macr_name_ptr = strtok(line_copy, " \t\r\n");
    char* macro_name = (char*)malloc(MAX_LEN_MACRO_NAME*sizeof(char));

    macr_name_ptr = strtok(NULL, " \t\r\n");
    if(macr_name_ptr != NULL){
        strncpy(macro_name, macr_name_ptr, MAX_LEN_MACRO_NAME);
    }
    else{
        /*TODO: check if you need to not exit and return all errors*/
        printf("Missing of macro name\n");
        exit(1);
    }
    free(line_copy);
    return macro_name;
}

int check_how_many_elements_in_line(char *line){
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

bool validation_check_of_macro_line(char* line){
    char* line_copy = string_copy(line);
    int num_of_elem_in_line = check_how_many_elements_in_line(line_copy);
    char* macr_name_ptr = strtok(line_copy, " \t\r\n");

    if (num_of_elem_in_line != 2){
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

void initialize_macro_list_values(macro** macro_list, size_t macro_counter){
    *macro_list = realloc(*macro_list, (macro_counter + 1) * sizeof(macro));
    CHECK_ALLOCATION(*macro_list);

    (*macro_list)[macro_counter].first_line = malloc(sizeof(node));
    CHECK_ALLOCATION((*macro_list)[macro_counter].first_line);
    (*macro_list)[macro_counter].first_line->value = NULL;
    (*macro_list)[macro_counter].first_line->next = NULL;

    (*macro_list)[macro_counter].last_line = malloc(sizeof(node));
    CHECK_ALLOCATION((*macro_list)[macro_counter].last_line);
    (*macro_list)[macro_counter].last_line->value = NULL;
    (*macro_list)[macro_counter].last_line->next = NULL;
}

bool insert_new_macro_to_the_macro_list(char* macro_name, macro** macro_list, size_t macro_counter){
    initialize_macro_list_values(macro_list, macro_counter);
    (*macro_list)[macro_counter].macro_name = string_copy(macro_name);
    return true;
}

int search_index_in_macr_names(char* word, macro** macro_list, size_t macro_counter){
    /*return the index of "word" the macro statement or -1 if "word" is not in macro_list*/
    int i;
    for(i = 0; i < macro_counter; i++){
        if(strcmp((*macro_list)[i].macro_name, word) == 0){
            return i;
            }
    }
    return -1;
}

bool check_validation_macro_line_add_macro_to_macros_list(char *line, macro** macro_list, size_t macro_counter){
    char* macro_name;
    if(!validation_check_of_macro_line(line)){
        printf("invalid macro line, exiting\n");
        return false;
    }
    macro_name = find_macro_name(line);
    if(search_index_in_macr_names(macro_name, macro_list, macro_counter) != -1){
        printf("Duplicate macro name, change the name and try again.\n");
        return false;
    }
    if(insert_new_macro_to_the_macro_list(macro_name, macro_list, macro_counter)){
        free(macro_name);
        return true;
    }
    
    free(macro_name);
    return false;
}

bool is_it_a_macro_call(char* line, char* first_word_in_line, macro** macro_list, size_t macro_counter){
    /*TODO: separate to valid macro call and check macro statement*/
    if(check_how_many_elements_in_line(line) != 1)
        return false;
    if (search_index_in_macr_names(first_word_in_line, macro_list, macro_counter) >= 0){
        return true;
    }
    return false;
}


char* get_first_word_in_line(char* line){
    char* line_copy = string_copy(line);
    char* first_word_token = strtok(line_copy, " \t\r\n");
    char* first_word = string_copy(first_word_token);
    free(line_copy);
    return first_word;
}

void insert_macro_lines_instead_of_the_macro_call(char* macro_name, macro** macro_list, size_t macro_counter){
    int macro_name_index = search_index_in_macr_names(macro_name, macro_list, macro_counter);
    if (macro_name_index < 0){
        printf("unexpected error while parsing the macro.\n");
    }
    node *macr_line = macro_list[macro_name_index]->first_line;
    while (macr_line->value != NULL)
    {
        printf("%s", macr_line->value);
        macr_line = macr_line->next;
    }
}

node *create_new_line_node(){
    node *line_node = (node *)malloc(sizeof(node));
    CHECK_ALLOCATION(line_node);
    line_node->value = (char *)malloc(MAX_LEN_LINE_ASSEMBLY_FILE);
    CHECK_ALLOCATION(line_node->value);
    line_node->next = NULL;
    return line_node;
}

void insert_the_first_line(char* line, macro** macro_list, size_t macro_counter){
    
    first_line_allocate_memory(macro_list, macro_counter);
    last_line_allocate_memory(macro_list, macro_counter);
    macro_list[macro_counter]->first_line->value = string_copy(line);
    macro_list[macro_counter]->last_line->value = string_copy(line);
}
bool we_are_in_the_first_macro_line(macro** macro_list, size_t macro_counter){
    return macro_list[macro_counter]->first_line->value == NULL;
}
void insert_line_into_the_macro_list(char* line, macro** macro_list, size_t macro_counter){
        if(we_are_in_the_first_macro_line(macro_list, macro_counter)){
            insert_the_first_line(line, macro_list, macro_counter);
        }

        node *macr_line = ;
        /*todo after sport, i need to save pointer to the last line (tail) and insert lines from there*/

}

bool parse_file_with_macros(const char *filename){
    FILE *file = fopen(filename, "r");
    line_macro_state line_state;
    char line[MAX_LEN_LINE_ASSEMBLY_FILE];
    char* first_word_in_line = (char*)malloc(MAX_LEN_MACRO_NAME*sizeof(char));

    size_t macro_counter = 0;
    size_t allocated_memory_macro_list = 2;
    macro* macro_list = (macro*)calloc(allocated_memory_macro_list, sizeof(macro));
    CHECK_ALLOCATION(macro_list);
    /*TODO: add more space to macro list using realloc */

    if(file == NULL){
        /*Error opening file*/
        perror("Error: ");
        return false;
    }

    line_state = OTHER;
    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, file) != NULL){
        first_word_in_line = get_first_word_in_line(line);
        switch (line_state){
            case OTHER:
                if (is_it_a_macro_statement_line(line)){
                    if (check_validation_macro_line_add_macro_to_macros_list(line, &macro_list, macro_counter)){
                        macro_counter++;
                        line_state = INSIDE_MACRO;}
                    else{
                        printf("Error occurred while parsing the macro");
                        return false;
                    }
                }
                if (is_it_a_macro_call(line, first_word_in_line, &macro_list, macro_counter)){
                    
                    line_state = CALLED_MACRO;

                }
                break;
            case INSIDE_MACRO:
                printf("%s", line);
                line_state = END_MACRO;
                break;
            case END_MACRO:
                printf("%s", line);
                line_state = OTHER;
                break;
            case CALLED_MACRO:
                line_state = OTHER;
                break;
            default:
                break;
            /*case
            default:
                printf("error in cases");
                break;*/
        }
    }
    
    fclose(file);
    return true;
}

bool preprocess_macro(const char *input_file_name){
    bool ans = parse_file_with_macros(input_file_name);
    return ans;
}