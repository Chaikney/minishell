/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:16:53 by chaikney          #+#    #+#             */
/*   Updated: 2024/06/24 12:17:02 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME Multiple NORM failures in parse.c!

// Quotation-aware command line split / tokenising
// 3 modes: raw, "weak" quoting and 'strong' quoting
// RAW: stop on a space, respect \escapes, substitute variables.
// WEAK: sub variables? respect escapes?
// STRONG: just go to the end of the 'quotes' for our token
// Returns a NULL-terminated array of the parameters.
// - Allocate an array of strings
// - for each parameter:
// -- find a place to start ("stop character")
// -- selest parameter style based on the char found
// -- pass the command line and a pointer to our position in it.
// -- receive the paramter.
// -- break when the parameter is NULL.
// NOTE This applies variable substition where wanted.
// KILL Variable substitution interferes with unset builtin.
// NOTE This is not an issue, we do the same as bash does.
// NOTE we Initialise / NULLify the end values for memory safety.
char	**quote_aware_split(const char *cmdline, t_env *envt)
{
	char	**params;
	int		p_num;
	int		cmd_pos;

	p_num = 0;
	cmd_pos = 0;
	params = malloc (sizeof (char *) * MAXARGS);
	if (!params)
		return (NULL);
	while ((p_num < MAXARGS))
	{
		goto_stop_char(cmdline, &cmd_pos);
		if (cmdline[cmd_pos] == '\0')
			params[p_num] = NULL;
		else if (is_control_char(cmdline[cmd_pos]) == 1)
			params[p_num] = grab_control_seq(cmdline, &cmd_pos);
		else
			params[p_num] = get_any_parameter(cmdline, &cmd_pos, envt);
		if (params[p_num] == NULL)
			break ;
		p_num++;
	}
	while (p_num < MAXARGS)
		params[p_num++] = NULL;
	return (params);
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
	if (ft_strncmp(arr[0], "|", 1) == 0)
		is_bad = -1;
	while (arr[i] != NULL)
	{
		if (ft_strncmp(arr[i], "<", 1) == 0)
			num_input++;
		if (ft_strncmp(arr[i], ">", 1) == 0)
			num_output++;
		i++;
	}
	if (ft_strncmp(arr[i - 1], "|", 1) == 0)
		is_bad = -1;
	if ((num_input > 1) || (num_output > 1))
		is_bad = -1;
	return (is_bad);
}

// Clear the list of tokens generated from the cmdline.
// FIXED Sometimes this clear unitialised values - set to blank when set?
// Or stop when NULL reached?
void	wipe_tokens(char **arr)
{
	int	i;

	i = 0;
	while (i < MAXARGS)
		free(arr[i++]);
	free (arr);
}

// Returns the number of parsed tokens in the array.
int	count_tokens(char **arr)
{
    int	i;

    i = 0;
    while (arr[i] != NULL)
        i++;
    return (i);
}

// Parse input from cmdline into a command struct
// Returns the first cmd in a list of them.
// Variables:
//  - cmdline:	string from readline
// FIXED Ensure that we free *all* parts of token once cmd is complete
// TODO Give this a more descriptive name; we parse lots of things now.
// NOTE malloc'd variables used in this function:
// - cmdline:  set by readline in main
// - tokens:	list of strings mallocd to a set size
// - cmd_trim:	copy of cmdline without leading / trailing spaces; freed here
t_command	*parse_input(char *cmdline, t_env *envt)
{
	char		**tokens;
	t_command	*cmd_head;

	tokens = quote_aware_split(cmdline, envt);
	free (cmdline);
	if (!tokens)
		return (NULL);
	if (check_tokens(tokens) == -1)
	{
		wipe_tokens(tokens);
		return (NULL);
	}
	cmd_head = build_command_list(tokens);
	wipe_tokens(tokens);
	if (cmd_head->argv[0] == NULL)
		return (NULL);
    return (cmd_head);
}
