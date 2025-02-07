CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = pipex_bonus.c pipex_utils.c env_tool.c

HEADERS = pipex_bonus.h libft.h

LIBFT = libft.a

OBJ_FILES = $(SRC:.c=.o)

NAME = pipex_bonus

all: $(NAME)

$(NAME): $(OBJ_FILES)
		$(CC) $(OBJ_FILES) $(HEADER) $(LIBFT) -o $(NAME)

clean: 
		rm -f $(OBJ_FILES)

fclean:clean
		rm -f $(NAME)

re: fclean all

.PHONY :all clean fclean re
