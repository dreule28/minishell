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

LEXER_FILES := check_actions.c lexer_utils.c lexer.c lists_and_nodes.c expansion.c expansion_utils.c segments.c
LEXER := $(addprefix parser/lexer/, $(LEXER_FILES))

EXECUTOR_FILES := execute.c debug_utils.c
EXECUTOR := $(addprefix executor/, $(EXECUTOR_FILES))

BUILTINS_FILES := builtins.c env_builtins.c builtins_utils.c
BUILTINS := $(addprefix executor/builtins/, $(BUILTINS_FILES))

PROCESS_FILES := env_functions.c pipe_utils.c process_utils.c processes.c
PROCESS := $(addprefix executor/process/, $(PROCESS_FILES))

REDIRECTING_FILES := io_redir_loops.c io_redir_utils.c io_redir.c here_doc_utils.c
REDIRECTING := $(addprefix executor/redirecting/, $(REDIRECTING_FILES))

GC_FILES := ft_malloc.c gc_utils.c gc_libft_functions.c gc_itoa.c
GC := $(addprefix gc/, $(GC_FILES))

SRC_FILES := main.c debug.c signals.c
SRC := $(addprefix src/, $(SRC_FILES) $(PARSING) $(LEXER) $(EXECUTOR) $(BUILTINS) $(PROCESS) $(REDIRECTING) $(SHELL_)) $(GC)

################################################################################
###############               OBJECT FILES & RULES                ##############
################################################################################

OBJS := $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

# Compilation flags and linking options
CFLAGS := -Wall -Wextra -Werror -g -IIncludes -Ilibft -MMD -MP $(addprefix -I, $(INC_DIRS))
LDFLAGS := -Llibft -lft  -lreadline -lncurses
CFLAGS_SAN := $(CFLAGS) -fsanitize=address -g
LDFLAGS_SAN := $(LDFLAGS) -fsanitize=address

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "\033[0;32mCreating $(NAME)...\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) $(LDFLAGS)
	@echo "\033[0;32mSuccessful Compilation of $(NAME)\033[0m"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@echo "\033[0;32mCompiling $<...\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "\033[0;32mCompiling libft..\033[0m"
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	@echo "\033[0;32mCleaning object files...\033[0m"
	@$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\033[0;32mRemoving $(NAME)...\033[0m"
	@$(RM) $(OBJ_DIR)
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "\033[0;32mRemoved files succesfully \033[0m"

re: fclean all

help:
	@echo "Available targets:"
	@echo "  all      - Build minishell (default)"
	@echo "  clean    - Remove object files"
	@echo "  fclean   - Remove all generated files"
	@echo "  re       - Rebuild minishell"`

.PHONY: all clean fclean re help