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
// FIXED Too many lines, we can consolidate this.
// DONE Double check the echo / echon split and i values.
// FIXED These names were the wrong way round! ECHON should not have \n
// NOTE The output is correct but this setup is confusing.
void	ms_echo(t_command *cmd)
{
	int	i;

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
