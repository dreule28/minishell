/* debug.h - Add this to your project */
#ifndef DEBUG_H
# define DEBUG_H

# include "minishell.h"
# include <stdio.h>

/* Debug levels */
# define DBG_LEVEL_NONE  0
# define DBG_LEVEL_ERROR 1
# define DBG_LEVEL_WARN  2
# define DBG_LEVEL_INFO  3
# define DBG_LEVEL_TRACE 4

/* Set your desired debug level here */
# define CURRENT_DEBUG_LEVEL DBG_LEVEL_INFO

/* Color codes for terminal output */
# define COLOR_RED     "\x1b[31m"
# define COLOR_GREEN   "\x1b[32m"
# define COLOR_YELLOW  "\x1b[33m"
# define COLOR_BLUE    "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN    "\x1b[36m"
# define COLOR_RESET   "\x1b[0m"

/* If token types aren't defined in minishell.h, define them here */
# ifndef TK_WORD
enum token_types {
    DB_WORD = 1,
    DB_PIPE,
    DB_REDIR_IN,
    DB_REDIR_OUT,
    DB_REDIR_APPEND,
    DB_REDIR_HEREDOC,
    DB_BUILTIN
};
# endif

// /* If command types aren't defined in minishell.h, define them here */
// # ifndef CMD
// enum cmd_types {
//     CMD = 1,
//     PIPE,
//     BUILTIN
// };
// # endif

/* Debug macros */
# if CURRENT_DEBUG_LEVEL >= DBG_LEVEL_ERROR
#  define DEBUG_ERROR(fmt, ...) fprintf(stderr, COLOR_RED "[ERROR] " COLOR_RESET fmt "\n", ##__VA_ARGS__)
# else
#  define DEBUG_ERROR(fmt, ...)
# endif

# if CURRENT_DEBUG_LEVEL >= DBG_LEVEL_WARN
#  define DEBUG_WARN(fmt, ...) fprintf(stderr, COLOR_YELLOW "[WARN] " COLOR_RESET fmt "\n", ##__VA_ARGS__)
# else
#  define DEBUG_WARN(fmt, ...)
# endif

# if CURRENT_DEBUG_LEVEL >= DBG_LEVEL_INFO
#  define DEBUG_INFO(fmt, ...) fprintf(stderr, COLOR_BLUE "[INFO] " COLOR_RESET fmt "\n", ##__VA_ARGS__)
# else
#  define DEBUG_INFO(fmt, ...)
# endif

# if CURRENT_DEBUG_LEVEL >= DBG_LEVEL_TRACE
#  define DEBUG_TRACE(fmt, ...) fprintf(stderr, COLOR_CYAN "[TRACE] " COLOR_RESET fmt "\n", ##__VA_ARGS__)
# else
#  define DEBUG_TRACE(fmt, ...)
# endif

/* Function prototypes */
void validate_cmd_list(t_cmd_list *cmd_list);
void print_cmd_list(t_cmd_list *cmd_list);
void print_tokens(t_token_list *list);

#endif /* DEBUG_H */