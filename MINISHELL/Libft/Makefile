# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/15 21:31:38 by jeid              #+#    #+#              #
#    Updated: 2026/01/17 17:26:25 by jeid             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SOURCES = \
	ft_atoi.c \
	ft_char_is.c \
	ft_error.c \
	ft_itoa.c \
	ft_strcat.c \
	ft_strchr.c \
	ft_strcmp.c \
	ft_strcspn.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_strlen.c \
	ft_strsplit.c \
	ft_strtrim.c \
	ft_struct_lst.c \
	ft_struct_update_lst.c \
	ft_word_count.c

OBJDIR = objs
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC -I../includes

all: $(NAME)

$(NAME): $(OBJECTS)
	@echo "Creating $(NAME)..."
	@$(AR) -rcs $@ $?
	@echo "✓ $(NAME) created successfully!"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning libft object files..."
	@rm -rf $(OBJDIR)
	@echo "✓ Libft clean complete!"

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)
	@echo "✓ Libft fclean complete!"

re: fclean all

.PHONY: all bonus clean fclean re
