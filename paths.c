/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:03:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/25 19:14:31 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// When there is a control character present, guide it to the correct
// execution function(s)
// - Set up input redirection and remove / free those parts of cmd.
// - run not-final commands in a pipe, altering input pointer
// - setup output redirection and remove / free those parts of cmd
// - if the final command is a builtin, execute it directly.
// - otherwise pass it to be run in a fork
// NOTE i_redir is passed as pointer to change for the next step in pipe.
// NOTE i_redir == -1 is triggered if we couldn't open the input file path.
// NOTE We need use last_status to not run a final_cmd if penultimate fails.
// ...cant use g-proc because it leaves us in an unrecoverable state.
// DONE Changed the last_status check to something with SIGPIPE
// TODO Is input from any point in the pipe sensible?
// If so need more determine input calls......
void	direct_complex_command(t_command *cmd, t_env *envt)
{
	int	o_redir;
	int	i_redir;
	int	last_status;

	last_status = 0;
	i_redir = determine_input(cmd);
	while ((cmd->next != NULL) && (i_redir != -1))
	{
		o_redir = determine_output(cmd);
		last_status = run_in_pipe(cmd, &i_redir, o_redir, envt);
		if (last_status != EPIPE)
			cmd = cmd->next;
		else
			return ;
	}
	if ((last_status != EPIPE) && (i_redir != -1))
	{
		o_redir = determine_output(cmd);
		if (needs_to_fork(cmd) == 0)
			execute_builtin(cmd, envt);
		else
			run_final_cmd(cmd, i_redir, o_redir, envt);
	}
}

// Wrap and run the necessary for a command running in a pipe
// The pipe has been sucessfully set up before calling this.
// NOTE unsure whether close(tube[1]) should been fully removed.
// ...what was the use case it failed on?
//	close(tube[1]);
void	launch_child_cmd(int tube[2], t_command *cmd, int *i_file, t_env *envt)
{
	close(tube[0]);
	dup2(*i_file, STDIN_FILENO);
	close(*i_file);
	run_command(cmd, envt);
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
// DONE? Make output_redir relevant in run_in_pipe
// - pass o_redir as o_file - this will be an open file or STDOUT or -1
// - change the child process dup2 calls
// NOTE Does the output file need to be opened? Or closed?
// it was opened in determine_output.
// If the output file is closed before waitpid call, likely to break .
// IF there is output redirection then
// - *i_file has to be reset to STDIN
// (Otherwise we use the read end of the pipe (tube[0]) as before)
// NOTE mkdir i_exist | ls does not run ls
// This is because the error code terminates the run.
int	run_in_pipe(t_command *cmd, int *i_file, int o_file, t_env *envt)
{
	pid_t	child;
	int		tube[2];

	if (pipe(tube) == -1)
		exit_failed_pipe(NULL, tube[0], tube[1], envt);
	child = fork();
	if (child == -1)
		exit_failed_pipe(NULL, tube[0], tube[1], envt);
	else if (child == 0)
	{
		wire_up_output(o_file, tube);
		launch_child_cmd(tube, cmd, i_file, envt);
	}
	else
	{
		close(tube[1]);
		waitpid(child, &g_procstatus, 0);
		if (o_file == STDOUT_FILENO)
			*i_file = tube[0];
		else
			*i_file = STDIN_FILENO;
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
		wire_up_output(o_file, NULL);
		dup2(i_file, STDIN_FILENO);
		run_command(cmd, envt);
		if (i_file >= 0)
			close(i_file);
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
