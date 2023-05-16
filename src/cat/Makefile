
FLAGS = -Wall -Wextra -Werror

all: clean s21_cat

s21_cat: Makefile s21_cat.h s21_cat.c
	gcc $(FLAGS) s21_cat.c -o s21_cat
	gcc $(FLAGS) s21_cat.c -o tests/s21_cat
	
clang:
	clang-format -n *.c *.h
	clang-format -i *.c *.h

rebuild:
	make clean
	make all

clean:
	rm -rf ./tests/s21_cat
	rm -rf s21_cat
test:
	make clean
	make s21_cat
	cd ./tests; ./test_func_cat.sh
	