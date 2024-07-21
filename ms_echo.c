#include "minishell.h"

// TODO Add 42 header

// Minishell implementation of the echo command.
// Supports the -n option (NO newline after output)
// FIXME Too many lines, we can consolidate this.
// DONE Double check the echo / echon split and i values.
// FIXED These names were the wrong way round! ECHON should not have \n
// NOTE The output is correct but this setup is confusing.
void	ms_echo(t_command *cmd)
{
	int	i;

	if (cmd->builtin == ECHO)
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
	if (cmd->builtin == ECHON)
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
