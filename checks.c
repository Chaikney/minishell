/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 19:47:45 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/20 12:41:22 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions to protect against bad combinations of tokens.
// FIXME Too many functions in file

// Any redirection token is followed by a file
// bad returns -1
// TODO Not fully implemented yet.
// Find a control structure then check:
// i + 1 is a file
// ...and is accessible, or could be created
// TODO Could check validity of target
int	has_target(char **arr)
{
	int	i;

	i = 0;
	while (arr[i + 1] != NULL)
	{
		if ((ft_strncmp(arr[i], "<", 1) == 0)
			|| (ft_strncmp(arr[i], ">", 1) == 0))
		{
			if (is_control_char(arr[i + 1][0]) == 1)
				return (-1);
		}
		i++;
	}
	return (0);
}

// returns 0 if all the control tokens in the file are well-formed.
// That means the right length, with no other pieces
// > and < can be no longer than 2 chars,
// also if 2 long, they must be the same
// | must be 1 char long
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
			if ((arr[i][0] != '|')
				&& (ft_strlen(arr[i]) == 2)
				&& (arr[i][0] != arr[i][1]))
				return (-1);
		}
		i++;
	}
	return (0);
}

// returns -1 if the token array ends or begins
// with an (illegitimate) control character
// - is the first char in the end token a control? BAD
// - is the first token a > or |? BAD
// NOTE input redirection (both types) at the start is GOOD
// FIXED < test less is legit!!! so is < test rev
int	no_extreme_controls(char **arr)
{
	int	is_bad;
	int	len;

	is_bad = 0;
	len = count_tokens(arr);
	if (is_control_char(arr[len - 1][0]) == 1)
		return (-1);
	if ((arr[0][0] == '|') || (arr[0][0] == '>'))
		is_bad = -1;
	return (is_bad);
}

// return -1 if there are any control characters *not*
// followed by legitimate arguments (i.e. a plausible filename)
// NOTE This and has_target are *very* similar. Too similar?
int	then_text(char **arr)
{
	int	is_bad;
	int	i;

	is_bad = 0;
	i = count_tokens(arr);
	while (i > 0)
	{
		if (is_control_char(arr[i - 1][0] == 1))
		{
			if (arr[i] == NULL)
				return (-1);
			if (ft_isdigit(arr[i][0]) == 1)
				return (-1);
		}
		i--;
	}
	return (is_bad);
}

// Ensure that there are a plausible number of tokens between pipes.
// Generally this means at least one.
// BUT redirection tokens (and the token after them) do *not* count.
// This is because they are removed before command execution.
int	pipe_count(char **arr)
{
	int	i;
	int	j;

	i = 0;
	while (arr[i])
	{
		j = 0;
		while ((arr[i]) && (ft_strncmp(arr[i], "|", 1) != 0))
		{
			if ((ft_strncmp(arr[i], ">", 1) == 0) || (ft_strncmp(arr[i], ">", 1) == 0))
				i++;
			else
				j++;
			i++;
		}
		if (j < 1)
			return (-1);
		i++;
	}
	return (0);
}

// Return a -1 if there are illegal combinations of tokens
// e.g. multiple attempts at redirection for either I or O
// Implement sanity checks on token set:
// [x]	do not start with a pipe
// [x]	do not end with a pipe
// [x]	do not have both > and >>
// [x]	do not have both < and <<
// [ ]	must have text after any control char
int	check_tokens(char **arr)
{
	int	is_bad;

	is_bad = clean_controls(arr);
	if (is_bad == 0)
		is_bad = no_extreme_controls(arr);
	if (is_bad == 0)
		is_bad = has_target(arr);
	if (is_bad == 0)
		is_bad = then_text(arr);
	if (is_bad == 0)
		is_bad = pipe_count(arr);
	return (is_bad);
}
