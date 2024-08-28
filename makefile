FLAGS		= -ansi -pedantic -Wall -g -fsanitize=address
PROG_NAME 	= assembler
BUILD_DIR	= build

all: create_dir $(PROG_NAME)


$(PROG_NAME): $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o $(BUILD_DIR)/text_handler.o $(BUILD_DIR)/assembler.o  $(BUILD_DIR)/dynamic_list.o
	gcc $(FLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o $(BUILD_DIR)/text_handler.o $(BUILD_DIR)/assembler.o $(BUILD_DIR)/dynamic_list.o -o $(BUILD_DIR)/$(PROG_NAME)



$(BUILD_DIR)/main.o: main.c preprocess.h assembler.h
	gcc $(FLAGS) -c main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/preprocess.o: preprocess.c preprocess.h settings.h text_handler.h
	gcc $(FLAGS) -c preprocess.c -o $(BUILD_DIR)/preprocess.o

$(BUILD_DIR)/text_handler.o: text_handler.c text_handler.h settings.h
	gcc $(FLAGS) -c text_handler.c -o $(BUILD_DIR)/text_handler.o

$(BUILD_DIR)/assembler.o: assembler.c assembler.h settings.h
	gcc $(FLAGS) -c assembler.c -o $(BUILD_DIR)/assembler.o

$(BUILD_DIR)/dynamic_list.o: dynamic_list.c dynamic_list.h text_handler.h
	gcc $(FLAGS) -c dynamic_list.c -o $(BUILD_DIR)/dynamic_list.o

create_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROG_NAME)

.PHONY: all create_dir clean
