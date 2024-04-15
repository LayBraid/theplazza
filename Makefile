##
## EPITECH PROJECT, 2023
## cpp-epitech-template
## File description:
## Makefile
##

CC			=	g++

SRC			=	src/main.cpp \
				src/Kitchen.cpp \
				src/Reception.cpp \
				src/Storage.cpp \
				src/Timer.cpp \
				src/MessageQueue.cpp \
				src/Fork.cpp \
				src/Logger.cpp

TESTS 		=	$(filter-out src/main.cpp , $(SRC)) \
				tests/testMessageQueue.cpp

OBJ			=	$(SRC:%.cpp=%.o)

NAME		=	plazza

TEST_NAME	=	unit_tests

CXXFLAGS	=	-std=c++20 -Wall -Wextra -g3 -I./include

.PHONY: all
all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ)
	rm -f *.gc*
	rm -f vgcore*

.PHONY: fclean
fclean:	clean
	rm -f $(NAME)
	rm -f unit_tests

.PHONY:	re
re:	fclean all

###
# Tests
###
.PHONY: unit_tests
unit_tests: fclean $(NAME) $(TESTS)
	$(CC) -o $(TEST_NAME) $(TESTS) $(CXXFLAGS) --coverage -lcriterion

.PHONY: tests_run
tests_run:	unit_tests
	./$(TEST_NAME)

.PHONY: coverage
coverage:	tests_run
	gcovr --exclude tests

###
# Documentation
###
.PHONY: docs
docs:
	doxygen

.PHONY: clean_docs
clean_docs:
	rm -rf doc/html
