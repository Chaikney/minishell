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

// Wrap everything needed to work out where the output comes from,
// open the file with appropriate perms and return the fd to it.
// - Find the marker.
// - Set file perms for level (create or append)
// - find file name
// - check access to file, open it
// NOTE file mode is set to have NO EXEC permission so we don't...
// ...attempt to execute any file with the same name as a program in the wd
int	determine_output(t_command *cmd)
{
	int		i;
	int		o_fd;
	int		perms;
	char	*o_path;

	i = cmd->argc;
	o_fd = STDOUT_FILENO;
	perms = 0;
	while ((i-- > 0))
	{
		if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
		{
			perms = O_WRONLY | O_CREAT | O_TRUNC;
			if (ft_strncmp(cmd->argv[i], ">>", 2) == 0)
				perms = O_WRONLY | O_CREAT | O_APPEND;
			o_path = cmd->argv[i + 1];
			o_fd = open(o_path, perms, 0666);
			if (o_fd == -1)
			{
				g_procstatus = errno;
				perror("Could not open output file");
			}
		}
	}
	return (o_fd);
}

// When there is a control character present, guide it to the correct
// execution function(s)
// - Set up input redirection and remove / free those parts of cmd.
// - run not-final commands in a pipe, altering input pointer
// - setup output redirection and remove / free those parts of cmd
// - if the final command is a builtin, execute it directly.
// - otherwise pass it to be run in a fork
// NOTE i_redir is passed as pointer so it can be changed to the next step in pipe.
void	direct_complex_command(t_command *cmd, t_env *envt)
{
	int			o_redir;
	int			i_redir;

	i_redir = determine_input(cmd);
	remove_cmd_parts(cmd, "<");
	while (cmd->next != NULL)
	{
		run_in_pipe(cmd, &i_redir, envt);
		cmd = cmd->next;
	}
	o_redir = determine_output(cmd);
	remove_cmd_parts(cmd, ">");
	if (cmd->builtin != NONE)
		execute_builtin(cmd, envt);
	else
		run_final_cmd(cmd, i_redir, o_redir, envt);
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// NOTE The input file pointer connects us to the previous command in pipe.
// DONE Move builtin check to run_command to save lines?
//void	run_in_pipe(t_command *cmd, char **envp, int *i_file)
void	run_in_pipe(t_command *cmd, int *i_file, t_env *envt)
{
	pid_t	child;
	int		tube[2];

	if (pipe(tube) == -1)
		exit_failed_pipe(NULL, tube[0], tube[1], envt);
	child = fork();
	if (child == -1)
		exit_failed_pipe(NULL, tube[0], tube[1], envt);
	if (child == 0)
	{
		close(tube[0]);
		dup2(tube[1], STDOUT_FILENO);
		close(tube[1]);		// NOTE This reference to the file is not needed we use STDOUTs now
		// "redirect stdin to prevpipe"
		dup2(*i_file, STDIN_FILENO);// closes STDIN, uses its ref to point to i_file (last processes' pipe read end)
		close(*i_file);		// discard extra reference to the pipe read end.
		run_command(cmd, envt);	// asfter this all fds of the child are released.
	}
	else
	{
		close(tube[1]);	// Shell will not write to the pipe
		// NOTE: commenting out this line leads to endless loop or non-returning process
		waitpid(child, &g_procstatus, 0);
		*i_file = tube[0];	// Keep hold of the read end of this pipe for the next run.
		printf("process %s finished with code: %i\n", cmd->argv[0], g_procstatus); // HACK for debugging
	}
}

// Simplest command runner.
// Forks, sets up input and output for one child process
// and waits for it to complete.
// NOTE This is used with either simple commands or at the end of complex one.
// NOTE This *cannot* receive EXIT builtin
// FIXME run_final_cmd still has too many lines
void	run_final_cmd(t_command *cmd, int i_file, int o_file, t_env *envt)
{
	pid_t		child;
	extern int	g_procstatus;

	child = fork();
	if (child == -1)
		g_procstatus = errno;
	else if (child == 0)
	{
		if ((o_file >= 0) && (o_file != STDOUT_FILENO))	// Detect whether redirection is *needed*
		{
			dup2(o_file, STDOUT_FILENO); // Although if these are equal, nothing happens.
			close (o_file);	// if o_file was set as STDOUT before, closing would be dangerous as they are the *same* fd, not like dup (new fd pointing to same resource)
		}
		dup2(i_file, STDIN_FILENO);	// Expect this to be the same as with pipe
		if (i_file >= 0)
			close(i_file);
		run_command(cmd, envt);	// after this all fds of the child are released.
	}
	else
	{
		waitpid(child, &g_procstatus, 0);
		if ((o_file >= 0) && (o_file != STDOUT_FILENO))	// Make sure we don't close STDOUT if no redirection was given.
			close(o_file);	// This makes sense if we are writing to a file *and* have access to it.
		if ((i_file >= 0) && (i_file != STDIN_FILENO))
			close(i_file);
		printf("process finished with code: %i\n", g_procstatus); // HACK for debugging
	}
}
