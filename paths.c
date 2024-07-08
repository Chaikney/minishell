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

// When there is a control character present, guide it to the correct
// execution function(s)
// TODO Shorter (but still descriptive!) name needed.
// NOTE Flags for o_redir:
// 0 - no output redirection
// 1 - create file mode
// 2 - append file mode
// Similar with input redir:
// 0 - no special input
// 1 - input from file
// 2 - input from STDIN with stop word.
// TODO Split into identification and routing parts (is too long)
// TODO Implement pipe handling - split commands and run in order
// TODO Implement << stop word type input!
// TODO Unify input and output mangling so they can both run.
void	handle_complex_command_structure(t_command *cmd, char **envp)
{
	int	num_pipes;
	int	i;
	int	o_redir;
	int	i_redir;

	num_pipes = 0;
	i = 0;
	o_redir = 0;
	i_redir = 0;
	while (i < cmd->argc)
		if (ft_strncmp(cmd->argv[i++], "|", 1) == 0)
			num_pipes++;
	if (num_pipes > 0)	// HACK refuse to handle commands with pipes
	{
		printf("\nmore than one (%i) command to run!\nNot implemented yet", num_pipes);
	}
	else	// we can handle the other redir types
	{
		// Identity if input or redirection is needed, and its type.
		i = cmd->argc;
		while (i-- > 0)
			if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
			{
				o_redir = 1;
				if (ft_strncmp(cmd->argv[i], ">>", 2) == 0)
					o_redir = 2;
				break ;
			}
		i = 0;
		while (i++ > 0)
			if (ft_strncmp(cmd->argv[i], "<", 1) == 0)
			{
				i_redir = 1;
				if (ft_strncmp(cmd->argv[i], "<<", 2) == 0)
					i_redir = 2;
				break ;
			}
		// NOTE Below is the direction part
		if (o_redir > 0)
				direct_output(cmd, envp, o_redir);
		if (i_redir > 0)
		{
			if (i_redir == 2)
			{
				// open the here_doc palaver
			}
			else
			{
				// run using an opened file
			}
		}
	}
}

// Unified setup of output-to-file
// - Set file perms for level (create or append)
// - find file name (final cmd parameter)
// - check access to file, open it
// - set fd of file to be STDOUT
// NOTE DO we also have to slice off the > and after? Would be bad param for command...
// NOTE Of course, *input* may also need to be handled; so this is uncomplete.
// TODO What if this just returns an open / valid fd that we should use
// TODO Do we need to reset STDOUT afterwards, or is this going to be process-limited?
// FIXME This will not work without mangling the argvs before sending to run_
// TODO If this fails it should set the g_procstatus variable
void	direct_output(t_command *cmd, char **envp, int o_lvl)
{
	int		perms;
	char	*o_path;
	int		o_file;

	if (o_lvl == 2)	//
		perms = O_WRONLY | O_CREAT | O_APPEND;
	else if (o_lvl == 1)
		perms = O_WRONLY | O_CREAT | O_TRUNC;
	o_path = cmd->argv[cmd->argc];
	o_file = open(o_path, perms);
	if (o_file == -1)
		perror("Could not open output file");
	dup2(o_file, STDOUT_FILENO);
	close (o_file);	// this file descriptor not needed now? or call this after running?
	trim_cmdset(cmd);
	run_in_child(cmd, envp);
}

// TODO What if this just returns an open / valid fd that we should use
// for the command?
// TODO Implement stop word / here_doc input redirection
// NOTE That is in the format: cmd << stop_word
// NOTE Input redir in format:  < infile grep a1
void	setup_input(t_command *cmd, char **envp, int i_lvl)
{
	char	*i_path;
	int		i_file;
	char	*stop_word;

	if (i_lvl == 2)
	{
		// heredocs
		i_file = STDIN_FILENO;	// FIXME Or is this "open the file?"
		stop_word = cmd->argv[2];
		printf("read input until reaching %s", stop_word);	// HACK for debugging
	}
	else if (i_lvl == 1)
	{
		i_path = cmd->argv[1];
		i_file = open(i_path, O_RDONLY);
	}
	dup2(i_file, STDIN_FILENO);
	// TODO Slice off the first part of the cmd set.
	run_in_child(cmd, envp);	// FIXME This will not work without mangling argvs!
}

// remove the last part of the argv and argc of the passed command
// Check that there really *is* a redirect there
// take away the last 2 terms (filename and symbol)
// (both need to be free'd, yes?)
// TODO Make sure the removed pieces are not needed/used elsewhere.
// TODO This could also trim the first pieces - different method needed?
void	trim_cmdset(t_command *cmd)
{
	if (ft_strncmp(cmd->argv[cmd->argc - 1], ">", 1) == 0)	// NOTE this matches both
	{
		free(cmd->argv[cmd->argc]);	// the filename
		cmd->argv[cmd->argc - 1] = NULL;	// the control chars
		cmd->argc = cmd->argc - 2;
	}
	if (ft_strncmp(cmd->argv[0], "<", 1) == 0)	// match input redirs
	{
		printf("remove input file parts");
	}
	else if (ft_strncmp(cmd->argv[1], "<<", 2) == 0)	// FIXME try and match input redirs
	{
		printf("remove stop word parameters");
	}
}

// Make a child process to execute the command, putting the output in a pipe
// - fork
// - run command
// - wait for it to come back
// NOTE child == 0 means we are in the child process!
// DONE Can I use the output from this to measure state?
// TODO This needs an outer loop / function calling it with the correct cmd parts
// FIXME Looks like function is getting too long...
// TODO The exit_and_free should be unified with ms_exit, or renamed.
// ...don't want to leave the entire shell for fork/pipe errors.
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
	extern int	g_procstatus;

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
