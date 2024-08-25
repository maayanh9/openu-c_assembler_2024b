FLAGS		= -ansi -pedantic -Wall -g -fsanitize=address
PROG_NAME 	= assembler
BUILD_DIR	= build

all: create_dir $(PROG_NAME)


$(PROG_NAME): $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o $(BUILD_DIR)/utils.o
	gcc $(FLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/assembler.o -o $(BUILD_DIR)/$(PROG_NAME)



$(BUILD_DIR)/main.o: main.c preprocess.h assembler.h
	gcc $(FLAGS) -c main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/preprocess.o: preprocess.c preprocess.h settings.h utils.h
	gcc $(FLAGS) -c preprocess.c -o $(BUILD_DIR)/preprocess.o

$(BUILD_DIR)/utils.o: utils.c utils.h settings.h
	gcc $(FLAGS) -c utils.c -o $(BUILD_DIR)/utils.o

$(BUILD_DIR)/assembler.o: assembler.c assembler.h settings.h
	gcc $(FLAGS) -c assembler.c -o $(BUILD_DIR)/assembler.o

create_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROG_NAME)

.PHONY: all create_dir clean
