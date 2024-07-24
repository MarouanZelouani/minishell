NAME = minishell
CC = cc
LIBFT_DIR = ./lib/libft
LIBFT = $(LIBFT_DIR)/libft.a
FLAGS = -Wall -Wextra -Werror
SRC = ./src/main.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(FLAGS) -lreadline $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@
	
clean:
	@rm -rf $(OBJ)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -rf $(LIBFT)
	@rm -rf $(NAME)

re: fclean all