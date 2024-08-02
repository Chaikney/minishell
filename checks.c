/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:47:45 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/01 19:47:47 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions to protect against bad combinations of tokens.

// Return a -1 if there are illegal combinations of tokens
// [x]	have text after any control char
int	proximity_test(char **arr)
{
	int	i;
	int	is_bad;

	i = 0;
	is_bad = 0;
	while (arr[i + 1] != NULL)
	{
		if (is_control_char(arr[i][0]) == 1)
		{
			if ((arr[i + 1]) && (ft_isalpha(arr[i + 1][0]) == 0))
			{
				is_bad = -1;
				break ;
			}
		}
		i++;
	}
	return (is_bad);
}

// Return a -1 if there are illegal combinations of tokens
// e.g. multiple attempts at redirection for either I or O
// DONE Implement sanity checks on token set.
// [x]	do not start with a pipe
// [x]	do not end with a pipe
// [x]	do not have both > and >>
// [x]	do not have both < and <<
// [x]	do not have more than one input directive
// [x]	do not have more than one output directive
// [ ]	have text after any control char
int	check_tokens(char **arr)
{
	int	num_input;
	int	num_output;
	int	i;
	int	is_bad;

	num_input = 0;
	num_output = 0;
	is_bad = 0;
	i = 0;
	if ((ft_strncmp(arr[0], "|", 1) == 0)
		&& (ft_strncmp(arr[i - 1], "|", 1) == 0))
		is_bad = -1;
	while (arr[i] != NULL)
	{
		if (ft_strncmp(arr[i], "<", 1) == 0)
			num_input++;
		if (ft_strncmp(arr[i], ">", 1) == 0)
			num_output++;
		i++;
	}
	if ((num_input > 1) || (num_output > 1))
		is_bad = -1;
	if (is_bad == 0)
		is_bad = proximity_test(arr);
	return (is_bad);
}
