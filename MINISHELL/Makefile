# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/15 21:32:26 by jeid              #+#    #+#              #
#    Updated: 2026/02/02 22:43:11 by jeid             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCDIR = src
INCDIR = includes
LIBFTDIR = Libft
OBJDIR = objs

LIBFT = $(LIBFTDIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I$(INCDIR) -I$(LIBFTDIR)

SRCS = $(SRCDIR)/main.c \
	$(SRCDIR)/Builtin/cd.c $(SRCDIR)/Builtin/echo.c $(SRCDIR)/Builtin/env.c $(SRCDIR)/Builtin/exit.c \
	$(SRCDIR)/Builtin/export.c $(SRCDIR)/Builtin/helper_export.c $(SRCDIR)/Builtin/export_utils.c $(SRCDIR)/Builtin/pwd.c $(SRCDIR)/Builtin/unset.c \
	$(SRCDIR)/Execution/executing.c $(SRCDIR)/Execution/helper_execute.c $(SRCDIR)/Execution/helper_execute2.c \
	$(SRCDIR)/Execution/helper_execute3.c $(SRCDIR)/Execution/helper_execute4.c $(SRCDIR)/Execution/helper_execute5.c \
	$(SRCDIR)/Execution/helper_redirections.c $(SRCDIR)/Execution/redirections.c $(SRCDIR)/Execution/here_doc.c \
	$(SRCDIR)/System/set_env.c $(SRCDIR)/System/signals.c \
	$(SRCDIR)/Tokenization/tokenizing.c $(SRCDIR)/Tokenization/parsing.c $(SRCDIR)/Tokenization/helper_tokenizer.c \
	$(SRCDIR)/Tokenization/helper_functions.c $(SRCDIR)/Tokenization/helper_expander.c $(SRCDIR)/Tokenization/helper_quote_check.c
OBJECTS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) -lreadline -o $(NAME)
	@echo "✓ $(NAME) created successfully!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "Building libft..."
	@make -C $(LIBFTDIR)

clean:
	@echo "Cleaning object files..."
	@make clean -C $(LIBFTDIR)
	@rm -rf $(OBJDIR)
	@echo "✓ Clean complete!"

fclean: clean
	@echo "Removing binaries..."
	@make fclean -C $(LIBFTDIR)
	@rm -f $(NAME)
	@echo "✓ Full clean complete!"

re: fclean all

.PHONY: all clean fclean re
