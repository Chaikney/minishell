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

// FIXME Too many functions in file.

// Find the path part (after the redirect >>)
// Open a file of that name for writing.
// Set the output to that file.
// run the command in a child process
// close file when done.
// FIXME This will not work without mangling the argvs before sending to run_
// NOTE The main (only?) difference from redirect_output is the O_APPEND flag
// TODO This could be combined with redirect: just set file options differently.
void	run_in_child_append_output(t_command *cmd, char **envp)
{
	int	out_file;
	int	redirect_posn;

	redirect_posn = find_flow_control(cmd);
	out_file = open(cmd->argv[redirect_posn + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (out_file == -1)
		perror("Could not open output file");
	dup2(out_file, STDOUT_FILENO);
	close (out_file);
	run_in_child(cmd, envp);
}

// Find the path part (after the redirect >)
// Open a file of that name for writing.
// Set the output to that file.
// run the command in a child process
// close file when done.
// FIXME This will not work without mangling the argvs
// TODO Think about how to find the needed parts
// TODO Do we need to reset STDOUT afterwards, or is this going to be process-limited?
void	run_in_child_redirect_output(t_command *cmd, char **envp)
{
	int	out_file;
	int	redirect_posn;

	redirect_posn = find_flow_control(cmd);
	out_file = open(cmd->argv[redirect_posn + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_file == -1)
		perror("Could not open output file");
	dup2(out_file, STDOUT_FILENO);
	close (out_file);
	run_in_child(cmd, envp);	// FIXME This will not work without mangling argvs!
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// TODO Can I use the output from this to measure state?
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
		waitpid(child, &g_procstatus, 0);
		close(tube[0]);
		printf("process finished with code: %i", g_procstatus); // HACK for debugging
	}
}

// Simplest command runner.
// Launches one command in a child process and waits for it to complete.
// NOTE This is the one we use for simple commands.
// TODO do something with error status here.
void	run_in_child(t_command *cmd, char **envp)
{
	pid_t	child;
	int		ret_val;

	g_procstatus = 0;
	child = fork();
	if (child == -1)
		exit_and_free(NULL, -1, -1);
	if (child == 0)
		run_command(cmd, envp);
	else
	{
		ret_val = waitpid(child, &g_procstatus, 0);
		if (ret_val == -1)
			printf("error in child process");
		printf("process finished with code: %i", g_procstatus); // HACK for debugging
	}
}
