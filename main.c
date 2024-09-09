#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_files.h"

bool assembler(const char *input_file_name){
    FirstPassOutput first_pass_output;
    SecondPassOutput second_pass_output;

    /*First pass: */
    first_pass_output = first_pass(input_file_name);
    if (!first_pass_output.success){
        return false;
    }

    /*Second pass: */
    second_pass_output = second_pass(first_pass_output);
    if (!second_pass_output.success){
        return false;
    }

    /*Exporting files: */
    return export_output_assembler_files(second_pass_output, input_file_name);
}

void assembler_process(const char *input_file_name){
    char* am_extention;
    bool preprocess_macro_done_successfully = preprocess_macro(input_file_name);

    printf("processing file: %s\n", input_file_name);

    if (!preprocess_macro_done_successfully){
        printf("file: %s error occurred in processing the file with the macros.\n", input_file_name);
        exit(1);
    }
    am_extention = change_file_extention(input_file_name, FILE_EXTENSION_PREPROCESSOR);
    if(assembler(am_extention)) {
        printf("file: %s, processed successfully\n\n", input_file_name);
    }
    else {
        printf("error occurred while processing file: %s. fix the errors above and run the assembles again.\n", input_file_name);
    }


    free(am_extention);
}

void check_if_user_inserted_files(int argc){
    if (argc == 1){
        /*No input files inserted*/
        fprintf(stderr, "Please run this program again with assembly code file / files\n");
        exit(1);
    }
}

void process_all_the_input_files(int argc, const char *argv[]){
    int i;
    check_if_user_inserted_files(argc);
    for (i = 1; i < argc; i++)
    {
        /*all the assembler process for one input file at a time*/
        assembler_process(argv[i]);
    }
}

int main(int argc, const char *argv[]){
    process_all_the_input_files(argc, argv);
    return 0;
}