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

// FIXME Not clear what cmdline == NULL attempts, i can't trigger it.
// ...readline man page says this what it returns on EOF on an empty line.
// TODO Ensure that *all* commands run quit or return to here.
// NOTE Exit called by user does not need to free prompt
// ....as we destory it before eval
int	main(int argc, char **argv, char **envp)
{
	char	*cmdline;
	char	*prompt;
	t_env	*enviro;

	(void) argv;
	cmdline = NULL;
	if (argc == 1)
	{
		setup_signals();
		enviro = parse_env(envp);
		sort_env(enviro);
		while (1)
		{
			prompt = get_prompt(envp);
			cmdline = readline(prompt);
			free (prompt);
			if (cmdline == NULL)
				ms_exit(NULL);
			if ((cmdline[0] != '\0'))
			{
				add_history((const char *) cmdline);
				eval(cmdline, envp, enviro);
			}
		}
	}
	else
		printf("no args needed to minishell\n");
	return (0);
}
