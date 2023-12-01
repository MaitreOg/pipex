NAME	= pipex

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = pipex_utils.c pipex.c split.c

OBJECTS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re