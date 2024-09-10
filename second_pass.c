#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "second_pass.h"
#include "settings.h"
#include "dynamic_list.h"
#include "first_pass.h"
#include "text_and_digits_handler.h"


typedef enum Table {
    SYMBOLS_TABLE,
    ENTRY_TABLE,
    EXTERN_TABLE
} Table;

char* get_entry_or_extern_label(ParsedLine *parsed_entry_line) {
    return parsed_entry_line->LineTypes.Directive.DirectiveTypes.entry_or_extern;
}
char *get_symbols_table_label(ParsedLine *line_with_label) {
    return line_with_label->label;
}

bool label_equals_this_cell(ParsedLine *line_from_table, Table which_table, char* label) {
    if(which_table == SYMBOLS_TABLE) {
        return strcmp(label, get_symbols_table_label(line_from_table)) == 0;
    }
    return strcmp(label, get_entry_or_extern_label(line_from_table)) == 0;
}

int get_extern_instruction_counter(int instruction_counter, int how_many_parameters_for_command, SourceOrDest src_or_dst) {

    /*The first instruction line is for the command*/
    instruction_counter ++;
    if(how_many_parameters_for_command == 2 && src_or_dst == DESTINATION)
        instruction_counter ++;
    return instruction_counter;
}

int find_in_table(DynamicList table, char* element_to_find, Table which_table) {
    /*return the found index or -1 if the index is not found*/
    int i;
    for(i = 0; i< table.list_length; i++) {
        ParsedLine *table_cell = (ParsedLine *)table.items[i];
        if(label_equals_this_cell(table_cell, which_table, element_to_find)) {
            return i;
        }
    }
    return -1;
}

bool is_a_direct_addressing(InstructionParameter src_or_dst) {
    if(src_or_dst.addressing_method == DIRECT) {
        return  true;
    }
    return false;
}

bool is_dest_a_direct_addressing(ParsedLine direct_line) {
    return is_a_direct_addressing(direct_line.LineTypes.Instruction.dest);
}

bool is_source_a_direct_addressing(ParsedLine direct_line) {
    return is_a_direct_addressing(direct_line.LineTypes.Instruction.source);
}

bool find_instruction_label_in_tables(DynamicList symbols_table, DynamicList external_ptrs,
                   InstructionParameter direct_src_or_dest, int *symbol_address, int instruction_counter, int how_many_parameters_for_command, SourceOrDest src_or_dst, bool * found_at_extern_table) {
    int ans = find_in_table(symbols_table, direct_src_or_dest.Addressing.Direct.direct, SYMBOLS_TABLE);
    if(ans != -1) {
        *symbol_address = ((ParsedLine *)symbols_table.items[ans])->mete_data.instruction_counter;
        return true;
    }
    ans = find_in_table(external_ptrs, direct_src_or_dest.Addressing.Direct.direct, EXTERN_TABLE);
    if(ans != -1) {
        *found_at_extern_table = true;
        *symbol_address = get_extern_instruction_counter(instruction_counter, how_many_parameters_for_command, src_or_dst);
        return true;
    }
    return false;
}

bool find_direct_label_in_tables(DynamicList symbols_table, DynamicList external_ptrs, ParsedLine direct_line, SourceOrDest src_or_dest, int *symbol_address, bool * found_at_extern_table) {
    switch (src_or_dest) {
        case SOURCE:
            return find_instruction_label_in_tables(symbols_table, external_ptrs, direct_line.LineTypes.Instruction.source, symbol_address, direct_line.mete_data.instruction_counter, atoi(instructions_commands_and_addressing[direct_line.LineTypes.Instruction.command][3]), SOURCE, found_at_extern_table);
        case DESTINATION:
            return find_instruction_label_in_tables(symbols_table, external_ptrs, direct_line.LineTypes.Instruction.dest, symbol_address, direct_line.mete_data.instruction_counter, atoi(instructions_commands_and_addressing[direct_line.LineTypes.Instruction.command][3]), DESTINATION, found_at_extern_table);
        default:
            return false;
    }
}

void update_entry_or_extern_table(DynamicList *entry_or_extern_file_data, int symbol_address, char* direct_label) {
    char *entry_or_extern_file_line = malloc(MAX_LEN_OF_LABEL + 6);
    CHECK_ALLOCATION(entry_or_extern_file_line);
    sprintf(entry_or_extern_file_line, "%s %04d\n", direct_label, symbol_address);
    insert_new_cell_into_dynamic_list(entry_or_extern_file_data, entry_or_extern_file_line);
    entry_or_extern_file_data->is_allocated = true;
}

bool update_direct_addressing_from_symbols_table_or_print_errors(DynamicList symbols_table, DynamicList external_ptrs, DynamicList direct_labels_ptrs, DynamicList *extern_file_data) {
    /*also creating the extern file data. maybe separate  it or add a very good documentation*/
    int i;
    bool printed = false;

    for(i = 0; i < direct_labels_ptrs.list_length; i++) {
        ParsedLine *direct_line = (ParsedLine*)direct_labels_ptrs.items[i];
        if(is_source_a_direct_addressing(*direct_line)) {
            int symbol_address;
            bool found_at_extern_table = false;

            if(!find_direct_label_in_tables(symbols_table, external_ptrs, *direct_line, SOURCE, &symbol_address, &found_at_extern_table)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.source.Addressing.Direct.direct);
                printed = true;
            }
            else if(found_at_extern_table) {
                update_entry_or_extern_table(extern_file_data, symbol_address, direct_line->LineTypes.Instruction.source.Addressing.Direct.direct);
            }

            direct_line->LineTypes.Instruction.source.Addressing.Direct.label_counter = symbol_address;

        }

        if(is_dest_a_direct_addressing(*direct_line)){
            int symbol_address;
            bool found_at_extern_table = false;
            if(!find_direct_label_in_tables(symbols_table, external_ptrs, *direct_line, DESTINATION, &symbol_address, &found_at_extern_table)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct);
                printed = true;
            }
            else if(found_at_extern_table){
                update_entry_or_extern_table(extern_file_data, symbol_address, direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct);
            }

            direct_line->LineTypes.Instruction.dest.Addressing.Direct.label_counter = symbol_address;

        }
    }

    if(printed){
        return false;
    }
    return true;
}

void error_massage(char *entry_label, bool *printed_errors) {
    printf("entry: %s is not found at the symbols table.\n", entry_label);
    *printed_errors = true;
}

bool parse_the_entry_table_to_output_file(DynamicList entry_ptrs, DynamicList *entry_file_data, DynamicList symbols_table) {
    int i;
    int found_at_index;
    char* entry_label;
    bool printed_errors = false;

    for (i=0; i<entry_ptrs.list_length; i++) {
        ParsedLine *entry_line = (ParsedLine*)entry_ptrs.items[i];
        entry_label = entry_line->LineTypes.Directive.DirectiveTypes.entry_or_extern;
        found_at_index = find_in_table(symbols_table, entry_label, SYMBOLS_TABLE);

        if(found_at_index == -1) {
            error_massage(entry_label, &printed_errors);
        }
        else {
            update_entry_or_extern_table(entry_file_data, ((ParsedLine*)symbols_table.items[found_at_index])->mete_data.instruction_counter, entry_label);
        }
    }
    if(printed_errors) {
        return false;
    }
    return true;
}

bool convert_data_directive_to_binary(int* data_numbers, int num_of_elements) {
    return true;
}

bool convert_directive_line_to_binary(ParsedLine *line, DynamicList* binary_list) {
    AssemblyDirective directive_type = line->LineTypes.Directive.directive_type;
    switch (directive_type) {
        case DATA:
            int* data_numbers = line->LineTypes.Directive.DirectiveTypes.DirectiveData.data_numbers;
            int num_of_elements = line->LineTypes.Directive.DirectiveTypes.DirectiveData.num_of_elements;
            convert_data_directive_to_binary(data_numbers, num_of_elements);
            break;
        case STRING:
            break;
        case EXTERN:
            break;
        case ENTRY:
            break;
        default:
            break;
    }

    return true;
}

bool convert_parsed_lines_to_binary(DynamicList parsed_lines_list, DynamicList* binary_list) {
    int i;
    for (i = 0; i < parsed_lines_list.list_length; i++) {
        ParsedLine *line = (ParsedLine*)parsed_lines_list.items[i];
        switch (line->line_type) {
            /* no need to check for ERROR_LINE, because the program will check for them before*/
            case DIRECTIVE_LINE:
                break;
            case COMMAND_LINE:
                break;
            case EMPTY_OR_COMMENT_LINE:
                /*no need to parse*/
                break;
            default:
                break;
        }
    }
    return true;
}

bool found_errors_in_the_assembly_input_file(DynamicList errors_ptrs) {
    /*checks for error lines. if there are error lines,
     * the lines will be printed.
     * the function returns:    true if there are errors
     *                          and false if there are not
     */

    /** TODO: add it to the first pass*/

    int i;
    if(errors_ptrs.list_length == 0) {
        return false;
    }
    for(i = 0; i< errors_ptrs.list_length; i++) {
        printf("%s", ((ParsedLine*)errors_ptrs.items[i])->LineTypes.error_str);
    }
    return true;
}

SecondPassOutput initialize_second_pass_output(FirstPassOutput first_pass_output) {
    SecondPassOutput second_data_output;
    second_data_output.direct_labels_ptrs = first_pass_output.direct_labels_ptrs;
    second_data_output.parsed_lines_list = first_pass_output.parsed_lines_list;
    second_data_output.errors_ptrs = first_pass_output.errors_ptrs;
    second_data_output.success = true;
    initialize_dynamic_list(&second_data_output.extern_file_data, sizeof(char) * (MAX_LEN_OF_LABEL + 6)); /*to insert directly the output fine of the extern*/
    initialize_dynamic_list(&second_data_output.entry_file_data, sizeof(char) * (MAX_LEN_OF_LABEL + 6)); /*to insert directly the output fine of the entry*/
    return second_data_output;
}

SecondPassOutput second_pass(FirstPassOutput first_pass_output){
    bool result = true;
    SecondPassOutput second_pass_output = initialize_second_pass_output(first_pass_output);

    if (!first_pass_output.success){
        result = false;
    }
    else if(!update_direct_addressing_from_symbols_table_or_print_errors(first_pass_output.symbols_table, first_pass_output.external_ptrs, first_pass_output.direct_labels_ptrs, &second_pass_output.extern_file_data)){
        result = false;
    }
    else if(!parse_the_entry_table_to_output_file(first_pass_output.entry_ptrs, &second_pass_output.entry_file_data, first_pass_output.symbols_table)){
        result = false;
    }
    else if(found_errors_in_the_assembly_input_file(second_pass_output.errors_ptrs)) {
        result = false;
    }

    free_dynamic_list(&first_pass_output.symbols_table);
    free_dynamic_list(&first_pass_output.direct_labels_ptrs);
    free_dynamic_list(&first_pass_output.external_ptrs);

    second_pass_output.success = result;
    return second_pass_output;
}
