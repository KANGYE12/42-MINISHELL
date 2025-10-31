# =========================
#      VARIABLES
# =========================

NAME = minishell

# Find all .c files inside parsing/
SRCS = $(wildcard parsing/*.c execute/*.c)

# Replace .c with .o (objects will be in the same folder)
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft

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
