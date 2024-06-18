/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:03:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/05/31 16:03:42 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return an executable path for cmd.
// NOTE cmd must be the name only, not any of its arguments.
// - split the pieces of PATH and add a trailing slash.
// - test the parts of path:
// -- does path + cmd = an executable?
// -- if YES we have our command: keep that and discard the rest.
// NOTE The return value of getenv("PATH") does not need to be freed
char	*find_command(char *cmd)
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

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// TODO We can make this work with & / background
// TODO How would this work with different input / output?
//
void	run_in_child_with_pipe(t_command *cmd, char **envp)
{
	pid_t	child;
	int		tube[2];

	if (pipe(tube) == -1)
		exit_and_free(NULL, tube[0], tube[1]);
	child = fork();
	if (child == -1)
		exit_and_free(NULL, tube[0], tube[1]);
	if (child == 0)
	{
		close(tube[0]);
		dup2(tube[1], STDOUT_FILENO);
		run_command(cmd, envp);
		close(tube[1]);
	}
	else
	{
		close(tube[1]);
		dup2(tube[0], STDIN_FILENO);
		waitpid(child, 0, 0);
		close(tube[0]);
	}
}

// Simplest command runner.
// Launches one command in a child process and waits for it to complete.
void	run_in_child(t_command *cmd, char **envp)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		exit_and_free(NULL, -1, -1);
	if (child == 0)
		run_command(cmd, envp);
	else
		waitpid(child, 0, 0);
}
