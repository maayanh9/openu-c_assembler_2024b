FLAGS		= -ansi -pedantic -Wall -g -fsanitize=address
PROG_NAME 	= assembler
BUILD_DIR	= build

all: create_dir $(PROG_NAME)


$(PROG_NAME): $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o
	gcc $(FLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/preprocess.o -o $(BUILD_DIR)/$(PROG_NAME)



$(BUILD_DIR)/main.o: main.c preprocess.h
	gcc $(FLAGS) -c main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/preprocess.o: preprocess.c preprocess.h settings.h
	gcc $(FLAGS) -c preprocess.c -o $(BUILD_DIR)/preprocess.o

create_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROG_NAME)

.PHONY: all create_dir clean
