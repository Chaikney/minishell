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

// Read the first command argument and if it matches a builtin, set the flag.
// TODO What do we look at in case of input redirection?
// The third parameter, argv[2], would be the one to read.
int parseBuiltin(t_command *cmd)
{
    if (cmd->argc == 0) 
        return (NONE);
    if ((ft_strncmp(cmd->argv[0], "cd", 2) == 0) && (ft_strlen(cmd->argv[0]) == 2))
        return (CD);
    else if ((ft_strncmp(cmd->argv[0], "exit", 4) == 0) && (ft_strlen(cmd->argv[0]) == 4) && (cmd->argv[1] == NULL))
        return (EXIT);
    else if ((ft_strncmp(cmd->argv[0], "echo", 4) == 0)&& (ft_strlen(cmd->argv[0]) == 4) && (cmd->argv[1] != NULL))
    {
        if ((ft_strncmp(cmd->argv[1], "-n", 2) == 0)&& (ft_strlen(cmd->argv[1]) == 2))
            return (ECHO);
        else
            return (ECHON);
    }
    else if ((ft_strncmp(cmd->argv[0], "pwd", 3) == 0) && (ft_strlen(cmd->argv[0]) == 3) && (cmd->argv[1] == NULL))
        return (PWD);
    else if ((ft_strncmp(cmd->argv[0], "export", 6) == 0)&& (ft_strlen(cmd->argv[0]) == 6) && (cmd->argv[1] != NULL))
        return (EXP);
    else if ((ft_strncmp(cmd->argv[0], "unset", 5) == 0)&& (ft_strlen(cmd->argv[0]) == 5))
        return (UNSET);
    else if ((ft_strncmp(cmd->argv[0], "env", 3) == 0)&& (ft_strlen(cmd->argv[0]) == 3) && (cmd->argv[1] == NULL) )
        return (ENV);
    return (NONE);
}

// FIXME echo skips its 1st argument
// FIXME Exit has to be typed twice to work?
// TODO Exit has to do some clean up
// TODO Commands into separate functions, this is too long.
// FIXME Too many lines in function.
// falta por añadir que hace cada
//  [ ] echo with -n (newline or not)
//  [x] cd with only a relative or absolute path
//  [x] pwd (no options)
//  [ ] unset (no options)
//  [x] env, no options or args
//  [ ] export
//  [x] exit no options.
void executeBuiltin(t_command *cmd, char **envp)
{
    int	i;

    while (1) 
    {
        if (cmd->builtin == CD)
        {
            if (cmd->argc < 2) 
                fprintf(stderr, "cd: missing argument\n");
            else 
            {
                if (chdir(cmd->argv[1]) != 0) 
                    printf("wrong address\n");
            }
            return;
        } 
        else if (cmd->builtin == EXIT)
            ms_exit(cmd);
        else if (cmd->builtin == ECHON)
        {
            i = 1;
            while (i < cmd->argc)
            {
                printf("%s", cmd->argv[i]);
                if (i < cmd->argc)
                    printf(" ");
                i++;
            }
            printf("\n");
            return;
        }
        else if (cmd->builtin == ECHO)
        {
            i = 2;
            while (i < cmd->argc)
            {
                printf("%s", cmd->argv[i]);
                if (i < cmd->argc - 1)
                    printf(" ");
                i++;
            }
            return;
        }
        else if (cmd->builtin == PWD)
        {
            ms_pwd();
            return ;
        }
        else if (cmd->builtin == EXP)
        {
            ms_export(cmd, envp);
            return;
        }
        else if (cmd->builtin == UNSET)
        {
            ms_unset(cmd, envp);
            return;
        }
        else if (cmd->builtin == ENV)
        {
            while (*envp) 
            {
                printf("%s\n", *envp);
                envp++;
            }
            return;
        }
        else
        {
            printf("Unknown builtin command\n");
            return;
        }
    }
}

// Substitute a variable's value into the parameter par being prepared.
// par is assumed to have enough space for the strlcat call.
// And was mallocated outside of this call.
// Returns should be the cursor position to continue reading cmdline from.
// or to move it
// TODO I think this is unused and can be removed. No different to the other
// ...except that one works...
int	var_sub(char *par, char *cmdline)
{
	int	i;
	char	*var_name;
	char	*value;

	i = 0;
	if (*cmdline == '$')
	{
		printf("found variable to sub:");// HACK for debugging only
		var_name = get_var_name(cmdline);
		printf("\t%s", var_name);// HACK for debugging only
		if ((value = getenv(var_name)))
		{
			printf("\tIts value is:%s", value);// HACK for debugging only
			ft_strlcat(par, value, ft_strlen(value) + 1);
			printf("\nparameter is now: %s", par);	// HACK for debugging
		}
		i = i + ft_strlen(var_name) + 1;	// NOTE +1 for the $
		free (var_name);
	}
	else
		printf("\nOdd. Variable name not found");
	return (i);
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
// TODO Splitting around > type chars needs to be rethought
// FIXME Note that in bash, >file is acceptable: matters for split!
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
//		printf("stop char found: %c\n", cmdline[cmd_pos]);
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
		printf("added: %s\n", params[p_num]);	// HACK remove debugging statement
//		printf("Stop pos is now %i (%c)", cmd_pos, cmdline[cmd_pos]);
		if (params[p_num] == NULL)
			break ;
		p_num++;
	}
	return (params);
}

// Parse input from cmdline into a command struct
// Return values:
// 0 - no tokens from split
// 1 - Nothing added to the cmd
// cmdline = string from readline
// cmd - t_command to hold tokens from cmdline and other data
// TODO Perhaps the trim of cmdline should happen in quote_aware_split?
// TODO Needs more consistent / clear error conditions
int	parse(const char *cmdline, t_command *cmd)
{
    char	**token;
    int	is_bg;
    char	*cmd_trim;
    
    is_bg = 0;	// HACK for compilation, remove later.
    cmd_trim = ft_strtrim(cmdline, " ");
    if (cmd_trim == NULL)
        perror("command line is NULL\n");
    token = quote_aware_split(cmd_trim);
    if (!token)
        return (0);
    cmd->argc = 0;
    while (token[cmd->argc] != NULL)
    {
        cmd->argv[cmd->argc] = token[cmd->argc];
        cmd->argc++;
        if (cmd->argc >= MAXARGS - 1) {
            break;
        }
    }
    cmd->argv[cmd->argc] = NULL;
    free(token);	// FIXME Ensure this frees *all* parts of token
    if (cmd->argc == 0)
        return (1);
    cmd->builtin = parseBuiltin (cmd);
    free (cmd_trim);
    return (is_bg);
}
