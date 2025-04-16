#include "minishell.h"

void print_env_list(t_env_list *env_list)
{
	t_env_node *curr;

	curr = env_list->head;
	while (curr)
	{
		printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}

int main(int argc, char **argv, char **env)
{
	char *prompt;
	t_token_list *tokens;
	t_cmd_list *cmd_list;
	t_env_list *env_list;

	(void)argc;
	(void)argv;
	(void)env;

	DEBUG_INFO("Minishell starting up");
	gc_init();
	env_list = get_envs(env);

	while (1)
	{
		prompt = readline("minishell > ");
		if (!prompt)
		{
			DEBUG_WARN("Received NULL from readline, exiting");
			break;
		}

		// Add to history if not empty
		// if (ft_strlen(prompt) > 0)
		// 	add_history(prompt);

		DEBUG_INFO("User input: [%s]", prompt);
		gc_add(prompt);  // adds the prompt string to the garbage collector

		if (ft_strlen(prompt) == 0)
		{
			DEBUG_INFO("Empty command, continuing");
			continue;
		}

		// Step 1: Lexical analysis (tokenization)
		tokens = lexer(prompt);
		if (!tokens)
		{
			DEBUG_ERROR("Lexer returned NULL tokens");
			continue;
		}
		DEBUG_INFO("Lexer complete, tokens created");
		print_tokens(tokens);

		if (tokens->syntax_error)
		{
			DEBUG_ERROR("Syntax errors detected during lexical analysis, skipping command");
			continue;
		}
		// Step 2: Token segmentation (splitting tokens into segments based on quotes)
		tokens = process_token_list(tokens);
		if (!tokens)
		{
			DEBUG_ERROR("Token segmentation failed");
			continue;
		}
		DEBUG_INFO("Token segmentation complete");

		// Step 3: Variable expansion
		segment_tokens(tokens, env_list);
		DEBUG_INFO("Variable expansion complete");
		print_tokens(tokens);  // Print tokens after expansion

		// Step 4: Parsing (convert tokens to commands)
		DEBUG_INFO("Starting parser");
		cmd_list = token_to_cmd(tokens);
		if (!cmd_list)
		{
			DEBUG_ERROR("Parser returned NULL command list");
			continue;
		}

		// Print and execute commands
		print_cmd_list(cmd_list);
		DEBUG_INFO("Ready for execution (not implemented yet)");
		// execute(cmd_list);

		if (ft_strcmp(prompt, "exit") == 0)
		{
			DEBUG_INFO("Exit command detected, terminating shell");
			break;
		}

		if (!ft_strcmp(prompt, "env"))
		{
			print_env_list(env_list);
			continue;
		}
	}

	DEBUG_INFO("Cleaning up and exiting");
	clean_up();
	return (0);
}

// dorker valgrind --leak-check=full  --show-leak-kinds=all --suppressions=sub.sub  ./minishell