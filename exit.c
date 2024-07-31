/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:37:32 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:37:35 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Shell's exit clears the relevant variables and ends the program.
// This handles USER-REQUESTED exits only.
// NOTE This is different from exits in pipes or failed commands.
// - free cmd
// - what about the readline input variable?
// TODO If this is called from main, we have a char* to free
void	ms_exit(t_command *cmd, t_env *envt)
{
	printf("Agurrrr....\n");
	clear_t_command(cmd);
	clear_environment(envt);
	exit(EXIT_SUCCESS);
}

// Our copy of the environment should all be dynamically
// allocated by us, so we free it.
// DO check the memory free-ing of clear_envrionment
// - works for the full environment
// - need to free individual blocks when UNSET
void	clear_environment(t_env *envt)
{
	t_env	*ptr;

	if (envt)
	{
		while (envt->next != NULL)
		{
			free (envt->vname);
			if (envt->value)
				free (envt->value);
			ptr = envt;
			envt = envt->next;
			free (ptr);
		}
		free (envt->vname);
		if (envt->value)
			free (envt->value);
		free (envt);
	}
}

// Call this when we need to exit from a successful process that
// ran in a pipe but hasn't otherwise (i.e. via execve) been exited.
// SO, the builtins, basicallyy.
// TODO Ensure this works as expected and clears everything.
// (File descriptors were handled before calling this.)
void	exit_successful_pipe(t_command *cmd)
{
	clear_t_command(cmd);
	g_procstatus = 0;
	exit (EXIT_SUCCESS);
}

// Cleanly exit from a failed command in a pipe
void	exit_failed_pipe(t_command *cmd, int fd_in, int fd_out)
{
	g_procstatus = errno;
	if (cmd)
		clear_t_command(cmd);
	perror(strerror(errno));
	if ((fd_in) && (fd_in != -1))
		close(fd_in);
	if ((fd_out) && (fd_out != -1))
		close(fd_out);
	exit (EXIT_FAILURE);
}

// Free the memory allocated to a t_command and all those following it.
// Frees all argv, including the final NULL
// - clear all the char* held in argv
// - move to the nex tin the chain
// - free the t_command left behind.
void	clear_t_command(t_command *cmd)
{
	int			i;
	t_command	*ptr;

	if (cmd)
	{
		while (cmd->next != NULL)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv[cmd->argc]);
			ptr = cmd;
			cmd = cmd->next;
			free (ptr);
		}
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv[cmd->argc]);
		free (cmd);
	}
}
