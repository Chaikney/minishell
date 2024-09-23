/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:46:34 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/20 10:58:39 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return 1 if the words match
// NOTE s_len +1 because it includes the newline character
static int	word_match(char *stop, char *line)
{
	int	l_len;
	int	s_len;

	if (!line)
		return (1);
	l_len = ft_strlen(line);
	s_len = ft_strlen(stop);
	if (ft_strncmp(stop, line, s_len) == 0)
		if (l_len == (s_len + 1))
			return (1);
	return (0);
}

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
// NOTE read end of pipe is not needed by GNL
int	stopword_input(t_command *cmd, int fd[2], int posn)
{
	int		reader;
	char	*line;

	reader = fork();
	if (reader == 0)
	{
		close(fd[0]);
		while (1)
		{
			line = get_next_line(STDIN_FILENO);
			if (word_match(cmd->argv[posn + 1], line) == 1)
			{
				free (line);
				exit_successful_pipe(cmd);
			}
			write(fd[1], line, ft_strlen(line));
			free (line);
		}
	}
	else
	{
		close(fd[1]);
		waitpid(reader, 0, 0);
	}
	return (fd[0]);
}

// Receive a command and the type of redirection needed.
// Return a file descriptor to access that input.
// Return -1 and set g_procstatus if fd can't be accessed.
// NOTE Expected behaviour:
// If opening the file fails,
// bash quits with error and doesn’t run the command.
// If it succeeds, bash uses the file descriptor
// of the opened file as the stdin file descriptor for the command.
// NOTE If a file opened for input does not exist, do not execute the command.
int	setup_input(t_command *cmd, int i_lvl, int posn)
{
	char	*i_path;
	int		i_file;
	int		fd[2];

	if (i_lvl == 2)
	{
		pipe(fd);
		i_file = stopword_input(cmd, fd, posn);
	}
	else if (i_lvl == 1)
	{
		i_path = cmd->argv[posn + 1];
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
// 1 - input from file (< infile grep a1)
// 2 - input from STDIN with stop word (cmd << stop_word)
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
			i_redir = 1;
			if (ft_strncmp(cmd->argv[i], "<<", 2) == 0)
				i_redir = 2;
			i_fd = setup_input(cmd, i_redir, i);
		}
		i++;
	}
	remove_cmd_parts(cmd, "<");
	return (i_fd);
}
