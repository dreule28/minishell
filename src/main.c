#include "minishell.h"

int	get_exit_status(int new_code, int set)
{
	static int	status = 0;

	if (set)
		status = new_code;
	return (status);
}

// Helper function to print environment list contents
void print_env_list(t_env_list *list)
{
    t_env_node *current;
    int i = 0;

    if (!list)
    {
        DEBUG_ERROR("Cannot print NULL env list");
        return;
    }

    printf("\n%s=== Environment List ===%s\n", COLOR_GREEN, COLOR_RESET);
    printf("Total environment variables: %zu\n", list->size);

    current = list->head;
    while (current)
    {
        printf("ENV %d: [%s%s%s=%s%s%s]\n",
               i++, COLOR_CYAN, current->type, COLOR_RESET,
               COLOR_YELLOW, current->value, COLOR_RESET);
        current = current->next;
    }
    printf("%s=====================%s\n\n", COLOR_GREEN, COLOR_RESET);
}

int main(int argc, char **argv, char **env)
{
    char *prompt;
    t_token_list *tk_list;
    t_cmd_list *cmd_list;
    t_env_list *env_list;
    (void)argc;
    (void)argv;


    gc_init();
    env_list = get_envs(env);
	start_signals();
    // DEBUG_INFO("Environment list initialized");
    // print_env_list(env_list);

    while (1)
    {
        prompt = readline("2, 3 Years dagestan forget > ");
		if (!prompt)
		{
			ft_putstr_fd("exit\n", 2);
			clean_up();
			exit(0);
		}
		if (*prompt == '\0')
			continue;
			// break;
        if (prompt)
        {
            gc_add(prompt);
            // DEBUG_INFO("Processing input: [%s]", prompt);

			if (ft_strlen(prompt) > 0)
				add_history(prompt);
            tk_list = lexer(prompt);
            DEBUG_INFO("Lexer completed");
            print_tokens(tk_list);

            tk_list = process_token_list(tk_list);
            DEBUG_INFO("Token processing completed");
            print_tokens(tk_list);

			segment_tokens(tk_list, env_list);
			DEBUG_INFO("Variable expansion complete");
            cmd_list = token_to_cmd(tk_list);
			// DEBUG_INFO("REDIRTYPE: %d\n",cmd_list->tail->files->head->redir_type);
            // DEBUG_INFO("Command list generation completed");
            print_cmd_list(cmd_list);

            if (ft_strcmp(prompt, "exit") == 0)
                break;

            // DEBUG_INFO("Executing command");
			// DEBUG_INFO("env list : %s\n", env_list->head->type);
			
            execute(env_list, cmd_list);
            delete_tmp_files("tmp");
			// check_fds();
        }
    }
    clean_up();
    return (0);
}

// dorker valgrind --leak-check=full  --show-leak-kinds=all --suppressions=sub.sub --track-fds=yes ./minishell