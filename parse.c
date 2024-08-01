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

// The more complicated / slower parsing that we need to do to identify
// a builtin.
t_builtin	detailed_parse_builtin(t_command *cmd, int posn)
{
	int	len;

	len = ft_strlen(cmd->argv[posn]);
	if ((ft_strncmp(cmd->argv[posn], "exit", 4) == 0) && (len == 4))
		return (EXIT);
	else if ((ft_strncmp(cmd->argv[posn], "unset", 5) == 0) && (len == 5))
		return (UNSET);
	else if ((ft_strncmp(cmd->argv[posn], "export", 6) == 0) && (len == 6))
		return (EXP);
	else if ((ft_strncmp(cmd->argv[posn], "echo", 4) == 0) && (len == 4))
	{
		if ((cmd->argv[posn + 1]) && (ft_strlen(cmd->argv[posn + 1]) == 2)
			&& (ft_strncmp(cmd->argv[posn + 1], "-n", 2) == 0))
			return (ECHON);
		else
			return (ECHO);
	}
	return (NONE);
}

// Read the given command argument and if it matches a builtin, set the flag.
// (posn will be 0 except when there is input redirection)
// First a sanity check on argument annd position,
// then we check whether the first letter matches one of the builtin names.
// then the shorter names.
// If more complicated parsing is needed, we call detailed_parse_builtin
// NOTE We no longer check if the command has extra arguments
// - the builtin can just ignore them
t_builtin	parse_builtin(t_command *cmd, int posn)
{
	t_builtin	retvalue;
	int			len;

	if ((cmd->argc == 0) || (cmd->argc < posn))
		retvalue = (NONE);
	else
	{
		len = ft_strlen(cmd->argv[posn]);
		if (ft_strchr("cepu", cmd->argv[posn][0]) == NULL)
			retvalue = (NONE);
		else if ((ft_strncmp(cmd->argv[posn], "cd", 2) == 0) && (len == 2))
			retvalue = (CD);
		else if ((ft_strncmp(cmd->argv[posn], "pwd", 3) == 0) && (len == 3))
			retvalue = PWD;
		else if ((ft_strncmp(cmd->argv[posn], "env", 3) == 0) && (len == 3))
			retvalue = ENV;
		else
			retvalue = detailed_parse_builtin(cmd, posn);
	}
	return (retvalue);
}

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
// FIXME Likely quote_aware_split has too many lines.
// KILL Variable substitution interferes with unset builtin.
// NOTE This is not an issue, we do the same as bash does.
// NOTE we Initialise / NULLify the end values for memory safety.
// FIXED export MS_TEST="can have spaces" is being split and should not be.
char	**quote_aware_split(const char *cmdline, t_env *envt)
{
	char	**params;
	int		p_num;
	int		cmd_pos;

	p_num = 0;
	cmd_pos = 0;
	params = malloc (sizeof (char *) * MAXARGS);	// FIXED? These are still not (always?) freed.
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
		{
			params[p_num] = get_any_parameter(cmdline, &cmd_pos, envt);
		}

		/* } */
		/* else if (cmdline[cmd_pos] == '\"') */
		/* { */
		/* 	params[p_num] = get_weak_param(cmdline, &cmd_pos); */
		/* 	if (needs_sub(params[p_num]) != -1) */
		/* 		params[p_num] = substitute_variables(params[p_num]); */
		/* } */
		/* else if (cmdline[cmd_pos] == '\'') */
		/* 	params[p_num] = get_strong_param(cmdline, &cmd_pos); */
		/* else */
		/* { */
		/* 	params[p_num] = get_raw_param(cmdline, &cmd_pos); */
		/* 	if (needs_sub(params[p_num]) != -1) */
		/* 		params[p_num] = substitute_variables(params[p_num]); */
		/* } */
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

int	count_tokens(char **arr)
{
    int	i;

    i = 0;
    while (arr[i] != NULL)
        i++;
    return (i);
}

// Assign tokens to parts of a command struct, include with list.
// When it reaches a pipe character it stops.
// Outside the function must handle ->next
// Return NULL when we finish.
// This would retain the I/O redir but remove the pipes.(replaced with the ->next)
// NOTE We copy the values to make it easier to can wipe all tokens.
t_command	*build_command(char **tokens)
{
	static int	i;
	int			j;
	int			num_tokens;
	t_command	*new_cmd;

	j = 0;
    num_tokens = count_tokens(tokens);
	new_cmd = init_new_command();
	while ((i < num_tokens) && (tokens[i]) && (ft_strncmp(tokens[i], "|", 1) != 0))
	{
		new_cmd->argv[j] = ft_strdup(tokens[i]);
		new_cmd->argc++;
		i++;
		j++;
	}
	new_cmd->argv[new_cmd->argc] = NULL;
	if (tokens[i] == NULL)
		i = 0;
	else
		i++;	// step over the pipe.
    // TODO Break out the builtin parsing to elsewhere.
    // NOTE feeding this ./minishell makes it go to posn 2 and crash
	if ((new_cmd->argv[0]) && (ft_isalpha(new_cmd->argv[0][1]) == 1))
		new_cmd->builtin = parse_builtin(new_cmd, 0);
	else
		new_cmd->builtin = parse_builtin(new_cmd, 2);
	return (new_cmd);
}

// Take an array of strings (tokens from the split cmdline)
// Return the number of pipe characters it contains.
int	count_pipes(char **arr)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	while (arr[i] != NULL)
		if (ft_strncmp(arr[i++], "|", 1) == 0)
			n++;
	return (n);
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
// DONE Create the t_command list explicitly here (not later split)
// FIXME make parse function shorter
// DONE Perhaps the trim of cmdline should happen elsewhere?
// DONE Split out "count pipes" functioning
// DONE Must free tokens if the checks do not pass.
// FIXED Too many variables in parse.
t_command	*parse(char *cmdline, t_env *envt)
{
	char		**tokens;
	t_command	*next_cmd;
	t_command	*cmd_ptr;
	t_command	*cmd_head;
	int			num_pipes;

	tokens = quote_aware_split(cmdline, envt);
	free (cmdline);
	if (!tokens)
		return (NULL);
	print_tokens(tokens);	// HACK for debugging, remove later.
	if (check_tokens(tokens) == -1)
	{
		wipe_tokens(tokens);
		return (NULL);
	}
	num_pipes = count_pipes(tokens);
	cmd_head = build_command(tokens);
	cmd_ptr = cmd_head;
	while (num_pipes > 0)
	{
		next_cmd = build_command(tokens);
		cmd_ptr->next = next_cmd;
		num_pipes--;
		cmd_ptr = cmd_ptr->next;
		print_cmd_parts(next_cmd);	// HACK for debugging, remove later
	}
	wipe_tokens(tokens);
	if (cmd_head->argv[0] == NULL)
		return (NULL);
	print_cmd_parts(cmd_head);	// HACK for debugging
    return (cmd_head);
}
