/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:56:28 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/16 17:56:32 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remove specified cmd arg characters from memory
// Assume that they have been processed and the command split into a list.
// This is picking up the parts left in the original cmd.
// TODO Check the remove_cmd_part(s) functions - this one unused.
void	remove_cmd_part(t_command *cmd, int posn)
{
	int	i;

	free(cmd->argv[posn]);
	i = posn;
	while (cmd->argv[i + 1] != NULL)
	{
		cmd->argv[i] = cmd->argv[i + 1];
		i++;
	}
	cmd->argv[i] = cmd->argv[i + 1];
	cmd->argc = cmd->argc - 1;
}

// Allocate and safely set up a t_command struct (object).
// The memory allocated here is completely freed in clear_t_command
t_command	*init_new_command(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));	// FIXED? Memory allocated here is not freed
	new_cmd->argc = 0;
	new_cmd->builtin = NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}
