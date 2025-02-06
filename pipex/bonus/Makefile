CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = pipex_bonus.c

HEADERS = pipex_bonus.h libft.h

LIBFT = libft.a

OBJ_FILES = $(SRC:.c=.o)

NAME = pipex_bonus

all: $(NAME)

$(NAME):
		$(CC) $(SRC) $(HEADER) $(LIBFT) -o $(NAME)

clean: 
		rm -f $(OBJ_FILES)

fclean:clean
		rm -f $(NAME)

re: fclean all

.PHONY :all clean fclean re
