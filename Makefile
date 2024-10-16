NAME = minishell

SOURCES  := builtins.c builtins2.c cd.c env.c exec2.c exec.c export.c find_executable.c handlers.c minishell.c parser2.c \
			parser.c parser3.c sort.c tokenizer2.c tokenizer.c utils.c var_utils.c libft/ft_atoi.c libft/ft_bzero.c \
			libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c libft/ft_isascii.c libft/ft_isdigit.c \
			libft/ft_isprint.c libft/ft_itoa.c libft/ft_memchr.c libft/ft_memcmp.c libft/ft_memcpy.c \
			libft/ft_memmove.c libft/ft_memset.c libft/ft_putchar_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c\
			libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strchr.c libft/ft_strdup.c libft/ft_striteri.c libft/ft_strjoin.c\
			libft/ft_strlcat.c libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strmapi.c libft/ft_strncmp.c libft/ft_strnstr.c\
			libft/ft_strrchr.c libft/ft_strtrim.c libft/ft_substr.c libft/ft_tolower.c libft/ft_toupper.c libft/printf.c\
			libft/printf_utils.c libft/safe_alloc.c libft/to_string.c libft/vector/vch_0.c libft/vector/vch_1.c libft/vector/vch_2.c\
			libft/vector/vi_0.c libft/vector/vi_1.c libft/vector/vi_2.c libft/vector/vl_0.c libft/vector/vl_1.c libft/vector/vl_2.c\
			libft/vector/vlu_0.c libft/vector/vlu_1.c libft/vector/vlu_2.c libft/vector/vstr_0.c libft/vector/vstr_1.c libft/vector/vstr_2.c 

OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -g -I. -I./libft -Werror  -I/usr/local/include
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
