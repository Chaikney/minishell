#include "minishell.h"

// TODO Add 42 header

// Builtin ENV command.
// TODO env should not display empty variables, unlike export.
// TODO Ensure bash-compatibility
void	ms_env(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
	return ;
}
