#include "minishell.h"

// Various finders and helper functions.

// TODO Add 42 header

// Returns the line index number of PATH for the requested var string
// If not found, returns -1
int	find_env_var(char **envp, const char *var)
{
	size_t	len;
	int	i;

	len = 0;
	i = 0;
	len = ft_strlen(var);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		else if(ft_strncmp(envp[i], var, len) == 0 && envp[i][len] != '=')
			return(i);
		i++;
	}
	return (-1);
}

// Look through a parsed set of commands and see if it contains
// flow control parameters: | > < >> or <<
// They would be separate from commands either side.
// (locates, it is up to the outer loop to re-identify what the thing is)
// FIXME Segfault if cmdset (or any part of it) is null
int	find_flow_control(t_command *cmdset)
{
	int	i;

	i = 0;
	while (cmdset->argv[i])
	{
		if (ft_strncmp(cmdset->argv[i], ">>", 2) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<<", 2) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], ">", 1) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "<", 1) == 0)
			break ;
		else if (ft_strncmp(cmdset->argv[i], "|", 1) == 0)
			break ;
//        printf("nothing in %i", i);	// HACK Debugging only remove later
		i++;
	}
	if (i == cmdset->argc)
		i = -1;
	return (i);
}

// Debugging function to view the active parts of a command.
// If there is a list in the command, it prints all commands
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

// Moves the *posn pointer forward through a command line
// until it reaches a stop character (which is badly named because we
// use this to find a place to *start* parsing).
// The stop chars are null or any non-space character.
// (The quote styles are matched for in the main loop.)
void	goto_stop_char(const char *cmdline, int *posn)
{
	while ((cmdline[*posn] != '\0') && (cmdline[*posn] == ' '))
		(*posn)++;
}

// Return an executable path for cmd.
// NOTE cmd must be the name only, not any of its arguments.
// - split the pieces of PATH and add a trailing slash.
// - test the parts of path:
// -- does path + cmd = an executable?
// -- if YES we have our command: keep that and discard the rest.
// NOTE The return value of getenv("PATH") does not need to be freed
char	*search_in_path(char *cmd)
{
	char	**pathparts;
	char	*candidate;
	char	*slashed;
	char	*goodpath;
	int		i;

	i = 0;
	goodpath = NULL;
	pathparts = ft_split(getenv("PATH"), ':');
	while ((pathparts[i] != NULL) && (!goodpath))
	{
		slashed = ft_strjoin(pathparts[i], "/");
		candidate = ft_strjoin(slashed, cmd);
		if (access(candidate, X_OK) == 0)
			goodpath = ft_strdup(candidate);
		free (candidate);
		free(slashed);
		i++;
	}
	i = -1;
	while (pathparts[++i] != NULL)
		free(pathparts[i]);
	free(pathparts);
	return (goodpath);
}
