NAME = webserv

INCLUDES = -I./includes

FLAGS = -Wall -Wextra -Werror -std=c++98 ${INCLUDES} -g -fsanitize=address,undefined

SRCS = ${wildcard ./src/**/*.cpp} ${wildcard ./src/*.cpp} ${wildcard ./*.cpp}

CC = c++

OBJECT_FILES = $(patsubst %.cpp,%.o,$(SRCS))

%.o : %.cpp
	$(CC) $(FLAGS) -c $^ -o $@

all : $(NAME)

$(NAME) : $(OBJECT_FILES)
	$(CC) $(FLAGS) $^ -o $(NAME)

clean :
	rm -rf ${OBJECT_FILES}

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re