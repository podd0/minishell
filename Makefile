NAME = minishell

SOURCES  := $(wildcard *.c) $(wildcard libft/*.c) $(wildcard libft/vector/*.c)

OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -g -I. -I./libft -Werror  -I/usr/local/include -fsanitize=address -fsanitize=undefined

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS)  $(CFLAGS) -lreadline -o $@ 

%.o: %.c
	$(CC) -c $(CFLAGS) $? -o $@

clean:
	$(RM) -f $(OBJECTS)

fclean: clean
	$(RM) -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
