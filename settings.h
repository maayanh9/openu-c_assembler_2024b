#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81
#define MAX_LEN_MACRO_NAME 76
#define MAX_LEN_OF_A_SINGLE_WORD 80
#define MAX_LEN_ERROR_STR 300

#define MAX_NUMBERS_IN_DATA_LABEL 38
#define MAX_ASCII_STRING_LEN 74
#define MAX_LEN_OF_LABEL 31

#define LEN_OF_DIRECTIVE_LIST 4

#define FILE_EXTENTION_PREPROCESSOR ".am"
#define FILE_EXTENTION_INPUT_ASSEMBLER_FILE ".as"
#define FILE_EXTENTION_OBJECT_FILE ".ob"
#define FILE_EXTENTION_EXTERNAL_FILE ".ext"

/*
static const char* instructions_commands_list[16] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};*/
static const char* directives_list[LEN_OF_DIRECTIVE_LIST] = {"data", "string", "extern", "entry"};


typedef struct Node{
    void *value;
    struct Node* next;
} Node;



typedef struct LineMetaData{
    int data_counter;
    int instruction_counter;
    int line_counter;
}LineMetaData;

typedef enum{
    ERROR_LINE,
    DIRECTIVE_LINE,
    COMMAND_LINE,
    EMPTY_OR_COMMENT_LINE
}LineType;

typedef enum{
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
} AssemblyInstruction;

typedef enum{
    DATA,
    STRING,
    EXTERN,
    ENTRY
} AssemblyDirective;

typedef enum{
    NO_LABEL,
    HAS_LABEL
} HasLabel;


typedef enum{
    IMMEDIATE,
    DIRECT,
    DIRECT_REGISTER,
    INDIRECT_REGISTER
} AddressingMethod;

typedef struct ParsedLine{
    LineMetaData mete_data;
    LineType line_type;
    union LineTypes{
        char error_str[MAX_LEN_ERROR_STR];
        struct {
            AssemblyDirective directive_type;
            union DirectiveTypes{
                struct {
                    int num_of_elements;
                    int data_numbers[MAX_NUMBERS_IN_DATA_LABEL];
                }DirectiveData;
                char ascii_string[MAX_ASCII_STRING_LEN];
                char entry_or_extern[MAX_LEN_OF_A_SINGLE_WORD - 6];
            } DirectiveTypes;
        } Directive;
        struct {
            AssemblyInstruction instruction;
            
        } Instruction;
        
    }LineTypes;
    
    HasLabel has_label;
    char label[MAX_LEN_OF_LABEL + 1];

}ParsedLine;


#endif