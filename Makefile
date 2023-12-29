NAME=amadeus

CC=g++
FLAGS= -std=c++11 -pedantic -Wall -Wextra -Werror

FILES= objs/manager.o objs/managerStats.o \
	objs/ui.o objs/uiPlanner.o \
	objs/uiPlannerSearch.o objs/uiStatSearch.o

all: $(NAME)

objs/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(FILES)
	$(CC) -pthread $(FLAGS) $(FILES) -o $(NAME)

doxygen:
	@doxygen Doxyfile

clean:
ifeq ($(OS),Windows_NT)
	cmd /C rmdir /s /q objs
	cmd /C mkdir objs 
else
	rm -fr $(FILES)
endif

fclean: clean
ifeq ($(OS),Windows_NT)
	cmd /C del $(NAME).exe
else
	rm -fr $(NAME)
endif

re: fclean all

.PHONY: all clean fclean re
