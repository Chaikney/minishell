/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:03:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/20 12:43:53 by emedina-         ###   ########.fr       */
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
// FIXED <z> leads to double free segfault.
// TODO Would be good to check validity of o_path here...
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
// NOTE i_redir is passed as pointer to change for the next step in pipe.
// NOTE We need use last_status to not run a final_cmd if penultimate fails.
// ...cant use g-proc because it leaves us in an unrecoverable state.
// FIXME Too many lines in function
void	direct_complex_command(t_command *cmd, t_env *envt)
{
	int			o_redir;
	int			i_redir;
	int			last_status;

	last_status = 0;
	i_redir = determine_input(cmd);
	if (cmd->argv[0][0] != '<' && cmd->argv[0][1] != '<')
	{
		remove_cmd_parts(cmd, "<");
		while ((cmd->next != NULL) && (i_redir != -1))
		{
			last_status = run_in_pipe(cmd, &i_redir, envt);
			if (last_status == 0)
				cmd = cmd->next;
			else
				return ;
		}
		if ((last_status == 0) && (i_redir != -1))
		{
			o_redir = determine_output(cmd);
			remove_cmd_parts(cmd, ">");
			if (needs_to_fork(cmd) == 0)
				execute_builtin(cmd, envt);
			run_final_cmd(cmd, i_redir, o_redir, envt);
		}
	}
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// In child:
// - STDOUT made to point to the Write end of the pipe
// - the old reference (tube[0]) is closed
// - "redirect stdin to prevpipe"
// - run the command, everything will be cleared with that process exit
// discard extra reference
// In parent (i.e. the shell):
// - close write end of pipe
// - wait for the child, collect exit code for g_procstatus
// - Keep hold of the read end of this pipe for the next run.
// NOTE The input file pointer connects us to the previous command in pipe.
int	run_in_pipe(t_command *cmd, int *i_file, t_env *envt)
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
		close(tube[1]);
		dup2(*i_file, STDIN_FILENO);
		close(*i_file);
		run_command(cmd, envt);
	}
	else
	{
		close(tube[1]);
		waitpid(child, &g_procstatus, 0);
		*i_file = tube[0];
	}
	return (g_procstatus);
}

// Simplest command runner.
// Forks, sets up input and output for one child process
// and waits for it to complete.
// NOTE This is used with either simple commands or at the end of complex one.
// NOTE This *cannot* receive EXIT builtin
/// In child:
// - STDOUT made to point to file, if needed.
// - "redirect stdin to prevpipe"
// - run the command, everything will be cleared with that process exit
// discard extra reference
// In parent (i.e. the shell):
// - wait for the child, collect exit code for g_procstatus
// - close output reference IF it is not STDOUT
// - close input reference IF it is not STDIn
void	run_final_cmd(t_command *cmd, int i_file, int o_file, t_env *envt)
{
	pid_t		child;

	child = fork();
	if (child == -1)
		g_procstatus = errno;
	else if (child == 0)
	{
		if ((o_file >= 0) && (o_file != STDOUT_FILENO))
		{
			dup2(o_file, STDOUT_FILENO);
			close (o_file);
		}
		dup2(i_file, STDIN_FILENO);
		if (i_file >= 0)
			close(i_file);
		run_command(cmd, envt);
	}
	else
	{
		waitpid(child, &g_procstatus, 0);
		if ((o_file >= 0) && (o_file != STDOUT_FILENO))
			close(o_file);
		if ((i_file >= 0) && (i_file != STDIN_FILENO))
			close(i_file);
	}
}
