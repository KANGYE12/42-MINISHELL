# =========================
#      VARIABLES
# =========================

NAME = minishell

# Find all .c files inside parsing/
SRCS =  parsing/check_args.c \
		parsing/enviroment.c \
		parsing/free.c \
		parsing/main.c \
		parsing/parse_cmd_helper.c \
		parsing/parse_cmd.c \
		parsing/parse_tokens.c \
		parsing/remove_quotes.c \
		parsing/token_special_cases.c \
		parsing/token_split_helper.c \
		parsing/tokens_split.c \
		parsing/utils.c \
		parsing/utils02.c \
		parsing/variables_expansion.c \
		parsing/variables_expansion02.c \
		execute/builtins.c \
		execute/builtins2.c \
		execute/builtins3.c \
		execute/builtins4.c \
		execute/execute.c \
		execute/find_path.c \
		execute/redirections.c 

# Replace .c with .o (objects will be in the same folder)
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft -g

RM = rm -f

# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# =========================
#         RULES
# =========================

all: $(LIBFT) $(NAME)

# Build the executable. Also linking directly the functions GNU Readline library
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) -lreadline -lhistory -lncurses

# Build libft if needed
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Compile each .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# =========================
#        CLEANING
# =========================

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
