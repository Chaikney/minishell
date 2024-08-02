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

// Any redirection token is followed by a file
// bad returns -1
// TODO Not fully implemented yet.
int	has_target(char **arr)
{
	int	i;

	i = 0;
	while (arr[i + 1] != NULL)
	{
		if ((ft_strncmp(arr[i], "<", 1) == 0)
			|| (ft_strncmp(arr[i], ">", 1) == 0))
		{
			// i + 1 is a file
			// ...and is accessible, or could be created
		}
		i++;
	}
	return (0);		// HACK incomplete checks!
}

// returns 0 if all the control tokens in the file are well-formed.
// That means the right length, with no other pieces
int	clean_controls(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (is_control_char(arr[i][0]) == 1)
		{
			if (ft_strlen (arr[i]) > 2)
				return (-1);
			if ((arr[i][0] == '|') && (ft_strlen(arr[i]) != 1))
				return (-1);
			if ((arr[i][0] != '|') && (arr[i][0] != arr[i][1]))
				return (-1);
		}
		i++;
	}
	return (0);
}

// returns -1 if the token array ends or begins with a control character
int	no_extreme_controls(char **arr)
{
	int	is_bad;
	int	len;

	is_bad = 0;
	len = count_tokens(arr);
	printf("array len is %i", len);
	if ((is_control_char(arr[0][0]) == 1)
		|| (is_control_char(arr[len - 1][0]) == 1))
		is_bad = -1;
	return (is_bad);
}

// Return a -1 if there are illegal combinations of tokens
// [x]	have text after any control char
// FIXME Unsure about this test.
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
// FIXME too many lines in check_tokens
// FIXED less | hangs forever
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
	if ((is_bad = clean_controls(arr)) == -1)
		printf("dirty control structures");
	if (is_bad == 0)
	{
		if ((is_bad = no_extreme_controls(arr)) == -1)
			printf("malformed cotrol structures");
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
		if ((is_bad == 0) && ((num_input != 0) || (num_output != 0)))
			is_bad = has_target(arr);
		if (is_bad == -1)
			printf("failed redirection number or targets");
	}
	if (is_bad == 0)
		if (proximity_test(arr) == -1)
			printf("failed prox tester");
	return (is_bad);
}
