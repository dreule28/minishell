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

PARSING_FILES :=
PARSING := $(addprefix parser/parser/, $(PARSING_FILES))

LEXER_FILES := lexer.c lexer_utils.c lists_and_nodes.c
LEXER := $(addprefix parser/lexer/, $(LEXER_FILES))

EXECUTOR_FILES := 	execute.c \
					file_check.c \
					testing.c
EXECUTOR := $(addprefix executor/, $(EXECUTOR_FILES))

GC_FILES := ft_malloc.c \
			gc_utils.c 
GC := $(addprefix gc/, $(GC_FILES))

SHELL_FILES := shell.c
SHELL_ := $(addprefix shell/, $(SHELL_FILES))

SRC_FILES := main.c
SRC := $(addprefix src/, $(SRC_FILES) $(PARSING) $(EXECUTOR) $(SHELL_)) $(GC)


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

.PHONY: all clean fclean re