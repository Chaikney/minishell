/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:47:55 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/02 13:10:00 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_procstatus;

// Sends cmdline to parse function to get a t_command list
// Pass it to the complex function
// Clear the command struct on return.
// NOTE Was an execbuiltin check here
// as some builtins don't work in pipes!
// - EXIT has to be an exit from the shell.
// - EXPORT has to change values in the process above.
// FIXED Too many lines in eval function
// DONE Restore length check
void	eval(char *cmdline, t_env *envt)
{
	t_command	*cmd;
	char		*trimmed;
	int			len;

	len = 0;
	while (cmdline[len] == ' ')
		len ++;
	if (cmdline[len] == '\0')
	{
		free (cmdline);
		return ;
	}
	trimmed = ft_strtrim(cmdline, "\t\n\r\f\v\b ");
	if ((trimmed == NULL) || (ft_strlen(trimmed) > MAXPARAM))
	{
		printf("Input too long (or short) for **MINI**shell to process...\n");
		free (trimmed);
		return ;
	}
	cmd = parse_input(trimmed, envt);
	if (cmd)
	{
		direct_complex_command(cmd, envt);
		clear_t_command(cmd);
	}
}

// read the Starting environment into a variable to access.
// for line in envp
// - find name (all up to =)
// - find value
// - set marker to next t_env
t_env	*parse_env(char **envp)
{
	t_env	*ptr;
	t_env	*first;
	t_env	*next;

	if (*envp)
	{
		first = init_new_env(*envp);
		ptr = first;
		envp++;
		while (*envp)
		{
			next = init_new_env(*envp);
			ptr->next = next;
			ptr = ptr->next;
			envp++;
		}
		return (first);
	}
	return (NULL);
}

// Print a message explaining what the shell can and cannot do.
// TODO Update start message to match what has been added in
void	startup_message(void)
{
	printf("\n********************************************\n");
	printf("\n\tWelcome to Minishell!\nby chaikney and emedina- for 42U\n");
	printf("\n********************************************\n");
	printf("This interactive-only shell supports:\n\t- multiple pipes\n\t");
	printf("- input and output redirection and\n\t");
	printf("- variable substitution (in normal input only).\n");
	printf("Note that this is *not* a clone of bash and all its quirks.\n");
	printf("Other shells like fish have been considered in its design.\n");
	printf("This shell does *not* support:");
	printf("\n\t- Multiple or mid-pipe redirection");
	printf("\n\t- Running a copy of itself.\n\t- Running in a pipe,");
	printf("\n\t- Pasted or multiline input");
	printf("\n\t- Wildcards / globbing, or\n\t- Script execution.\n");
	printf("\n********************************************\n");
}

// Simple checks for the validity of launch.
// - We must be in an interactive terminal
// - There are no extra arguments given.
// NOTE The argc error condition uses printf to avoid strange message
// ": Success"
int	startup_checks(int argc)
{
	if ((isatty(STDIN_FILENO) == 0) || (isatty(STDOUT_FILENO) == 0))
	{
		perror("minishell must run in a terminal");
		exit (EXIT_FAILURE);
	}
	if (argc != 1)
	{
		printf("no args needed to minishell\n");
		exit (EXIT_FAILURE);
	}
	setup_signals();
	startup_message();
	return (0);
}

// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// NOTE Exit called by user does not need to free prompt
// ....as we destory it before eval
// FIXED segfault with <>
// NOTE cat << a - in bash this relies on multiline input.
// Here we are not required to handle that ("no unclosed quotes") therefore
// use CTRL-D to end the input.
// NOTE cat << a << b << c << d - whatever it does in bash,
// 		we do *not* handle multiple redirection.
// FIXME echo "thing" > out | less	what then?
// Problem was it did not recognise the redir.
// bash & fish - out contains "thing", less is blank
// TODO Test echo thing > out -- would expect redirection.
int	main(int argc, char **argv, char **envp)
{
	char	*cmdline;
	char	*prompt;
	t_env	*enviro;

	(void) argv;
	startup_checks(argc);
	cmdline = NULL;
	enviro = parse_env(envp);
	sort_env(enviro);
	while (1)
	{
		prompt = get_prompt(enviro);
		cmdline = readline(prompt);
		free (prompt);
		if (cmdline == NULL)
			ms_exit(NULL, enviro);
		if ((cmdline[0] != '\0'))
		{
			add_history((const char *) cmdline);
			eval(cmdline, enviro);
		}
	}
	return (0);
}
