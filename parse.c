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

// Read the given command argument and if it matches a builtin, set the flag.
// (posn will be 0 except when there is input redirection)
// FIXME Lines are too long for norminette
t_builtin parse_builtin(t_command *cmd, int posn)
{
	if (cmd->argc == 0)
		return (NONE);
	if ((ft_strncmp(cmd->argv[posn], "cd", 2) == 0) && (ft_strlen(cmd->argv[posn]) == 2))
		return (CD);
	else if ((ft_strncmp(cmd->argv[posn], "exit", 4) == 0) && (ft_strlen(cmd->argv[posn]) == 4) && (cmd->argv[posn + 1] == NULL))
		return (EXIT);
	else if ((ft_strncmp(cmd->argv[posn], "echo", 4) == 0)&& (ft_strlen(cmd->argv[posn]) == 4) && (cmd->argv[posn + 1] != NULL))
	{
		if ((ft_strncmp(cmd->argv[posn + 1], "-n", 2) == 0)&& (ft_strlen(cmd->argv[posn + 1]) == 2))
			return (ECHON);
		else
			return (ECHO);
	}
	else if ((ft_strncmp(cmd->argv[posn], "pwd", 3) == 0) && (ft_strlen(cmd->argv[posn]) == 3) && (cmd->argv[posn + 1] == NULL))
		return (PWD);
	else if ((ft_strncmp(cmd->argv[posn], "export", 6) == 0)&& (ft_strlen(cmd->argv[posn]) == 6) && (cmd->argv[posn + 1] != NULL))
		return (EXP);
	else if ((ft_strncmp(cmd->argv[posn], "unset", 5) == 0)&& (ft_strlen(cmd->argv[posn]) == 5))
		return (UNSET);
	else if ((ft_strncmp(cmd->argv[posn], "env", 3) == 0)&& (ft_strlen(cmd->argv[posn]) == 3) && (cmd->argv[posn + 1] == NULL) )
		return (ENV);
	return (NONE);
}

// Check value of cmd->builtin and direct to desired function.
// falta por añadir que hace cada
//  [x] echo with -n (newline or not)
//  [x] cd with only a relative or absolute path
//  [x] pwd (no options)
//  [x] unset (no options)
//  [x] env, no options or args
//  [x] export
//  [x] exit no options.
void executeBuiltin(t_command *cmd, char **envp)
{
	if (cmd->builtin == CD)
        ms_cd(cmd, envp);
	else if (cmd->builtin == EXIT)
		ms_exit(cmd);
	else if ((cmd->builtin == ECHON) || (cmd->builtin == ECHO))
		ms_echo(cmd);
	else if (cmd->builtin == PWD)
		ms_pwd();
	else if (cmd->builtin == EXP)
		ms_export(cmd, envp);
	else if (cmd->builtin == UNSET)
		ms_unset(cmd, envp);
	else if (cmd->builtin == ENV)
        ms_env(envp);
	else
	{
		printf("Unknown builtin command\n");
		return;
	}
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
char	**quote_aware_split(const char *cmdline)
{
	char	**params;
	int	p_num;
	int	cmd_pos;

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
		else if (cmdline[cmd_pos] == '\"')
		{
			params[p_num] = get_weak_param(cmdline, &cmd_pos);
            if (needs_sub(params[p_num]) != -1)
                params[p_num] = substitute_variables(params[p_num]);
		}
		else if (cmdline[cmd_pos] == '\'')
			params[p_num] = get_strong_param(cmdline, &cmd_pos);
		else
		{
			params[p_num] = get_raw_param(cmdline, &cmd_pos);
            if (needs_sub(params[p_num]) != -1)
                params[p_num] = substitute_variables(params[p_num]);
		}
		if (params[p_num] == NULL)
			break ;
		p_num++;
	}
	return (params);
}

// debugging aid: print a list of strings.
void	print_tokens(char **arr)
{
    int 	i;

    i = 0;
    printf("\nAnalysing token list.");
    while ((arr[i]) && (i < MAXARGS))
    {
        printf("\ntoken %i:\t%s", i, arr[i]);
        i++;
    }
}

// Return a -1 if there are illegal combinations of tokens
// e.g. multiple attempts at redirection for either I or O
// TODO Implement sanity checks on token set.
// [x]	do not start with a pipe
// [ ]	do not end with a pipe
// [ ]	do not have both > and >>
// [ ]	do not have both < and <<
// [ ]	do not have more than one input directive
// [ ]	do not have more than one output directive
int	check_tokens(char **arr)
{
	if (ft_strncmp(arr[0], "|", 1) == 0)
		perror("bad command format");	// TODO Throw appropriate error here.
    return (0);
}

// Clear the list of tokens generated from the cmdline.
// FIXME Sometimes this clear unitialised values - set to blank when set?
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
// When it reaches a pipe character it stops. Outside the function must handle ->next
// Return NULL when we finish.
// This would retain the I/O redir but remove the pipes.(replaced with the ->next)
t_command	*build_command(char **tokens)
{
	static int	i;
	int		j;
	int		num_tokens;
	t_command	*new_cmd;

	j = 0;
    num_tokens = count_tokens(tokens);
	/* new_cmd = malloc(sizeof(t_command));	// FIXME Memory allocated here is not freed */
	/* new_cmd->argc = 0; */
	/* new_cmd->builtin = NONE; */
	/* new_cmd->next = NULL; */
	new_cmd = init_new_command();
	while ((i < num_tokens) && (tokens[i]) && (ft_strncmp(tokens[i], "|", 1) != 0))
	{
		new_cmd->argv[j] = ft_strdup(tokens[i]);	// NOTE if we copy the values, can wipe all tokens
		new_cmd->argc++;
		i++;
		j++;
	}
	// we have reached the end, or a pipe.
	new_cmd->argv[new_cmd->argc] = NULL;
//	new_cmd.argv[j] = NULL;	// NOTE Should we also inc argc? Does the null count?
	if (tokens[i] == NULL)
		i = 0;
	else
		i++;	// step over the pipe.
    // TODO Break out the builtin parsing to elsewhere.
	if ((new_cmd->argv[0]) && (ft_isalpha(new_cmd->argv[0][1]) == 1))
		new_cmd->builtin = parse_builtin(new_cmd, 0);
	else
		new_cmd->builtin = parse_builtin(new_cmd, 2);
	return (new_cmd);
}

// Parse input from cmdline into a command struct
// Return values:
// 0 - command parsed successfully
// -1 - parsing failed
// Variables:
//  - cmdline:	string from readline
//  - cmd:		t_command to hold tokens from cmdline and other data
// TODO Perhaps the trim of cmdline should happen in quote_aware_split?
// FIXME Ensure that we free *all* parts of token once cmd is complete
// TODO Give this a more descriptive name; we parse lots of things now.
// DONE Better name and defined purpose for is_bg variable.
// NOTE malloc'd variables used in this function:
// - cmdline:  set by readline in main
// - tokens:	list of strings mallocd to a set size
// - cmd_trim:	copy of cmdline without leading / trailing spaces; freed here
// - cmd:		not mallocd, an uninitialised address from eval.
// Need consistency in t_comamnds. The split ones are mallocd, the one here (base) is not.
// So hard to know what can be freed safely.
// TODO Create the t_command list explicitly here (not later split)
//int	parse(const char *cmdline, t_command *cmd)
t_command	*parse(const char *cmdline)
{
    char	**tokens;
    int	is_parsed;
    char	*cmd_trim;
    t_command	*next_cmd;
    t_command	*cmd_ptr;
    t_command	*cmd_head;
    int	i;
    int	num_pipes;

//    (void) cmd;	// HACK for debugging;
    is_parsed = -1;
    cmd_trim = ft_strtrim(cmdline, " ");
    if (cmd_trim == NULL)
        perror("command line is NULL\n");
    tokens = quote_aware_split(cmd_trim);
    free (cmd_trim);
    if (!tokens)
        return (NULL);
    print_tokens(tokens);
	i = 0;
    num_pipes = 0;
	while (tokens[i] != NULL)
		if (ft_strncmp(tokens[i++], "|", 1) == 0)
			num_pipes++;
    cmd_head = build_command(tokens);
    cmd_ptr = cmd_head;

//    cmd = &new_cmd;	// NOTE Marking the first command in the set.
    i = 1;
    while (i <= num_pipes)
    {
        next_cmd = build_command(tokens);
        cmd_ptr->next = next_cmd;
        i++;
        cmd_ptr = cmd_ptr->next;
        print_cmd_parts(next_cmd);
    }
    /* cmd->argc = 0; */
    /* while (tokens[cmd->argc] != NULL) */
    /* { */
    /*     cmd->argv[cmd->argc] = tokens[cmd->argc]; */
    /*     cmd->argc++; */
    /*     if (cmd->argc >= MAXARGS - 1) */
    /*         break ; */
    /* } */
    /* cmd->argv[cmd->argc] = tokens[cmd->argc];	// which i take to be the null */
//    cmd->argv[cmd->argc] = NULL;
//
    wipe_tokens(tokens);

    if (cmd_head->argv[0] == NULL)
        return (NULL);
    print_cmd_parts(cmd_head);	// HACK for debugging
    is_parsed = 0;
    if (is_parsed == 0)
        return (cmd_head);
    else {
        return (NULL);
    }
//    return (is_parsed);
}
