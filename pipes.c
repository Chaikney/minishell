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

// Allocate and safely set up a t_command struct (object).
// The memory allocated here is completely freed in clear_t_command
// TODO pipes.c is almost empty and/or misnamed.
t_command	*init_new_command(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));	// FIXED? Memory allocated here is not freed
	new_cmd->argc = 0;
	new_cmd->builtin = NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}
