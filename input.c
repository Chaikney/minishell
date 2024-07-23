/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:46:34 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/18 19:46:37 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Implmentation of << "here_doc" or stop word based input
// - open a pipe
// - open a child process
// - use the child process to get input into STDOUT
// -- calls GNL and writes that output to the pipe
// -- until it gets NULL back
// -- then the process EXITs
// - parent process reads from the other end of the
// -- close the write end as uneeded
// -- dups stdin to the read end of the pipe
// -- waits for reader to finish
// - returns the read end of the parent's pipe
// TODO	stopword = cmd->argv[1] is a hardcoded assumption, is it safe?
// TODO Is there any cleanup to do in reader fork? child copy of cmd?
// TODO Should we set g_procstatus here?
// FIXME Protect against if first call to line returns null
// 		reader process never would never end!
int	stopword_input(t_command *cmd)
{
	int		fd[2];
	int		reader;
	char	*line;
	char	*stopword;

	pipe(fd);
	reader = fork();
	if (reader == 0)
	{
		close(fd[0]);	// NOTE read end of pipe is not needed by GNL
		stopword = cmd->argv[1];
//		printf("stopword is: %s", stopword);	// HACK for debugging
		line = get_next_line(STDIN_FILENO);
		while (line)
		{
			if (ft_strncmp(stopword, line, ft_strlen(stopword)) == 0)
			{
				free (line);
				exit_pipe(cmd);
			}
			write(fd[1], line, ft_strlen(line));
			free (line);
			line = get_next_line(STDIN_FILENO);
		}
		free (line);	// We have written it and so it is no longer needed.?
	}
	else
	{
		close(fd[1]);
		waitpid(reader, 0, 0);
	}
	return (fd[0]);
}

// NOTE That is in the format: cmd << stop_word
// NOTE Input redir in format:  < infile grep a1
// If opening the file fails,
// bash quits with error and doesn’t run the command.
// If it succeeds, bash uses the file descriptor
// of the opened file as the stdin file descriptor for the command.
// NOTE i_file values of -1 are sometimes attempted to be closed
// (In cases of BUILTIN commands)
// ...better they should be caught instead
int	setup_input(t_command *cmd, int i_lvl)
{
	char	*i_path;
	int		i_file;

	if (i_lvl == 2)
	{
		i_file = stopword_input(cmd);
	}
	else if (i_lvl == 1)
	{
		i_path = cmd->argv[1];
		i_file = open(i_path, O_RDONLY);
		if (i_file == -1)
			g_procstatus = errno;
	}
	else
		i_file = STDIN_FILENO;
	return (i_file);
}

// Wrap everything needed to work out where the input comes from,
// and return the fd to it.
// Flags for input redir:
// 0 - no special input
// 1 - input from file
// 2 - input from STDIN with stop word.
int	determine_input(t_command *cmd)
{
	int	i;
	int	i_fd;
	int	i_redir;

	i = 0;
	i_redir = 0;
	i_fd = STDIN_FILENO;
	while (i < cmd->argc - 1)
	{
		if (ft_strncmp(cmd->argv[i], "<", 1) == 0)
		{
//				print_cmd_parts(cmd);	// HACK for debugging
			i_redir = 1;
			if (ft_strncmp(cmd->argv[i], "<<", 2) == 0)
				i_redir = 2;
			i_fd = setup_input(cmd, i_redir);
		}
		i++;
	}
//	printf("Checked input for '%s'. fd will be: %i\n", cmd->argv[0], i_fd);
	return (i_fd);
}
