/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:39:25 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:39:29 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Minishell implementation of the echo command.
// Supports the -n option (NO newline after output)
void	ms_echo(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->builtin == ECHO)
		i = 1;
	else if (cmd->builtin == ECHON)
		i = 2;
	while (i < cmd->argc)
	{
		printf("%s", cmd->argv[i]);
		if (i < cmd->argc - 1)
			printf(" ");
		i++;
	}
	if (cmd->builtin == ECHO)
		printf("\n");
	return ;
}
