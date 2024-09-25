/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:26:28 by emedina-          #+#    #+#             */
/*   Updated: 2024/09/24 15:00:02 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Wrapper to call the command list builder
// and clear the array of parsed tokens.
t_command	*parse_input2(char **tokens)
{
	t_command	*cmd_head;

	cmd_head = build_command_list(tokens);
	wipe_tokens(tokens);
	if (cmd_head->argv[0] == NULL)
		return (NULL);
	return (cmd_head);
}

// Checks to ensure that any quotations in the input are closed.
// This part of the function is core of the analysis
// 0 - quotes are NOT closed
// 1 - quotes are closed
int	closed_quotes2(int j, int counter1, int counter2, char *ptr)
{
	while (ptr[j] != '\0')
	{
		if (ptr[j] == '\"')
		{
			counter1++;
			if (ptr[j - 1] == '\\')
				counter1--;
		}
		if (ptr[j] == '\'')
		{
			counter2++;
			if (ptr[j - 1] == '\\')
				counter2--;
		}
		j++;
	}
	if (counter1 % 2 != 0 || counter2 % 2 != 0)
		return (0);
	return (1);
}
