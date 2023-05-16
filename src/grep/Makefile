
FLAGS = -Wall -Wextra -Werror

all: clean s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep
	gcc $(FLAGS) s21_grep.c -o tests/s21_grep
	
clang:
	clang-format -style=Google -n *.c *.h
	clang-format -style=Google -i *.c *.h

rebuild:
	make clean
	make all

clean:
	rm -rf ./tests/s21_grep
	rm -rf s21_grep
test:
	make clean
	make s21_grep
	cd ./tests; ./test_func_grep.sh
	