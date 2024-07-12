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

// Wrap everything needed to work out where the output comes from,
// and return the fd to it.
// FIXED Returns STDOUT no matter what
// NOTE Flags for o_redir:
// 0 - no output redirection
// 1 - create file mode
// 2 - append file mode
int	determine_output(t_command *cmd)
{
	int	i;
	int	o_fd;
	int	o_redir;

	i = cmd->argc;
	o_fd = STDOUT_FILENO;
	o_redir = 0;
	while ((i-- > 0))
	{
		if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
		{
			o_redir = 1;
			if (ft_strncmp(cmd->argv[i], ">>", 2) == 0)
				o_redir = 2;
			o_fd = direct_output(cmd, o_redir);
		}
	}
	printf("Checked output for '%s'. fd will be: %i", cmd->argv[0], o_fd);
	return (o_fd);
}

// Wrap everything needed to work out where the input comes from,
// and return the fd to it.
// FIXED Returns STDIN no matter what
// Similar with input redir:
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
	printf("Checked input for '%s'. fd will be: %i\n", cmd->argv[0], i_fd);
	return (i_fd);
}

// When there is a control character present, guide it to the correct
// execution function(s)
// TODO Shorter (but still descriptive!) name needed.
// TODO Function will need to be shorter once it is working
// TODO Implement << stop word type input!
// DONE Unify input and output mangling so they can both run.
// DONE Unify pipes and i/o redirection
// TODO Ensure that after pipes we still have a working shell input!
// TODO Must be able to handle BUILTINS here as well.
// FIXME Output redirection no longer works
// FIXME < test | rev | rev triggered a crash
// FIXED test < less failed with "text file busy" - unclosed fd?
// FIXED ls | rev | tac | rev displays reversed debug. Does that mean bad setup?
void	handle_complex_command_structure(t_command *cmd, char **envp)
{
	int	num_pipes;
	int	i;
	int	o_redir;
	int	i_redir;
	t_command	*cmdlist;

	num_pipes = 0;
	i = 0;
	while (i < cmd->argc)
		if (ft_strncmp(cmd->argv[i++], "|", 1) == 0)
			num_pipes++;
	i_redir = determine_input(cmd);
	o_redir = determine_output(cmd);
	// HACK Next few lines debugging statements to remove
	/* printf("Command before excision"); */
	/* print_cmd_parts(cmd); */
	remove_cmd_parts(cmd, ">");
	remove_cmd_parts(cmd, "<");
	/* printf("Command after excision"); */
	/* print_cmd_parts(cmd); */
	printf("\tOutput to: %i\tInput to: %i", o_redir, i_redir);
	// FIXED something does not return and shell loses control.
	if (num_pipes > 0)
	{
		cmdlist = make_cmd_list(cmd, num_pipes);
		print_cmd_parts(cmdlist);
		// set input file to be STDIN, if needed.
//		dup2(i_redir, STDIN_FILENO);
		// close input file descriptor
//		close(i_redir);	// TODO Understand if this closes STDIN after above
		while (cmdlist->next != NULL)
		{
			run_in_child_with_pipe(cmdlist, envp, &i_redir);
			cmdlist = cmdlist->next;
		}
		// output file becomes STDOUT
		dup2(o_redir, STDOUT_FILENO);
		// close output fd
//		close(o_redir);
		// run final command - in piex this is without a pipe but here
		// we need a child process, for sure.
		run_in_child(cmdlist, envp, i_redir, o_redir);	// FIXME These -1 may be the problem!
	}
	else	// we can handle the other redir types
	{
		print_cmd_parts(cmd);	// HACK for debugging
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
// TODO If no redir is needed, then should we return STDOUT_FILENO?
// TODO Potential to merge with determine_output
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
// TODO If no redir is needed, then should we return STDIN_FILENO?
// TODO Potential to merge with determine_output
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
		i_file = STDIN_FILENO;
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
// Now takes an input file pointer to connect to the previous command in pipe.
void	run_in_child_with_pipe(t_command *cmd, char **envp, int *i_file)
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
		close(tube[1]);	// NOTE Probably only called if run_command has failed
		// "redirect stdin to prevpipe"
		dup2(*i_file, STDIN_FILENO);
		close(*i_file);
		run_command(cmd, envp);
	}
	else
	{
		close(tube[1]);
		// NOTE: commenting out this line leads to endless loop or non-returning process
//		dup2(tube[0], STDIN_FILENO);	// NOTE do not mess with shell / parent fds
		waitpid(child, &g_procstatus, 0);
		*i_file = tube[0];	// Keep hold of the read end of this pipe for the next run.
//		close(tube[0]);
		printf("process %s finished with code: %i\n", cmd->argv[0], g_procstatus); // HACK for debugging
	}
}

// Simplest command runner.
// Forks, sets up input and output for one child process
// and waits for it to complete.
// NOTE This is the one we use for simple commands. Should work with redirect.
// FIXME This no longer works for the simple output redirection case
// (Though alone pipes are happy)
// FIXME output redirection at the end of pipes does not work.
// TODO rename to "run_last" or similar
// TODO Do I need a pipe in this one as well?
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
	//	close(i_file);
		run_command(cmd, envp);
	}
	else
	{
//		printf("waiting for child process: %i", child);	// HACK for debugging
//		close(o_file);	// This makes sense if we are writing to a file *and* have access to it.
//		close(i_file);	// NOTE If this file is closed, shell never returns
		ret_val = waitpid(child, &g_procstatus, 0);
		if (ret_val == -1)
			printf("error in child process");
		printf("process finished with code: %i", g_procstatus); // HACK for debugging
	}
}
