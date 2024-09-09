#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "second_pass.h"
#include "settings.h"
#include "dynamic_list.h"
#include "first_pass.h"
#include "text_handler.h"


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

bool find_in_table(DynamicList table, InstructionParameter direct_parameter, Table which_table, int* symbol_address, int insrucion_counter, int
                   how_many_parameters_for_command, SourceOrDest src_or_dst, bool* found_at_extern_table) {
    char* direct_element_name = direct_parameter.Addressing.Direct.direct;
    int i;
    for(i = 0; i< table.list_length; i++) {
        ParsedLine *symbols_table_cell = (ParsedLine *)table.items[i];
        if(label_equals_this_cell(symbols_table_cell, which_table, direct_element_name)) {
            if(which_table == EXTERN_TABLE) {
                *found_at_extern_table = true;
                *symbol_address = get_extern_instruction_counter(insrucion_counter, how_many_parameters_for_command, src_or_dst);
            }
            else {
                *symbol_address = symbols_table_cell->mete_data.instruction_counter;
            }
            return true;
        }
    }
    return false;
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

int find_in_tables(DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs,
                   InstructionParameter direct_src_or_dest, int *symbol_address, int instruction_counter, int how_many_parameters_for_command, SourceOrDest src_or_dst, bool* found_at_extern_table) {
    return find_in_table(symbols_table, direct_src_or_dest, SYMBOLS_TABLE, symbol_address, instruction_counter, how_many_parameters_for_command, src_or_dst, found_at_extern_table) ||
           find_in_table(entry_ptrs, direct_src_or_dest, ENTRY_TABLE, symbol_address, instruction_counter, how_many_parameters_for_command, src_or_dst, found_at_extern_table) ||
           find_in_table(external_ptrs, direct_src_or_dest, EXTERN_TABLE, symbol_address, instruction_counter, how_many_parameters_for_command, src_or_dst, found_at_extern_table);
}

bool find_direct_label_in_tables(DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs, ParsedLine direct_line, SourceOrDest src_or_dest, int *symbol_address, bool* found_at_extern_table) {
    switch (src_or_dest) {
        case SOURCE:
            return find_in_tables(symbols_table, entry_ptrs, external_ptrs, direct_line.LineTypes.Instruction.source, symbol_address, direct_line.mete_data.instruction_counter, atoi(instructions_commands_and_addressing[direct_line.LineTypes.Instruction.command][3]), SOURCE, found_at_extern_table);
        case DESTINATION:
            return find_in_tables(symbols_table, entry_ptrs, external_ptrs, direct_line.LineTypes.Instruction.dest, symbol_address, direct_line.mete_data.instruction_counter, atoi(instructions_commands_and_addressing[direct_line.LineTypes.Instruction.command][3]), DESTINATION, found_at_extern_table);
        default:
            return false;
    }
}

bool update_direct_addressing_from_symbols_table_or_print_errors(DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs, DynamicList direct_labels_ptrs, DynamicList *extern_file_data) {
    int i;
    bool printed = false;
    char* extern_file = malloc(MAX_LEN_OF_LABEL + 6);

    printf("\n");
    for(i = 0; i < direct_labels_ptrs.list_length; i++) {
        ParsedLine *direct_line = (ParsedLine*)direct_labels_ptrs.items[i];
        if(is_source_a_direct_addressing(*direct_line)) {
            int symbol_address;
            bool found_at_extern_table = false;

            if(!find_direct_label_in_tables(symbols_table, entry_ptrs, external_ptrs, *direct_line, SOURCE, &symbol_address, &found_at_extern_table)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.source.Addressing.Direct.direct);
                printed = true;
            }
            else if(found_at_extern_table) {
                char *extern_file_line = malloc(MAX_LEN_OF_LABEL + 6);
                CHECK_ALLOCATION(extern_file_line);
                sprintf(extern_file_line, "%s %04d\n", direct_line->LineTypes.Instruction.source.Addressing.Direct.direct, symbol_address);
                insert_new_cell_into_dynamic_list(extern_file_data, extern_file_line);
                extern_file_data->is_allocated = true;
            }
        }

        if(is_dest_a_direct_addressing(*direct_line)){
            int symbol_address;
            bool found_at_extern_table = false;
            if(!find_direct_label_in_tables(symbols_table, entry_ptrs, external_ptrs, *direct_line, DESTINATION, &symbol_address, &found_at_extern_table)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct);
                printed = true;
            }
            else if(found_at_extern_table){
                char *extern_file_line = malloc(MAX_LEN_OF_LABEL + 6);
                CHECK_ALLOCATION(extern_file_line);
                sprintf(extern_file_line, "%s %04d\n", direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct, symbol_address);
                insert_new_cell_into_dynamic_list(extern_file_data, extern_file_line);
            }
        }
    }

    if(printed){
        return false;
    }
    return true;
}

SecondPassOutput initialize_second_pass_output(FirstPassOutput first_pass_output) {
    SecondPassOutput second_data_output;
    second_data_output.entry_ptrs = first_pass_output.entry_ptrs;
    second_data_output.direct_labels_ptrs = first_pass_output.direct_labels_ptrs;
    second_data_output.parsed_lines_list = first_pass_output.parsed_lines_list;
    second_data_output.errors_ptrs = first_pass_output.errors_ptrs;
    second_data_output.success = true;
    initialize_dynamic_list(&second_data_output.extern_file_data, sizeof(char) * (MAX_LEN_OF_LABEL + 6)); /*to insert directly the output fine of the extern*/
    return second_data_output;
}

SecondPassOutput second_pass(FirstPassOutput first_pass_output){
    int i;
    bool result = true;
    SecondPassOutput second_pass_output = initialize_second_pass_output(first_pass_output);

    if (!first_pass_output.success){
        result = false;
    }
    else if(!update_direct_addressing_from_symbols_table_or_print_errors(first_pass_output.symbols_table, first_pass_output.entry_ptrs, first_pass_output.external_ptrs, first_pass_output.direct_labels_ptrs, &second_pass_output.extern_file_data)){
        result = false;
    }

    free_parsed_data_output_dynamic_lists(second_pass_output.parsed_lines_list, first_pass_output.symbols_table, first_pass_output.errors_ptrs, first_pass_output.entry_ptrs, first_pass_output.external_ptrs, first_pass_output.direct_labels_ptrs);
    second_pass_output.success = result;
    return second_pass_output;
}
