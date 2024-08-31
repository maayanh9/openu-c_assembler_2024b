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

void add_next_word(SeparateLineIntoWords* separate_line, char* token_ptr) {
    char* new_word = string_copy(token_ptr);
    separate_line->words[separate_line->words_counter] = new_word;
    separate_line->words_counter++;
}

void handle_parsing_string_directive(SeparateLineIntoWords* separate_line, char* token_ptr){
    char* end_word_ptr;
    add_next_word(separate_line, token_ptr);
    token_ptr = strtok(NULL, ""); /*the whole text after .string*/

    /* move the ptr forward if there are spaces ot tabs*/
    if (token_ptr){
        while (*token_ptr == ' ' || *token_ptr == '\t'){
            token_ptr++;
        }
    }
    /* replace the last characters to \0 if they are spaces, tabs or \n
    used to make sure the last word in .string ends without those characters*/
    end_word_ptr = token_ptr + strlen(token_ptr) - 1;
    while (end_word_ptr > token_ptr && (*end_word_ptr == ' ' || *end_word_ptr == '\t' || *end_word_ptr == '\n' || *end_word_ptr == '\r'))
    {
        *end_word_ptr = '\0';
        end_word_ptr --;
    }
    add_next_word(separate_line, token_ptr);
}

SeparateLineIntoWords separate_line_into_words(char *line){
    SeparateLineIntoWords separate_line;
    char *line_copy = string_copy(line);
    char* token_ptr = strtok(line_copy, " \t\r\n");

    separate_line.words_counter = 0;
    
    while (token_ptr){   
        if (strcmp(token_ptr, ".string") == 0){
            handle_parsing_string_directive(&separate_line, token_ptr);
            break;
        }
        else {
            add_next_word(&separate_line, token_ptr);
        }
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

int is_a_command_and_which(char* potential_command){
    int i;
    for(i = 0; i < LEN_OF_COMMANDS_LIST; i ++){
        if(strcmp(instructions_commands_and_addressing[i][0], potential_command) == 0){
            return i;
        }
    }
    return -1;
}

int which_directive(char* potential_directive){
    /* return the directive num from AssemblyDirective enum if it is a valid directive or -1 if it is not there*/
    /*directive is the string after the '.' */
    char * directive = potential_directive + sizeof(char);
    int i;
    for(i = 0; i < LEN_OF_DIRECTIVE_LIST; i++){
        if(strcmp(directives_list[i], directive) == 0){
            return i;
        }
    }
    return -1;
}

bool is_the_last_word_in_this_line(int parsed_words_counter, int how_many_words_in_line){
    return (parsed_words_counter == how_many_words_in_line);
}

void error_line(ParsedLine* parsed_line, char* type, char* invalid_data, DynamicList *errors_ptrs){
    /*type: directive, label or command*/
    int line_counter = parsed_line->mete_data.line_counter;
    parsed_line->line_type = ERROR_LINE;

    sprintf(parsed_line->LineTypes.error_str, "Invalid %s: %s, at line %d.\n", type, invalid_data, line_counter);

    insert_new_cell_into_dynamic_list(errors_ptrs, parsed_line);
    printf("%s\t**********", parsed_line->LineTypes.error_str);
    
}
bool is_valid_directive_call(int directive_num, int parsed_words_ctr, int how_many_words_in_line){
    return (directive_num != -1 && !is_the_last_word_in_this_line(parsed_words_ctr, how_many_words_in_line));
}
bool starts_with_comma(char* data_parameters){
    if(strlen(data_parameters) == 0)
        return false;
    return data_parameters[0] == ',';
}

bool ends_with_comma(char* data_parameters){
    if(strlen(data_parameters) == 0)
        return false;
    return data_parameters[strlen(data_parameters) - 1] == ',';
}
bool has_only_numbers_and_commas(char* data_parameters){
    char* ptr = data_parameters;
    while(*ptr){
        if(!isdigit(*ptr) && *ptr != ',' && *ptr != '+' && *ptr != '-'){
            return false;
        }
        ptr++;
    }
    return true;
}
bool have_sequence_of_commas(char* data_parameters){
    char* ptr = data_parameters;
    int comma_in_a_row_counter = 0;
    while(*ptr){
        if(*ptr == ','){
            comma_in_a_row_counter ++;
            if (comma_in_a_row_counter >= 2){
                return true;
            }
        }
        else {
            comma_in_a_row_counter = 0;
        }
        ptr++;
    }
    return false;
}
bool have_plus_or_minus_only_after_comma(char* data_parameters){
    char* ptr = data_parameters;
    bool comma_flag = true;
    while(*ptr){
        if(*ptr == ','){
            comma_flag = true;
        }
        else if(*ptr == '+' || *ptr == '-'){
            if(!comma_flag){
                return false;
            }
            comma_flag = false;
        }
        else{
            comma_flag = false;
        }
        ptr++;
    }
    return true;
}
bool valid_data_num_parameters(char* data_parameters){
    return !ends_with_comma(data_parameters) && !starts_with_comma(data_parameters) && has_only_numbers_and_commas(data_parameters) 
    && !have_sequence_of_commas(data_parameters) && have_plus_or_minus_only_after_comma(data_parameters);
}

char* connect_unparsed_separate_strings(SeparateLineIntoWords separated_words, int parsed_words_ctr){
    /*Connect again the separate words easier parse*/
    char* connected_string = (char*)malloc(sizeof(char) * (MAX_LEN_LINE_ASSEMBLY_FILE));
    int connected_strings_counter = parsed_words_ctr;
    *connected_string = '\0';
    while (separated_words.words_counter > connected_strings_counter){
        strcat(connected_string, separated_words.words[connected_strings_counter]);
        connected_strings_counter ++;
    }
    return connected_string;
}

bool insert_data_numbers_into_list(ParsedLine* parsed_line, char* data_parameters){
    char* data_numbers_token = strtok(data_parameters, ",+");
    int next_num;
    int array_num_counter = 0;
    parsed_line->LineTypes.Directive.DirectiveTypes.DirectiveData.num_of_elements = 0;
    while (data_numbers_token){
        next_num = atoi(data_numbers_token);
        printf("%d\t", next_num);
        parsed_line->LineTypes.Directive.DirectiveTypes.DirectiveData.data_numbers[array_num_counter] = next_num;
        parsed_line->LineTypes.Directive.DirectiveTypes.DirectiveData.num_of_elements++;
        array_num_counter++;
        data_numbers_token = strtok(NULL, ",");
        if(array_num_counter >= MAX_NUMBERS_IN_DATA_LABEL){
            return false;
        }
    }
    return true;
}

bool insert_data_directive_into_parsed_line_or_error(ParsedLine* parsed_line, int* parsed_words_ctr, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs){
    /* parse the .data parameters (check if its valid) 
    and insert it into the parsed_line struct*/
    char *data_parameters = connect_unparsed_separate_strings(separated_words, *parsed_words_ctr);
    bool answer = true;
    if(valid_data_num_parameters(data_parameters)){
        if(insert_data_numbers_into_list(parsed_line, data_parameters)){
            parsed_line->mete_data.data_counter += parsed_line->LineTypes.Directive.DirectiveTypes.DirectiveData.num_of_elements;
            parsed_line->mete_data.instruction_counter += parsed_line->LineTypes.Directive.DirectiveTypes.DirectiveData.num_of_elements;
        }
        else{
            error_line(parsed_line, ".data directive (too many integers)", data_parameters, errors_ptrs);
            answer = false;
        }
    }
    else{
        error_line(parsed_line, ".data directive", data_parameters, errors_ptrs);
        answer = false;
    }

    free(data_parameters);
    return answer;
}

bool valid_string_parameter(char* string_context){
    return (strlen(string_context) > 1 && string_context[0] == '\"' && string_context[strlen(string_context) - 1] == '\"');
}

bool insert_string_directive_into_parsed_line_or_error(ParsedLine* parsed_line, int* parsed_words_ctr, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs, char* line){
    if(valid_string_parameter(separated_words.words[*parsed_words_ctr])){
        char* ascii_string = separated_words.words[*parsed_words_ctr] + 1; /* the ascii string without the " at the beginning*/
        ascii_string[strlen(ascii_string) - 1] = '\0'; /* cut the " at the end of the string*/
        strcpy(parsed_line->LineTypes.Directive.DirectiveTypes.ascii_string, ascii_string);
        parsed_line->mete_data.instruction_counter += strlen(parsed_line->LineTypes.Directive.DirectiveTypes.ascii_string) + 1; /* +1 for the \0 */
    }
    else{
        error_line(parsed_line, ".string directive", separated_words.words[*parsed_words_ctr], errors_ptrs);
        return false;
    }
    return true;
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
    if(!string_contains_only_letters_and_numbers(label) || is_a_dup_label_name(label, symbols_table) || is_the_last_word_in_this_line(parsed_words_ctr, how_many_words_in_line) || strlen(label) > MAX_LEN_OF_LABEL){
        return false;
    }
    return true;
}

bool a_duplicate(char* label, DynamicList entry_or_extern_ptrs){
    int i;
    for(i = 0; i < entry_or_extern_ptrs.list_length; i++){
        ParsedLine* parsed_line = (ParsedLine*)entry_or_extern_ptrs.items[i];
        
        if(strcmp(label, parsed_line->LineTypes.Directive.DirectiveTypes.entry_or_extern) == 0){
            return true;
        }
    }
    return false;
}

bool duplicate_label_of_extern_or_entry(char* label, DynamicList entry_ptrs, DynamicList external_ptrs){
    return a_duplicate(label, entry_ptrs) || a_duplicate(label, external_ptrs);
}

bool valid_entry_or_extern_parameter(ParsedLine* parsed_line, DynamicList symbols_table, int parsed_words_ctr, SeparateLineIntoWords separated_words, DynamicList entry_ptrs, DynamicList external_ptrs){
    if(parsed_line->has_label == HAS_LABEL){ /* ignore label for .extern and .entry */
        if(is_valid_label(parsed_line->label, symbols_table, parsed_words_ctr, separated_words.words_counter))
            printf("NOTE: in line %d, the label: %s has no meaning.\n", parsed_line->mete_data.line_counter, parsed_line->label);
        else{
            printf("NOTE: in line %d, the label: %s has no meaning.\nIn addition, this label is also invalid label.\n", parsed_line->mete_data.line_counter, parsed_line->label);
        }
    }
    return !is_the_last_word_in_this_line(parsed_words_ctr, separated_words.words_counter) && (string_contains_only_letters_and_numbers(separated_words.words[parsed_words_ctr]))
                && !duplicate_label_of_extern_or_entry(separated_words.words[parsed_words_ctr], entry_ptrs, external_ptrs);
}

bool insert_entry_or_extern_directive_into_parsed_line_or_error(ParsedLine* parsed_line, int* parsed_words_ctr, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs, DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs){
    if(valid_entry_or_extern_parameter(parsed_line, symbols_table, *parsed_words_ctr, separated_words, entry_ptrs, external_ptrs)){
        strcpy(parsed_line->LineTypes.Directive.DirectiveTypes.entry_or_extern, separated_words.words[*parsed_words_ctr]);
        return true;
    }
    else{
        if(parsed_line->LineTypes.Directive.directive_type == ENTRY)
            error_line(parsed_line, ".entry directive", separated_words.words[*parsed_words_ctr], errors_ptrs);
        else
            error_line(parsed_line, ".extern directive", separated_words.words[*parsed_words_ctr], errors_ptrs);
        return false;
    }
}

bool insert_directive_parameters(ParsedLine* parsed_line, int* parsed_words_ctr, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs, DynamicList *entry_ptrs, DynamicList *external_ptrs, DynamicList symbols_table){
    AssemblyDirective directive_type = parsed_line->LineTypes.Directive.directive_type;
    bool answer = true;
    switch (directive_type)
    {
    case DATA:
        answer = insert_data_directive_into_parsed_line_or_error(parsed_line, parsed_words_ctr, line_counter, separated_words, errors_ptrs);
        break;
    case STRING:
        answer = insert_string_directive_into_parsed_line_or_error(parsed_line, parsed_words_ctr, line_counter, separated_words, errors_ptrs, separated_words.words[*parsed_words_ctr]);
        break;
    case EXTERN:
        answer = insert_entry_or_extern_directive_into_parsed_line_or_error(parsed_line, parsed_words_ctr, separated_words, errors_ptrs, symbols_table, *entry_ptrs, *external_ptrs);
        if(answer){
            insert_new_cell_into_dynamic_list(external_ptrs, parsed_line);
        }
        break;
    case ENTRY:
        answer = insert_entry_or_extern_directive_into_parsed_line_or_error(parsed_line, parsed_words_ctr, separated_words, errors_ptrs, symbols_table, *entry_ptrs, *external_ptrs);
        if (answer){
            insert_new_cell_into_dynamic_list(entry_ptrs, parsed_line);
        }
        break;
    
    default:
    /*for input problems*/
        return false;
    }
    return answer;
}

bool check_validation_and_insert_directive_parameters(ParsedLine* parsed_line, int* parsed_words_ctr, DynamicList *symbols_table, int line_counter, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs, DynamicList *entry_ptrs, DynamicList *external_ptrs){
    int directive_num = which_directive(separated_words.words[*parsed_words_ctr]);
    parsed_line->line_type = DIRECTIVE_LINE;
    if(is_valid_directive_call(directive_num, *parsed_words_ctr, separated_words.words_counter)){
        parsed_line->LineTypes.Directive.directive_type = directive_num;
        (*parsed_words_ctr) ++;
        if(insert_directive_parameters(parsed_line, parsed_words_ctr, line_counter, separated_words, errors_ptrs, entry_ptrs, external_ptrs, *symbols_table))
            return true;
        else{
            return false;
        }
    }
    else{
        error_line(parsed_line, "directive", separated_words.words[*parsed_words_ctr], errors_ptrs);
        return false;
    }
}

bool ignore_label(char* the_word_after_label){
    return (strcmp(the_word_after_label, ".extern") == 0 || strcmp(the_word_after_label, ".entry") == 0);
}


bool check_validation_and_insert_label_data(ParsedLine* parsed_line, int* parsed_words_ctr, SeparateLineIntoWords separated_words, DynamicList *symbols_table, int line_counter, int words_in_line_counter, DynamicList *errors_ptrs){
    
    /*Copy the label without the ':' */
    char* label = string_copy(separated_words.words[0]);
    label[strlen(label) - 1] = '\0';

    parsed_line->has_label = HAS_LABEL;
    
    if(is_valid_label(label, *symbols_table, *parsed_words_ctr, words_in_line_counter) || (separated_words.words_counter >=2 && ignore_label(separated_words.words[1]))){
        strcpy(parsed_line->label, label);
        (*parsed_words_ctr) ++;
        insert_new_cell_into_dynamic_list(symbols_table, parsed_line);
        free(label);
        return true;
    }
    else{
        if(is_the_last_word_in_this_line(*parsed_words_ctr, separated_words.words_counter))
            error_line(parsed_line, "label", "no parameters after label", errors_ptrs);
        else
            error_line(parsed_line, "label", label, errors_ptrs);
        free(label);
        return false;
    }
    
}
bool more_than_one_comma(char* parameters){
    char* ptr = parameters;
    int comma_ctr = 0;
    while(*ptr){
        if(*ptr == ','){
            comma_ctr ++;
            if(comma_ctr >= 2)
                return false;
        }
        ptr++;
    }
    return true;
}
SeparateLineIntoWords instruction_parameters(char* parameters){
    SeparateLineIntoWords parsed_parameters;
    char* parameters_copy = string_copy(parameters);
    char* parameters_token;

    parsed_parameters.words_counter = 0;
    
    parameters_token = strtok(parameters_copy, ",");
    while(parameters_token != NULL){
        strcpy(parsed_parameters.words[parsed_parameters.words_counter], parameters_token);
        parsed_parameters.words_counter ++;
        parameters_token = strtok(NULL, ",");
    }
    free(parameters_copy);
    return parsed_parameters;
}

bool first_check_valid_parameters_or_error_line(ParsedLine* parsed_line, char* parameters, DynamicList *errors_ptrs){

    if(ends_with_comma(parameters)){
        error_line(parsed_line, "instruction", "ends with comma", errors_ptrs);
        return false;
    }
    else if (starts_with_comma(parameters)){
        error_line(parsed_line, "instruction", "starts with comma", errors_ptrs);
        return false;
    }
    else if (have_sequence_of_commas(parameters)){
        error_line(parsed_line, "instruction", "have a sequence of commas", errors_ptrs);
        return false;
    }
    else if (more_than_one_comma(parameters)){
        error_line(parsed_line, "instruction", "too many parameters", errors_ptrs);
        return false;
    }
    
    return true;
}


bool is_register(char* parameter, char** error_note){
    char* ptr = parameter;
    int i;
    if(*ptr != 'r')
        return false;
    ptr ++;
    if(isdigit(ptr)){
        for(i = 0; i < NUM_OF_REGISTERS; i++){
            if(i == atoi(ptr))
                return true;
        }
        *error_note = "invalid register name";
    }
    return false;
}
bool is_direct_register(char* parameter, char** error_note){
    return is_register(parameter, error_note);
}
bool is_indirect_register(char* parameter, char** error_note){
    char* ptr = parameter;
    if(*ptr != '*')
        return false;
    ptr ++;
    return is_register(ptr, error_note);
}

bool is_direct(char* parameter){
    return isalnum(parameter);
}

bool is_immediate(char* parameter){
    char* ptr = parameter;
    if(*ptr != '#')
        return false;
    ptr ++;
    return isdigit(ptr);
}

int get_addressing_methods(char* parameter, char** error_note){
    /* return the num of addressing method or -1 if its invalid parameter */
    int addressing_method = -1;
    if(is_immediate(parameter)){
        addressing_method = IMMEDIATE;
    }
    else if (is_direct(parameter)){
        addressing_method = DIRECT;
    }
    else if (is_direct_register(parameter, error_note)){
        addressing_method = DIRECT_REGISTER;
    }
    else if (is_indirect_register(parameter, error_note)){
        addressing_method = INDIRECT_REGISTER;
    }
    return addressing_method;
}

bool valid_num_of_parameters(int parameters_in_line, int how_many_parameters_needed){
    return (parameters_in_line == how_many_parameters_needed);
}

typedef enum{
    SOURCE = 1,
    DESTINATION
}SourceOrDest;

bool valid_addressing_per_command(AssemblyCommands command, SourceOrDest src_or_dest, AddressingMethod addressing_method, char** error_note){
    const char* valid_addressing_ptr = instructions_commands_and_addressing[command][src_or_dest];
    while (*valid_addressing_ptr){
        if(atoi(valid_addressing_ptr) == addressing_method){
            return true;
        }
        valid_addressing_ptr ++;
    }
    if(src_or_dest == SOURCE){
        *error_note = "invalid addressing method at SRC";
    }
    else{
        *error_note = "invalid addressing method at DST";
    }
    return false;
}

bool is_valid_addressing_methods(char* parameters, AssemblyCommands command, char** error_note, int* src, int* dst){
    /* change also the addressing methods (the values in src and dst):
                    initialize them to -2 (will stay -2 only if there is no use of them) 
                    if they are in use, the function change them:
                        for valid addressing method - the number of the addressing method
                        for invalid methods -1
        and the error note if the parameters are invalid*/

    SeparateLineIntoWords parsed_instruction_parameters = instruction_parameters(parameters);
    int how_many_parameters_needed;

    *src = -2;
    *dst = -2;

    /* the number of parameters for a valid command */
    how_many_parameters_needed = atoi(instructions_commands_and_addressing[command][3]);
    if(!valid_num_of_parameters(parsed_instruction_parameters.words_counter, how_many_parameters_needed)){
        *error_note = "instruction with invalid number of parameters";
        return false;
    }
    if(how_many_parameters_needed == 0){
        return true;
    }
    else if (how_many_parameters_needed == 1){
        *src = get_addressing_methods(parsed_instruction_parameters.words[0], error_note);
        return (*src != -1) && valid_addressing_per_command(command, SOURCE, *src, error_note);
    }
    else if (how_many_parameters_needed == 2){
        *src = get_addressing_methods(parsed_instruction_parameters.words[0], error_note);
        *dst = get_addressing_methods(parsed_instruction_parameters.words[1], error_note);
        return (*src != -1) && (*dst != -1) && valid_addressing_per_command(command, SOURCE, *src, error_note)
                                            && valid_addressing_per_command(command, DESTINATION, *dst, error_note);
    }
    return false;
}

bool check_validation_and_insert_instruction_parameters(ParsedLine* parsed_line, int* parsed_words_ctr, DynamicList *symbols_table, SeparateLineIntoWords separated_words, DynamicList *errors_ptrs){
    char* instruction_parameters_in_one_word;
    char *error_note = NULL;
    int source_parameter;
    int destination_parameter;

    int command_num = is_a_command_and_which(separated_words.words[*parsed_words_ctr]);
    parsed_line->LineTypes.Instruction.command = command_num;
    
    instruction_parameters_in_one_word = connect_unparsed_separate_strings(separated_words, *parsed_words_ctr);

    if(!first_check_valid_parameters_or_error_line(parsed_line, instruction_parameters_in_one_word, errors_ptrs)){
        free(instruction_parameters_in_one_word);
        return false;
    }    

    if(!is_valid_addressing_methods(instruction_parameters_in_one_word, command_num, &error_note, &source_parameter, &destination_parameter)){
        if(error_note != NULL){
            error_line(parsed_line, "instuction", error_note, errors_ptrs);
        }
        else{
            error_line(parsed_line, "instuction", instruction_parameters_in_one_word, errors_ptrs);
        }
        free(instruction_parameters_in_one_word);
        return false;
    }

    free(instruction_parameters_in_one_word);
    return true;
}


ParsedLine* parse_line(char* line, LineMetaData *counters, DynamicList *symbols_table, DynamicList *errors_ptrs, DynamicList *entry_ptrs, DynamicList *external_ptrs, ParsedLine *parsed_line){
    SeparateLineIntoWords separated_words = separate_line_into_words(line);
    int parsed_words_ctr = 0;
    parsed_line->mete_data.instruction_counter = counters->instruction_counter;
    parsed_line->mete_data.line_counter = counters->line_counter;
    if (is_comment_or_empty_line(separated_words)){
        parsed_line->line_type = EMPTY_OR_COMMENT_LINE;
        goto finished_parsing_line;
    }

    if (has_a_label(separated_words)){
        if(!check_validation_and_insert_label_data(parsed_line, &parsed_words_ctr, separated_words, symbols_table, counters->line_counter, separated_words.words_counter, errors_ptrs)){
            goto finished_parsing_line;
        }
    }

    if(is_a_directive(separated_words.words[parsed_words_ctr])){
        if(!check_validation_and_insert_directive_parameters(parsed_line, &parsed_words_ctr, symbols_table, counters->line_counter, separated_words, errors_ptrs, entry_ptrs, external_ptrs)){
            goto finished_parsing_line;
        }
    }
    /*command or invalid data*/
    else{
        if(is_a_command_and_which(separated_words.words[parsed_words_ctr]) != -1){
            check_validation_and_insert_instruction_parameters(parsed_line, &parsed_words_ctr, symbols_table, separated_words, errors_ptrs);
        }
        else{
            error_line(parsed_line, "line", line, errors_ptrs);
        }
    }
    

finished_parsing_line:
    counters->instruction_counter = parsed_line->mete_data.instruction_counter;
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
    DynamicList entry_ptrs;
    DynamicList external_ptrs;
    initialize_dynamic_list(&parsed_lines_list, sizeof(ParsedLine));
    initialize_dynamic_list(&symbols_table, sizeof(ParsedLine *));
    initialize_dynamic_list(&errors_ptrs, sizeof(ParsedLine *));
    initialize_dynamic_list(&entry_ptrs, sizeof(ParsedLine *));
    initialize_dynamic_list(&external_ptrs, sizeof(ParsedLine *));

    counters.instruction_counter = 100;
    counters.line_counter = 1;

    if(!check_if_file_opened_successfully(input_file)){
        result = false;
        goto cleanup;}
    

    while (fgets(line, MAX_LEN_LINE_ASSEMBLY_FILE, input_file) != NULL){
        ParsedLine *parsed_line = malloc(sizeof(ParsedLine));
        CHECK_ALLOCATION(parsed_line);
        parse_line(line, &counters, &symbols_table, &errors_ptrs, &entry_ptrs, &external_ptrs, parsed_line);
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
    free_dynamic_list(&external_ptrs);
    free_dynamic_list(&entry_ptrs);
    return result;
}

bool assembler(const char *input_file_name){
    bool ans = first_pass(input_file_name);
    return ans;
}

