#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_LEN_LINE_ASSEMBLY_FILE 81
#define MAX_LEN_MACRO_NAME 76
#define MAX_LEN_OF_A_SINGLE_WORD 80
#define MAX_LEN_ERROR_STR 300

#define MAX_NUMBERS_IN_DATA_LABEL 38
#define MAX_ASCII_STRING_LEN 74

#define FILE_EXTENTION_PREPROCESSOR ".am"
#define FILE_EXTENTION_INPUT_ASSEMBLER_FILE ".as"
#define FILE_EXTENTION_OBJECT_FILE ".ob"
#define FILE_EXTENTION_EXTERNAL_FILE ".ext"



typedef struct Node{
    void *value;
    struct Node* next;
} Node;


typedef struct DynamicList{
    void* items;
    int item_size;
    int list_length;
    int max_capacity;
} DynamicList;

typedef struct LineMetaData{
    int data_counter;
    int instruction_counter;
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
    LABEL,
    NO_LABEL
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
        char* error_str[MAX_LEN_ERROR_STR];
        struct {
            AssemblyDirective directive_type;
            union DirectiveTypes{
                int data_numbers[MAX_NUMBERS_IN_DATA_LABEL];
                char ascii_string[MAX_ASCII_STRING_LEN];
                char entry_or_extern[MAX_LEN_OF_A_SINGLE_WORD - 6];
            } DirectiveTypes;
        } Directive;
        struct {
            AssemblyInstruction instruction;
            
        } Instruction;
        
    }LineTypes;
    
    HasLabel has_label;
    char label[MAX_LEN_OF_A_SINGLE_WORD];

}ParsedLine;





#endif