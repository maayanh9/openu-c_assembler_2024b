FLAGS		= -ansi -pedantic -Wall -g -lm
PROG_NAME 	= assembler
BUILD_DIR	= build

all: create_dir $(PROG_NAME)

$(PROG_NAME): main.o preprocess.o first_pass.o second_pass.o dynamic_list.o output_files.o text_and_digits_handler.o
	gcc $(FLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$@

main.o: main.c preprocess.h settings.h text_and_digits_handler.h \
 first_pass.h dynamic_list.h second_pass.h output_files.h

preprocess.o: preprocess.c preprocess.h settings.h \
 text_and_digits_handler.h

first_pass.o: first_pass.c text_and_digits_handler.h settings.h \
 dynamic_list.h first_pass.h

second_pass.o: second_pass.c second_pass.h first_pass.h dynamic_list.h \
 settings.h text_and_digits_handler.h

dynamic_list.o: dynamic_list.c dynamic_list.h text_and_digits_handler.h

output_files.o: output_files.c output_files.h second_pass.h first_pass.h \
 dynamic_list.h settings.h text_and_digits_handler.h

text_and_digits_handler.o: text_and_digits_handler.c \
 text_and_digits_handler.h

%.o:
	gcc $(FLAGS) -c $< -o $(BUILD_DIR)/$@

create_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(PROG_NAME)

.PHONY: all create_dir clean
