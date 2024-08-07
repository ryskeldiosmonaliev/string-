OS=$(shell uname)
CC=gcc
CFLAGS = -std=c11 -Werror -Wextra -Wall
ifeq ($(OS), Darwin)
 CHECK= -lcheck -lpthread -lm
else
 CHECK= -lcheck -lpthread -lm -lsubunit
endif
SOURCE = s21_string.c s21_sprintf.c Tests/*.c
OBJECTS = $(SOURCE:.c=.o)

all: main

main: s21_string.a test

s21_string.a: s21_string.o s21_sprintf.o
	ar rcs s21_string.a s21_string.o s21_sprintf.o
	ranlib s21_string.a

test: s21_string.a
	$(CC) $(CFLAGS) $(SOURCE) s21_string.a $(CHECK) -o test `pkg-config --cflags --libs check`
	./test

debug_flag:
	$(eval CFLAGS += -g)

debug: clean debug_flag main
	rm -rf *.o

rebuild: clean all

gcov_report: s21_string.a
	$(CC) $(CFLAGS) --coverage $(SOURCE) s21_string.a -L. s21_string.a $(CHECK) -o ./test
	./test
	mkdir report
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html
	rm -rf *.gcda *.gcno *.out

leaks: clean test
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./test

clean:
	rm -rf *.o *.gcda *.gcno *.a *.out *.info *.dSYM report test



