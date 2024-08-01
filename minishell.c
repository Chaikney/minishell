/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:47:55 by chaikney          #+#    #+#             */
/*   Updated: 2024/06/06 13:48:06 by chaikney         ###   ########.fr       */
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
// TODO Move some of direct_complex_command back here?
// TODO Can we remove envp from eval altogether?
void	eval(char *cmdline, char **envp, t_env *envt)
{
	t_command	*cmd;
	char		*trimmed;

	trimmed = ft_strtrim(cmdline, " ");
	if (trimmed == NULL)
		perror("command line is NULL\n");
	free (cmdline);
	cmd = parse(trimmed, envt);
	if (cmd)
	{
		direct_complex_command(cmd, envp, envt);
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
	return (0);
}

// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// TODO Ensure that *all* commands run quit or return to here.
// NOTE Exit called by user does not need to free prompt
// ....as we destory it before eval
// FIXED the g_procstatus var $? needs to get special treatment.
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
			eval(cmdline, envp, enviro);
		}
	}
	return (0);
}
