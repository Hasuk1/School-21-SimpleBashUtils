
FLAGS = -Wall -Wextra -Werror

all: clean s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep
	
clang:
	clang-format -n *.c *.h
	clang-format -i *.c *.h

rebuild:
	make clean
	make all

clean:
	rm -rf s21_grep
test:
	make clean
	make s21_grep
	sh test_func.sh
	