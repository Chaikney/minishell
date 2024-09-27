/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:16:53 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/24 14:55:28 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Functions to parse the command line into an array of
// strings / tokens.

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

// Takes cmdline string (previously trimmed).
// Checks that all quotes are closed.
// Splits cmdline into list of tokens.
// Those tokens undergo some logic / structure checks
// If they pass, the tokens are parsed into a list of command structs.
// Returns a pointer to the first t_command in the set.
// Or NULL if there is a failure at any point.
// NOTE malloc'd variables used in this function:
// - cmdline:  set by readline in main
// - tokens:	list of strings mallocd to a set size (freed in parse_input2)
t_command	*parse_input(char *cmdline, t_env *envt)
{
	char		**tokens;
	t_command	*cmd_head;
	int			run_or_not;

	run_or_not = 0;
	run_or_not = closed_quotes(cmdline);
	if (run_or_not == 0)
	{
		free (cmdline);
		return (NULL);
	}
	tokens = quote_aware_split(cmdline, envt);
	free (cmdline);
	if (!tokens)
		return (NULL);
	if (check_tokens(tokens) == -1)
	{
		wipe_tokens(tokens);
		perror("malformed input. Try bash or tee. Or stop messing about");
		return (NULL);
	}
	cmd_head = tokens_to_list(tokens);
	return (cmd_head);
}
