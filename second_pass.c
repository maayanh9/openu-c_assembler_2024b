#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "second_pass.h"
#include "settings.h"
#include "dynamic_list.h"
#include "assembler.h"


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

bool find_in_table(DynamicList table, InstructionParameter direct_parameter, Table which_table, int* symbol_address, int insrucion_counter) {
    char* direct_element_name = direct_parameter.Addressing.Direct.direct;
    int i;
    for(i = 0; i< table.list_length; i++) {
        ParsedLine *symbols_table_cell = (ParsedLine *)table.items[i];
        if(label_equals_this_cell(symbols_table_cell, which_table, direct_element_name)) {
            if(which_table == EXTERN_TABLE) {
                *symbol_address = insrucion_counter;
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
                   InstructionParameter direct_src_or_dest, int *symbol_address, int instruction_counter) {
    return find_in_table(symbols_table, direct_src_or_dest, SYMBOLS_TABLE, symbol_address, instruction_counter) ||
           find_in_table(entry_ptrs, direct_src_or_dest, ENTRY_TABLE, symbol_address, instruction_counter) ||
           find_in_table(external_ptrs, direct_src_or_dest, EXTERN_TABLE, symbol_address, instruction_counter);
}

bool find_direct_label_in_tables(DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs, ParsedLine direct_line, SourceOrDest src_or_dest, int *symbol_address) {
    switch (src_or_dest) {
        case SOURCE:
            return find_in_tables(symbols_table, entry_ptrs, external_ptrs, direct_line.LineTypes.Instruction.source, symbol_address, direct_line.mete_data.instruction_counter);
        case DESTINATION:
            return find_in_tables(symbols_table, entry_ptrs, external_ptrs, direct_line.LineTypes.Instruction.dest, symbol_address, direct_line.mete_data.instruction_counter);
        default:
            return false;
    }
}

bool update_direct_addressing_from_symbols_table_or_print_errors(DynamicList symbols_table, DynamicList entry_ptrs, DynamicList external_ptrs, DynamicList direct_labels_ptrs) {
    int i;
    bool printed = false;
    printf("\n");
    for(i = 0; i < direct_labels_ptrs.list_length; i++) {
        ParsedLine *direct_line = (ParsedLine*)direct_labels_ptrs.items[i];
        if(is_source_a_direct_addressing(*direct_line)) {
            int symbol_address;
            if(!find_direct_label_in_tables(symbols_table, entry_ptrs, external_ptrs, *direct_line, SOURCE, &symbol_address)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.source.Addressing.Direct.direct);
                printed = true;
            }
            else {
                printf("%d: %s\t", symbol_address, direct_line->LineTypes.Instruction.source.Addressing.Direct.direct);
            }
        }
        if(is_dest_a_direct_addressing(*direct_line)){
            int symbol_address;
            if(!find_direct_label_in_tables(symbols_table, entry_ptrs, external_ptrs, *direct_line, DESTINATION, &symbol_address)){
                printf("no reference to label: %s\n", direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct);
                printed = true;
            }
            else {
                printf("%d: %s\t", symbol_address, direct_line->LineTypes.Instruction.dest.Addressing.Direct.direct);
            }
        }
    }
    if(printed){
        return false;
    }
    return true;
}

ParsedDataOutput second_pass(ParsedDataOutput first_pass_output){
    bool result = true;
    ParsedDataOutput second_data_output = first_pass_output;

    if (!first_pass_output.success){
        result = false;
    }
    else if(!update_direct_addressing_from_symbols_table_or_print_errors(second_data_output.symbols_table, first_pass_output.entry_ptrs, first_pass_output.external_ptrs, second_data_output.direct_labels_ptrs)){
        result = false;
    }

    free_parsed_data_output_dynamic_lists(second_data_output.parsed_lines_list, second_data_output.symbols_table, second_data_output.errors_ptrs, second_data_output.entry_ptrs, second_data_output.external_ptrs, second_data_output.direct_labels_ptrs);
    second_data_output.success = result;
    return second_data_output;
}
