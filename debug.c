#include "minishell.h"

// TODO Add 42 header
// TODO Remove file and refs before submission

// Functions that are only needed to help with debugging

// Debugging function to view the active parts of a command.
// If there is a list in the command, it prints all commands
// FIXED? invalid read can be triggered here from split pipe
void	print_cmd_parts(t_command *cmd)
{
	int	i;

	while (cmd->next != NULL)
	{
		i = 0;
		printf("\nExamining command with argc of %i:\n", cmd->argc);
		while (i < cmd->argc)
		{
			printf("Index: %i\t%s\n", i, cmd->argv[i]);
			i++;
		}
		cmd = cmd->next;
	}
	i = 0;
	printf("\nExamining command with argc of %i:\n", cmd->argc);
	while (i < cmd->argc)
	{
		printf("Index: %i\t%s\n", i, cmd->argv[i]);
		i++;
	}
}

// debugging aid: print a list of strings.
void	print_tokens(char **arr)
{
	int	i;

	i = 0;
	printf("\nAnalysing token list.");
	while ((arr[i]) && (i < MAXARGS))
	{
		printf("\ntoken %i:\t%s", i, arr[i]);
		i++;
	}
}
