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
void	eval(char *cmdline, char **envp)
{
	t_command	*cmd;

	cmd = parse(cmdline);
	if (cmd)
	{
		direct_complex_command(cmd, envp);
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

	(void) argv;
	cmdline = NULL;
	if (argc == 1)
	{
		setup_signals();
		while (1)
		{
			prompt = get_prompt();
			cmdline = readline(prompt);
			free (prompt);
			if (cmdline == NULL)
				ms_exit(NULL);
			if ((cmdline[0] != '\0'))
			{
				add_history((const char *) cmdline);
				eval(cmdline, envp);
			}
		}
	}
	else
		printf("no args needed to minishell\n");
	return (0);
}
