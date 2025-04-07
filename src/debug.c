/* debug.c - Implementation of debug utilities */
#include "debug.h"

void validate_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *curr;
    ssize_t count = 0;

    if (!cmd_list)
    {
        DEBUG_ERROR("Command list is NULL");
        return;
    }

    DEBUG_INFO("Validating command list structure...");

    /* Check for head/tail consistency */
    if (!cmd_list->head && cmd_list->tail)
    {
        DEBUG_ERROR("Invalid list: NULL head but non-NULL tail");
    }

    if (cmd_list->head && !cmd_list->tail)
    {
        DEBUG_ERROR("Invalid list: head exists but tail is NULL");
    }

    /* Traverse the list and count nodes */
    curr = cmd_list->head;
    while (curr)
    {
        count++;

        /* Check for cmd array validity */
        if (curr->cmd_type != PIPE && !curr->cmd && curr != cmd_list->tail)
        {
            DEBUG_ERROR("Non-pipe command node %zu has NULL cmd array", count);
        }

        /* Check for file list validity */
        if (!curr->files)
        {
            DEBUG_ERROR("Command node %zu has NULL files list", count);
        }
        else if (curr->files->size > 0 && !curr->files->head)
        {
            DEBUG_ERROR("Command node %zu has non-zero file size but NULL head", count);
        }

        /* Check if we've reached the tail */
        if (curr == cmd_list->tail && curr->next)
        {
            DEBUG_ERROR("Tail node points to another node!");
        }

        curr = curr->next;
    }

    /* Compare counted size with stored size */
    if (count != cmd_list->size)
    {
        DEBUG_ERROR("Size mismatch: counted %zu nodes but size field is %zu",
                    count, cmd_list->size);
    }
    else
    {
        DEBUG_INFO("Command list structure validation successful (%zu nodes)", count);
    }
}

void print_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_node *curr_cmd;
    t_file_node *curr_file;
    int cmd_index = 0;

    if (!cmd_list)
    {
        DEBUG_ERROR("Cannot print NULL command list");
        return;
    }

    printf("\n%s=== Command List Debug ===%s\n", COLOR_GREEN, COLOR_RESET);
    printf("Total commands: %zu\n", cmd_list->size);

    /* Validate the list structure before printing */
    validate_cmd_list(cmd_list);

    curr_cmd = cmd_list->head;
    while (curr_cmd)
    {
        printf("\n%sCommand Node %d:%s\n", COLOR_CYAN, cmd_index++, COLOR_RESET);

        /* Print command type */
        if (curr_cmd->cmd_type == PIPE)
            printf("Type: %sPIPE%s\n", COLOR_MAGENTA, COLOR_RESET);
        else if (curr_cmd->cmd_type == BUILTIN)
            printf("Type: %sBUILTIN%s\n", COLOR_YELLOW, COLOR_RESET);
        else
            printf("Type: %sCMD%s\n", COLOR_BLUE, COLOR_RESET);

        /* Print command arguments */
        if (curr_cmd->cmd)
        {
            printf("Arguments: ");
            for (int i = 0; curr_cmd->cmd[i]; i++)
                printf("[%s%s%s] ", COLOR_GREEN, curr_cmd->cmd[i], COLOR_RESET);
            printf("\n");
        }
        else
        {
            printf("Arguments: %sNONE%s\n", COLOR_RED, COLOR_RESET);
        }

        /* Print files */
        if (curr_cmd->files && curr_cmd->files->size > 0)
        {
            printf("Files (%zu):\n", curr_cmd->files->size);
            curr_file = curr_cmd->files->head;
            while (curr_file)
            {
                char *redir_type_str;
                switch (curr_file->redir_type)
                {
                    case TK_INFILE:
                        redir_type_str = "IN (<)";
                        break;
                    case TK_OUTFILE:
                        redir_type_str = "OUT (>)";
                        break;
                    case TK_APPEND:
                        redir_type_str = "APPEND (>>)";
                        break;
                    case TK_HEREDOC:
                        redir_type_str = "HEREDOC (<<)";
                        break;
                    default:
                        redir_type_str = "UNKNOWN";
                }

                printf("  - %s%s%s (type: %s)\n",
                       COLOR_BLUE, curr_file->filename, COLOR_RESET, redir_type_str);
                curr_file = curr_file->next;
            }
        }
        else
        {
            printf("Files: %sNONE%s\n", COLOR_YELLOW, COLOR_RESET);
        }

        curr_cmd = curr_cmd->next;
    }
    printf("\n%s========================%s\n\n", COLOR_GREEN, COLOR_RESET);
}

void print_tokens(t_token_list *list)
{
    t_token *current;
    int i = 0;

    if (!list)
    {
        DEBUG_ERROR("Cannot print NULL token list");
        return;
    }

    DEBUG_INFO("Printing tokens. List size: %zd", list->size);
    if (list->input)
        DEBUG_INFO("Input in list: [%s]", list->input);

    printf("\n%s=== Token List ===%s\n", COLOR_YELLOW, COLOR_RESET);

    current = list->head;
    while (current)
    {
        char *token_type;

        /* Convert token type to string for better readability */
        switch (current->token)
        {
            case TK_WORD:
                token_type = "WORD";
                break;
            case TK_PIPE:
                token_type = "PIPE";
                break;
            case TK_INFILE:
                token_type = "INFILE";
                break;
            case TK_OUTFILE:
                token_type = "OUTFILE";
                break;
            case TK_APPEND:
                token_type = "APPEND";
                break;
            case TK_HEREDOC:
                token_type = "HEREDOC";
                break;
            case TK_BUILTIN:
                token_type = "BUILTIN";
                break;
			case D_QUOTES:
				token_type = "D_QUOTES";
				break;
			case S_QUOTES:
				token_type = "S_QUOTES";
				break;
            default:
                token_type = "UNKNOWN";
        }

        printf("Token %d: [%s%s%s] (Type: %s)\n",
               i++, COLOR_GREEN, current->value, COLOR_RESET, token_type);
        current = current->next;
    }
    printf("%s=====================%s\n\n", COLOR_YELLOW, COLOR_RESET);
}