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
// NOTE The file mode has no exec permission to avoid that...
// ...a file with the same name as a program in the wd is attempted to be run
// because it has the X bit set!
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
// DONE Shorter (but still descriptive!) name needed.
// DONE Function will need to be shorter once it is working
// DONE Must be able to handle BUILTINS here as well.
// FIXED Naive addition of builtins does not work.
// FIXED < test | rev | rev no longer triggers a crash (form is invalid!)
// FIXME Too many lines in this function
void	direct_complex_command(t_command *cmd, char **envp)
{
	int			num_pipes;
	int			i;
	int			o_redir;
	int			i_redir;
	t_command	*cmdlist;

	num_pipes = 0;
	i = 0;
	while (i < cmd->argc)
		if (ft_strncmp(cmd->argv[i++], "|", 1) == 0)
			num_pipes++;
	i_redir = determine_input(cmd);
	o_redir = determine_output(cmd);
	remove_cmd_parts(cmd, ">");
	remove_cmd_parts(cmd, "<");
	if (num_pipes > 0)
	{
		cmdlist = make_cmd_list(cmd, num_pipes);
		while (cmdlist->next != NULL)
		{
			run_in_pipe(cmdlist, envp, &i_redir);
			cmdlist = cmdlist->next;
		}
		run_final_cmd(cmdlist, envp, i_redir, o_redir);
	}
	else
		run_final_cmd(cmd, envp, i_redir, o_redir);
}

// NOTE target could be a single char for matching purposes...
// To remove control parameters we find the > < character in cmd->argv
// ...it is that position and the next that need to be removed.
// So we copy the value +2 ahead from there to the end of the array.
// free parts and reduce argc by 2.
// FIXME One block is lost (to valgrind) after stripping things for > >>
// NOTE Is it legit to have << *after* > ?
// NOTE In bash the < and > can be anywhere: you take the control posn and the next param,
// NOTE Fixed arrays (as we have for argv) cannot be resized!
void	remove_cmd_parts(t_command *cmd, char *target)
{
	int	i;
//	char	*to_free;

	i = 0;
//	print_cmd_parts(cmd);	// HACK for debugging
	while ((cmd->argv[i]) && (ft_strncmp(cmd->argv[i], target, 1) != 0))	// NOTE this matches both
	{
		if (i == cmd->argc - 1)
			return ;
		i++;
	}
//	printf("\ntarget at position %i", i);	// HACK debugging only
	while (cmd->argv[i + 2])	// FIXME Can reach here when target not present
	{
//		to_free = cmd->argv[i];
		cmd->argv[i] = cmd->argv[i + 2];
		i++;
//		free (to_free);
	}
//	printf("\tending at position %i", i);	// HACK debugging only
//	print_cmd_parts(cmd);	// HACK for debugging
	cmd->argc = cmd->argc - 2;
	cmd->argv[cmd->argc] = NULL;
	/* free (cmd->argv[i + 1]);	// FIXME Invalid free */
	/* free (cmd->argv[i + 2]); */
//	print_cmd_parts(cmd);	// HACK for debugging
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// NOTE The input file pointer connects us to the previous command in pipe.
// TODO Move builtin check to run_command to save lines?
void	run_in_pipe(t_command *cmd, char **envp, int *i_file)
{
	pid_t	child;
	int		tube[2];

	if (pipe(tube) == -1)
		exit_failed_pipe(NULL, tube[0], tube[1]);
	child = fork();
	if (child == -1)
		exit_failed_pipe(NULL, tube[0], tube[1]);
	if (child == 0)
	{
		close(tube[0]);
		dup2(tube[1], STDOUT_FILENO);
		close(tube[1]);		// NOTE This reference to the file is not needed we use STDOUTs now
		// "redirect stdin to prevpipe"
		dup2(*i_file, STDIN_FILENO);// closes STDIN, uses its ref to point to i_file (last processes' pipe read end)
		close(*i_file);		// discard extra reference to the pipe read end.
		if (cmd->builtin == NONE)
			run_command(cmd, envp);	// asfter this all fds of the child are released.
		else
		{
			executeBuiltin(cmd, envp);
			exit_successful_pipe(cmd);
		}
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
// NOTE This is the one we use for simple commands. Should work with redirect.
void	run_final_cmd(t_command *cmd, char **envp, int i_file, int o_file)
{
	pid_t	child;
	int		ret_val;
	extern int	g_procstatus;

	child = fork();
	if (child == -1)
	{
		g_procstatus = errno;
	}
	if (child == 0)
	{
		if (o_file != STDOUT_FILENO)	// Detect whether redirection is *needed*
		{
			dup2(o_file, STDOUT_FILENO); // Although if these are equal, nothing happens.
			close (o_file);	// if o_file was set as STDOUT before, closing would be dangerous as they are the *same* fd, not like dup (new fd pointing to same resource)
		}
		dup2(i_file, STDIN_FILENO);	// Expect this to be the same as with pipe
		close(i_file);
		if (cmd->builtin == NONE)
			run_command(cmd, envp);	// asfter this all fds of the child are released.
		else
		{
			executeBuiltin(cmd, envp);
			exit_successful_pipe(cmd);
		}
	}
	else
	{
		ret_val = waitpid(child, &g_procstatus, 0);
//		dup2(o_file, STDOUT_FILENO); // NOTE BAD this closes the shell's STDOUT!
//		printf("waiting for child process: %i", child);	// HACK for debugging
		if (o_file != STDOUT_FILENO)	// Make sure we don't close STDOUT if no redirection was given.
			close(o_file);	// This makes sense if we are writing to a file *and* have access to it.
		if (i_file != STDIN_FILENO)
			close(i_file);	// NOTE If this file is closed, shell never returns
		if (ret_val == -1)
			printf("error in child process");
		printf("process finished with code: %i\n", g_procstatus); // HACK for debugging
	}
}
