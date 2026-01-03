NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -Iincludes
LDFLAGS		= -lreadline

# Directories
SRC_DIR		= src
INC_DIR		= includes
OBJ_DIR		= obj

# Source files
SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/lexer.c \
			  $(SRC_DIR)/parser.c \
			  $(SRC_DIR)/expander.c \
			  $(SRC_DIR)/quote_handler.c \
			  $(SRC_DIR)/validator.c \
			  $(SRC_DIR)/error.c \
			  $(SRC_DIR)/utils.c \
			  $(SRC_DIR)/signals.c \
			  $(SRC_DIR)/env.c \
			  $(SRC_DIR)/builtins.c \
			  $(SRC_DIR)/executor.c

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Colors
BLACK		= \033[0;30m
RED			= \033[0;31m
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
MAGENTA		= \033[0;35m
CYAN		= \033[0;36m
WHITE		= \033[0;37m
BOLD		= \033[1m
RESET		= \033[0m

# Rules
all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo ""
	@echo "$(YELLOW)$(BOLD) Linking object files...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo ""
	@echo "$(GREEN)$(BOLD)╔════════════════════════════════════════╗$(RESET)"
	@echo "$(GREEN)$(BOLD)║  $(WHITE)✓ $(GREEN)Build completed successfully!$	(GREEN)  ║$(RESET)"
	@echo "$(GREEN)$(BOLD)╚════════════════════════════════════════╝$(RESET)"
	@echo ""

define compile_file
	@printf "$(CYAN)[$(YELLOW)$(1)/$(2)$(CYAN)]$(RESET) $(BLUE)Compiling$(RESET) $(BOLD)$(notdir $(3))$(RESET)..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $(3) -o $(4) && \
		echo " $(GREEN)$(BOLD)✓$(RESET)" || \
		(echo " $(RED)$(BOLD)✗$(RESET)" && exit 1)
endef

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/minishell.h
	$(call compile_file,1,12,$<,$@)
$(OBJ_DIR)/lexer.o: $(SRC_DIR)/lexer.c $(INC_DIR)/minishell.h
	$(call compile_file,2,12,$<,$@)
$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.c $(INC_DIR)/minishell.h
	$(call compile_file,3,12,$<,$@)
$(OBJ_DIR)/expander.o: $(SRC_DIR)/expander.c $(INC_DIR)/minishell.h
	$(call compile_file,4,12,$<,$@)
$(OBJ_DIR)/quote_handler.o: $(SRC_DIR)/quote_handler.c $(INC_DIR)/minishell.h
	$(call compile_file,5,12,$<,$@)
$(OBJ_DIR)/validator.o: $(SRC_DIR)/validator.c $(INC_DIR)/minishell.h
	$(call compile_file,6,12,$<,$@)
$(OBJ_DIR)/error.o: $(SRC_DIR)/error.c $(INC_DIR)/minishell.h
	$(call compile_file,7,12,$<,$@)
$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c $(INC_DIR)/minishell.h
	$(call compile_file,8,12,$<,$@)
$(OBJ_DIR)/signals.o: $(SRC_DIR)/signals.c $(INC_DIR)/minishell.h
	$(call compile_file,9,12,$<,$@)
$(OBJ_DIR)/env.o: $(SRC_DIR)/env.c $(INC_DIR)/minishell.h
	$(call compile_file,10,12,$<,$@)
$(OBJ_DIR)/builtins.o: $(SRC_DIR)/builtins.c $(INC_DIR)/minishell.h
	$(call compile_file,11,12,$<,$@)
$(OBJ_DIR)/executor.o: $(SRC_DIR)/executor.c $(INC_DIR)/minishell.h
	$(call compile_file,12,12,$<,$@)

clean:
	@echo ""
	@echo "$(YELLOW)$(BOLD) Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)$(BOLD)✓$(RESET) $(GREEN)Object files removed$(RESET)"
	@echo ""

fclean: clean
	@echo "$(YELLOW)$(BOLD) Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)$(BOLD)✓$(RESET) $(GREEN)Executable removed$(RESET)"
	@echo ""
	@echo "$(CYAN)$(BOLD)╔════════════════════════════════════════╗$(RESET)"
	@echo "$(CYAN)$(BOLD)║   $(WHITE)✓ $(CYAN)Full clean completed!$(CYAN)   ║$(RESET)"
	@echo "$(CYAN)$(BOLD)╚════════════════════════════════════════╝$(RESET)"
	@echo ""

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re