#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "preprocess.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_files.h"

/* Assemble a pre-processed input file name
 * @input_file_name should be pre-processed
 */
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

/* Pre process a given file, then assemble the pre process output
 * @input_file_name to pre process and assemble
 */
void pre_process_and_assemble(const char *input_file_name){
    char* am_extention;
    bool preprocess_macro_done_successfully = preprocess_macro(input_file_name);

    printf("\n\t\t*******\nprocessing file: %s\n", input_file_name);

    if (!preprocess_macro_done_successfully){
        printf("file: %s error occurred in processing the file with the macros.\n\n", input_file_name);
        exit(1);
    }
    am_extention = change_file_extention(input_file_name, FILE_EXTENSION_PREPROCESSOR);
    if(assembler(am_extention)) {
        printf("file: %s, processed successfully\n\n", input_file_name);
    }
    else {
        printf("error occurred while processing file: %s. \nfix the errors above and run the assembles again.\n\n", input_file_name);
    }


    free(am_extention);
}

/* Validates the input of the user to make sure he entered at-least one file name */
void check_if_user_inserted_files(int argc){
    if (argc == 1){
        /*No input files inserted*/
        fprintf(stderr, "Please run this program again with assembly code file / files\n");
        exit(1);
    }
}

/* Iterate over all files given as input to the program and assemble them
 * @argc: expected to be passed from main
 * @argv: expected to be passed from main
 */
void process_all_the_input_files(int argc, const char *argv[]){
    int i;
    check_if_user_inserted_files(argc);
    for (i = 1; i < argc; i++)
    {
        /*all the assembler process for one input file at a time*/
        pre_process_and_assemble(argv[i]);
    }
}

/* Entry point of the program */
int main(int argc, const char *argv[]){
    process_all_the_input_files(argc, argv);
    return 0;
}