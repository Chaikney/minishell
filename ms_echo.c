#include "minishell.h"

// TODO Add 42 header

// Minishell implementation of the echo command.
// Supports the -n option
// FIXME Too many lines, we can consolidate this.
// TODO Double check the echo / echon split and i values.
void	ms_echo(t_command *cmd)
{
	int	i;

	if (cmd->builtin == ECHON)
	{
		i = 1;
		while (i < cmd->argc)
		{
			printf("%s", cmd->argv[i]);
			if (i < cmd->argc)
				printf(" ");
			i++;
		}
		printf("\n");
	}
	if (cmd->builtin == ECHO)
	{
		i = 2;
		while (i < cmd->argc)
		{
			printf("%s", cmd->argv[i]);
			if (i < cmd->argc - 1)
				printf(" ");
			i++;
		}
	}
	return ;
}
