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
// FIXME Note that in bash, >file is acceptable: matters for split!
// TODO Splitting around > type chars needs to be rethought
// KILL Variable substitution interferes with unset builtin.
// NOTE This is not an issue, we do the same as bash does.
char	**quote_aware_split(const char *cmdline)
{
	char	**params;
	int	p_num;
	int	cmd_pos;

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
// - token:
// - cmd_trim: copy of cmdline without leading / trailing spaces; freed here
int	parse(const char *cmdline, t_command *cmd)
{
    char	**token;
    int	is_parsed;
    char	*cmd_trim;
    
    is_parsed = -1;
    cmd_trim = ft_strtrim(cmdline, " ");
    if (cmd_trim == NULL)
        perror("command line is NULL\n");
    token = quote_aware_split(cmd_trim);
    if (!token)
        return (is_parsed);
    cmd->argc = 0;
    while (token[cmd->argc] != NULL)
    {
        cmd->argv[cmd->argc] = token[cmd->argc];
        cmd->argc++;
        if (cmd->argc >= MAXARGS - 1)
            break ;
    }
    cmd->argv[cmd->argc] = NULL;
    free(token);
    if (cmd->argc == 0)
        return (is_parsed);
    cmd->next = NULL;	// NOTE Without this setup, segfaults all over.
//     print_cmd_parts(cmd);	// HACK for debugging
    cmd->builtin = parse_builtin (cmd, 0);	// HACK Hardcoding; should ensure no redirection present.
    free (cmd_trim);
    is_parsed = 0;
    return (is_parsed);
}
