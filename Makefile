NAME = minishell
CC = cc
RM = rm -rf

################################################################################
###############                  DIRECTORIES                      ##############
################################################################################

OBJ_DIR = _obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INC_DIRS := Includes $(LIBFT_DIR)
SRC_DIRS := src src/parser src/executor
GC_DIRS  := gc
SHELL_DIRS := shell
HEADERS = -I $(LIBFT_DIR)/include -I

vpath %.h $(INC_DIRS)
vpath %.c $(SRC_DIRS) $(GC_DIRS) $(SHELL_DIRS)

################################################################################
###############                  SOURCE FILES                     ##############
################################################################################

PARSING_FILES := envs.c lists_and_nodes_parser.c token_to_cmd.c ttc_utils.c
PARSING := $(addprefix parser/parser/, $(PARSING_FILES))

LEXER_FILES := check_actions.c lexer_utils.c lexer.c lists_and_nodes.c expansion.c expansion_utils.c segments.c lexer_utils2.c
LEXER := $(addprefix parser/lexer/, $(LEXER_FILES))

EXECUTOR_FILES := execute.c debug_utils.c
EXECUTOR := $(addprefix executor/, $(EXECUTOR_FILES))

BUILTINS_FILES := builtins.c env_builtins.c builtins_utils.c  env_builtins_utils.c export_utils.c single_builtin_execution.c cd_utils.c more_cd_utils.c unset_utils.c
BUILTINS := $(addprefix executor/builtins/, $(BUILTINS_FILES))

PROCESS_FILES := env_functions.c pipe_utils.c process_utils.c processes.c env_utils.c
PROCESS := $(addprefix executor/process/, $(PROCESS_FILES))

REDIRECTING_FILES := io_redir_loops.c io_redir_utils.c io_redir.c here_doc_utils.c
REDIRECTING := $(addprefix executor/redirecting/, $(REDIRECTING_FILES))

GC_FILES := ft_malloc.c gc_utils.c gc_libft_functions.c gc_itoa.c
GC := $(addprefix gc/, $(GC_FILES))

SRC_FILES := main.c signals.c exit_code.c
SRC := $(addprefix src/, $(SRC_FILES) $(PARSING) $(LEXER) $(EXECUTOR) $(BUILTINS) $(PROCESS) $(REDIRECTING)) $(GC)

################################################################################
###############               OBJECT FILES & RULES                ##############
################################################################################

OBJS := $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

# Compilation flags and linking options
CFLAGS := -Wall -Wextra -Werror -g -IIncludes -Ilibft -MMD -MP $(addprefix -I, $(INC_DIRS))
LDFLAGS := -Llibft -lft  -lreadline -lncurses
CFLAGS_SAN := $(CFLAGS) -fsanitize=address -g
LDFLAGS_SAN := $(LDFLAGS) -fsanitize=address

# Color definitions for better terminal output
COLOR_RESET = \033[0m
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_CYAN = \033[0;36m

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(COLOR_GREEN)Creating $(NAME)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) $(LDFLAGS)
	@echo "$(COLOR_GREEN)Successful Compilation of $(NAME)$(COLOR_RESET)"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@echo "$(COLOR_GREEN)Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "$(COLOR_GREEN)Compiling libft..$(COLOR_RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	@echo "$(COLOR_GREEN)Cleaning object files...$(COLOR_RESET)"
	@$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(COLOR_GREEN)Removing $(NAME)...$(COLOR_RESET)"
	@$(RM) $(OBJ_DIR)
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(COLOR_GREEN)Removed files successfully$(COLOR_RESET)"

re: fclean all

help:
	@echo "Available targets:"
	@echo "  all      - Build minishell (default)"
	@echo "  clean    - Remove object files"
	@echo "  fclean   - Remove all generated files"
	@echo "  re       - Rebuild minishell"

.PHONY: all clean fclean re help