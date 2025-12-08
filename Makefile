NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I.
LDFLAGS		= -lreadline

# Source files
SRCS		= main.c \
			  lexer.c \
			  parser.c \
			  expander.c \
			  quote_handler.c \
			  validator.c \
			  error.c \
			  utils.c

OBJS		= $(SRCS:.c=.o)

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ Minishell compiled successfully!$(RESET)"

%.o: %.c minishell.h
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)✓ Compiled: $<$(RESET)"

clean:
	@rm -f $(OBJS)
	@echo "$(RED)✗ Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ Executable removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re