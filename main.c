#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int preprocess_macro(const char *input_file_name);

void assembler_process(const char *input_file_name){
    int preprocess_macro_done_successfully = preprocess_macro(input_file_name);
    if (preprocess_macro_done_successfully == 1){
        printf("file: %s error occurred in processing the file with the macros", input_file_name);
        exit(1);
    }
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