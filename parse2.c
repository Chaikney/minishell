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
// Scans the string *ptr and counts each type of quote.
// If there is an even number of each, the quotes are balanced.
// Returns:
// 0 - quotes are NOT closed
// 1 - quotes are closed
int	closed_quotes(char *ptr)
{
	int	j;
	int	counter1;
	int	counter2;

	j = 0;
	counter1 = 0;
	counter2 = 0;
	while (ptr[j] != '\0')
	{
		if (ptr[j] == '\"' )
			counter1++;
		if (ptr[j] == '\'')
			counter2++;
		j++;
	}
	if (counter1 % 2 != 0 || counter2 % 2 != 0)
		return (0);
	return (1);
}
