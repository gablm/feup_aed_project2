NAME=amadeus

CC=g++
FLAGS= -std=c++11 -pedantic -Wall -Wextra -Werror

FILES= objs/airline.o objs/airport.o objs/main.o objs/manager.o objs/ui.o \
	objs/managerStats.o objs/managerSearch.o

all: $(NAME)

objs/%.o: src/%.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(FILES)
	$(CC) $(FLAGS) $(FILES) -o $(NAME)

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
