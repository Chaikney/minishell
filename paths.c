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
// DONE Implement pipe handling - split commands
// TODO Implement pipe handling - run in order
// TODO Implement << stop word type input!
// DONE Unify input and output mangling so they can both run.
// FIXME Will need to be shorter
void	handle_complex_command_structure(t_command *cmd, char **envp)
{
	int	num_pipes;
	int	i;
	int	o_redir;
	int	i_redir;
	t_command	*cmdlist;

	num_pipes = 0;
	i = 0;
	o_redir = 0;
	i_redir = 0;
	while (i < cmd->argc)
		if (ft_strncmp(cmd->argv[i++], "|", 1) == 0)
			num_pipes++;
	// FIXME something does not return and shell loses control.
	if (num_pipes > 0)
	{
		cmdlist = make_cmd_list(cmd, num_pipes);
		printf("\nmore than one (%i) command to run!\nNot implemented yet. But if you see this I didn't crash while list making!!!!", num_pipes);
		print_cmd_parts(cmdlist);
		// set input file to be STDIN, if needed.
		// close input file descriptor
		while (cmdlist->next != NULL)
		{
			run_in_child_with_pipe(cmdlist, envp);
			cmdlist = cmdlist->next;
		}
		// output file becomes STDOUT
		// close output fd/
		// run final command - in piex this is without a pipe but here
		// we need a child process, for sure.
		run_in_child(cmdlist, envp, -1, -1);
	}
	else	// we can handle the other redir types
	{
		// Identify if input or redirection is needed, and its type.
		i = cmd->argc;
		while (i-- > 0)
			if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
			{
				o_redir = 1;
				if (ft_strncmp(cmd->argv[i], ">>", 2) == 0)
					o_redir = 2;
				o_redir = direct_output(cmd, o_redir);
				remove_cmd_parts(cmd, ">");
				break ;
			}
		i = 0;
//		print_cmd_parts(cmd);	// HACK for debugging
		while (i < cmd->argc - 1)
		{
			if (ft_strncmp(cmd->argv[i], "<", 1) == 0)
			{
//				print_cmd_parts(cmd);	// HACK for debugging
				i_redir = 1;
				if (ft_strncmp(cmd->argv[i], "<<", 2) == 0)
					i_redir = 2;
				i_redir = setup_input(cmd, i_redir);
				remove_cmd_parts(cmd, "<");
				break ;	// does this break out of the while, or only the if?
			}
			i++;
		}
//		print_cmd_parts(cmd);	// HACK for debugging
		run_in_child(cmd, envp, i_redir, o_redir);
	}
}

// Unified setup of output-to-file
// - Set file perms for level (create or append)
// - find file name (final cmd parameter)
// - check access to file, open it
// - set fd of file to be STDOUT
// NOTE DO we also have to slice off the > and after? Would be bad param for command...
// NOTE Of course, *input* may also need to be handled; so this is uncomplete.
// DONE? If this fails it should set the g_procstatus variable
int	direct_output(t_command *cmd, int o_lvl)
{
	int		perms;
	char	*o_path;
	int		o_file;

	if (o_lvl == 2)
		perms = O_WRONLY | O_CREAT | O_APPEND;
	else if (o_lvl == 1)
		perms = O_WRONLY | O_CREAT | O_TRUNC;
	o_path = cmd->argv[cmd->argc - 1];
//	printf("\nTrying to open file: %s\n", o_path);	// HACK for debugging
	o_file = open(o_path, perms, 0777);
	if (o_file == -1)
	{
		g_procstatus = errno;
		perror("Could not open output file");
	}
	else
		return (o_file);
	return (-1);
}

// DONE Check access() to i_path? open probably covers it
// TODO Implement stop word / here_doc input redirection
// NOTE That is in the format: cmd << stop_word
// NOTE Input redir in format:  < infile grep a1
// If opening the file fails,
// bash quits with error and doesn’t run the command.
// If it succeeds, bash uses the file descriptor
// of the opened file as the stdin file descriptor for the command.
int	setup_input(t_command *cmd, int i_lvl)
{
	char	*i_path;
	int		i_file;
	char	*stop_word;

	if (i_lvl == 2)
	{
		// heredocs
		perror("heredocs << not yet implemented");
		i_file = STDIN_FILENO;	// FIXME Or is this "open the file?"
		stop_word = cmd->argv[2];	// FIXME this should be "position after <<"
		printf("read input until reaching %s", stop_word);	// HACK for debugging
	}
	else if (i_lvl == 1)
	{
		i_path = cmd->argv[1];
		i_file = open(i_path, O_RDONLY);
		if (i_file == -1)
			g_procstatus = errno;
	}
	else
		i_file = -1;
	return (i_file);
}

// NOTE target could be a single char for matching purposes...
// DONE Make this safe for if target not found
// To remove control parameters we find the > < character in cmd->argv
// ...it is that position and the next that need to be removed.
// So we copy the value +2 ahead from there to the end of the array.
// free parts and reduce argc by 2.
// FIXME One block is lost (to valgrind) after stripping things for > >>
// NOTE Is it legit to have << *after* > ?
// NOTE In bash the < and > can be anywhere: you take the control posn and the next param,
// FIXED If I remove pieces fromm the start, execve later is "unaddressable bytes"
// ...is this because of argv pointer confusion??
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
	printf("\ntarget at position %i", i);	// HACK debugging only
	while (cmd->argv[i + 2])	// FIXME Can reach here when target not present
	{
//		to_free = cmd->argv[i];
		cmd->argv[i] = cmd->argv[i + 2];
		i++;
//		free (to_free);
	}
	printf("\tending at position %i", i);	// HACK debugging only
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
// DONE Can I use the output from this to measure state?
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
		close(tube[1]);	// NOTE Probably only called if run_command has failed
	}
	else
	{
		close(tube[1]);
		// NOTE: commenting out this line leads to endless loop or non-returning process
		dup2(tube[0], STDIN_FILENO);	// NOTE do not mess with shell / parent fds
		waitpid(child, &g_procstatus, 0);
		close(tube[0]);
		printf("process %s finished with code: %i\n", cmd->argv[0], g_procstatus); // HACK for debugging
	}
}

// Simplest command runner.
// Forks, sets up input and output for one child process
// and waits for it to complete.
// NOTE This is the one we use for simple commands.
// DONE do something with error status here.
void	run_in_child(t_command *cmd, char **envp, int i_file, int o_file)
{
	pid_t	child;
	int		ret_val;
	extern int	g_procstatus;

//	g_procstatus = 0;
	child = fork();
	if (child == -1)
	{
		g_procstatus = errno;
	//	exit_and_free(NULL, -1, -1);
	}
	if (child == 0)
	{
		if (o_file > 0)
			dup2(o_file, STDOUT_FILENO);
		if (i_file > 0)
			dup2(i_file, STDIN_FILENO);	// TODO I have no idea if this works!
		run_command(cmd, envp);
	}
	else
	{
//		printf("waiting for child process: %i", child);	// HACK for debugging
		ret_val = waitpid(child, &g_procstatus, 0);
		if (ret_val == -1)
			printf("error in child process");
		printf("process finished with code: %i", g_procstatus); // HACK for debugging
	}
}
