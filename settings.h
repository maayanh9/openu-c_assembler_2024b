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
#define LEN_OF_COMMANDS_LIST 16

#define NUM_OF_REGISTERS 8

#define FILE_EXTENSION_PREPROCESSOR ".am"
#define FILE_EXTENSION_INPUT_ASSEMBLER_FILE ".as"
#define FILE_EXTENSION_OBJECT_FILE ".ob"
#define FILE_EXTENSION_EXTERNAL_FILE ".ext"
#define FILE_EXTENSION_ENTRY_FILE ".ent"

#define LENGTH_OF_LINE_OBJECT_FILE 11 /* 4 digits for address + 1 for space + 5 for the word + 1 for \n*/

#define BIT_STORAGE_STARTS_FOR_OPCODE 11
#define BIT_STORAGE_STARTS_FOR_SOURCE_ADDRESSING_METHOD 7
#define BIT_STORAGE_STARTS_FOR_DESTINATION_ADDRESSING_METHOD 3
#define BIT_STORAGE_STARTS_FOR_A_R_E_FIELDS 0

#define BIT_STORAGE_STARTS_FOR_SOURCE_REGISTER 6
#define BIT_STORAGE_STARTS_FOR_DESTINATION_REGISTER 3


static const int addressing_to_fit_object_file[4] = {1, 2, 4, 8}; /* 0001, 0010, 0100, 1000 in decimal */
static const int a_r_e_fields[3] = {4, 2, 1}; /* set the right bit in the order on A=100, R=010, E=001 */

static const char *instructions_commands_and_addressing[LEN_OF_COMMANDS_LIST][4] = {
    /*command, dst, src, how_many_parameters*/
    {"mov", "123", "0123", "2"},
    {"cmp", "0123", "0123", "2"},
    {"add", "123", "0123", "2"},
    {"sub", "123", "0123", "2"},
    {"lea", "123", "1", "2"},
    {"clr", "123", NULL, "1"},
    {"not", "123", NULL, "1"},
    {"inc", "123", NULL, "1"},
    {"dec", "123", NULL, "1"},
    {"jmp", "12", NULL, "1"},
    {"bne", "12", NULL, "1"},
    {"red", "123", NULL, "1"},
    {"prn", "0123", NULL, "1"},
    {"jsr", "12", NULL, "1"},
    {"rts", NULL, NULL, "0"},
    {"stop", NULL, NULL, "0"}
};

static const char* directives_list[LEN_OF_DIRECTIVE_LIST] = {"data", "string", "extern", "entry"};


typedef struct Node{
    void *value;
    struct Node* next;
} Node;



typedef struct LineMetaData{
    int data_counter;
    int instruction_counter;
    int space_to_keep_for_current_line;
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
} AssemblyCommands;

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
    INDIRECT_REGISTER,
    DIRECT_REGISTER,
    INVALID_OR_NOT_IN_USE = -1
} AddressingMethod;

typedef struct InstructionParameter{
    AddressingMethod addressing_method;
    union Addressing
    {
        int immediate;
        struct{
            int label_counter;
            char direct[MAX_LEN_OF_LABEL];
        } Direct;
        int register_num;
    } Addressing;
    
} InstructionParameter;

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
            AssemblyCommands command;
            InstructionParameter source;
            InstructionParameter dest;
        } Instruction;
        
    }LineTypes;
    
    HasLabel has_label;
    char label[MAX_LEN_OF_LABEL + 1];

}ParsedLine;


#endif