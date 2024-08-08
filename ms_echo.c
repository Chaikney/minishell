/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:39:25 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/08 12:58:23 by emedina-         ###   ########.fr       */
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
		i = echon(cmd, i);
		if (cmd->argv[i] != NULL)
		{
			printf("%s", cmd->argv[i]);
			if (i < cmd->argc - 1)
				printf(" ");
			i++;
		}
	}
	if (cmd->builtin == ECHO)
		printf("\n");
	g_procstatus = 0;
	return ;
}

int	echon(t_command *cmd, int i)
{
	if (cmd->argv[1][0] == '-' && cmd->argv[1][1] == 'n')
	{
		while ((cmd->argv[i][0] == '-' && cmd->argv[i][1] == 'n')
			&& cmd->argv[i + 1] != NULL)
			i++;
		if (cmd->argv[i][0] == '-' && cmd->argv[i][1] == 'n')
			i++;
	}
	return (i);
}
