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
            return (ECHON);
        else
            return (ECHO);
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

// falta por añadir que hace cada 
// TODO? Need to pass envp here for some of the commands, or maybe not if we 
// FIXME cmd builtin == 1 I think its wrong
// FIXME echo causes segfault
// FIXME Exit has to be typed twice to work?
// TODO Exit has to do some clean up
// TODO Commands into separate functions, this is too long.
//  [ ] echo with -n (newline or not)
//  [ ] cd with only a relative or absolute path
//  [x] pwd (no options)
//  [ ] unset (no options)
//  [ ] env, no options or args
//  [ ] exit no options.
void executeBuiltin(t_command *cmd, char **envp)
{
    (void) envp;	// HACK for compilation, remove this or parameter later.
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
            exit(0);
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

// Return  the name of a $variable for later substitution
// - check we are at $
// - count length (until a space)
// - alloc a string
// - copy characters
// (alt: use the substr functions?)
// NOTE Returns the correct var_name without the $
char	*get_var_name(const char *str)
{
	int	name_len;
	char	*ptr;
	char	*var_name;

	ptr = (char *) str;
	if (*ptr != '$')
	{
		printf("Tried to find a variable but you passed the wrong position");
		return (NULL);
	}
    ptr++;
	name_len = 0;
	while ((*ptr != ' ') && (*ptr != '\"') && (*ptr != '\0'))
    {
		name_len++;
        ptr++;
    }
	var_name = malloc(sizeof(char) * (name_len + 1));
	if (!var_name)
		return (NULL);
	var_name[name_len] = '\0';
	while (name_len > 0)
	{
        ptr--;
		var_name[name_len - 1] = *ptr;
		name_len--;
	}
	return (var_name);
}

// Return one single parameter from a command line.
// TODO Can I wrap the substitute_variables call to make it work in here?
char	*get_param(const char *cmd)
{
	int		i;
	static int	j;
	char		*par;

	i = 0;
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	if (cmd[j] == '\0')
	{
		j = 0;
		return (NULL);
	}
	while ((cmd[j] != '\0'))
	{
		if (cmd[j] == ' ')
		{
            while ((cmd[j] == ' ') && (cmd[j] != '\0'))
                j++;
			break ;
		}
        // TODO Tests for other characters
		par[i++] = cmd[j++];
	}
	par[i] = '\0';
	printf("returning: %s\t", par);
	return (par);
}

// Return a "weakly-quoted" parameter from cmdline.
// (Weakly quoted = parameter expansion happens, space separation doesn't)
// - Fund the first double quote
// - Start copying characters until the end quote
// - Expand any variable found
// TODO Implement variable expansion.
// TODO Handle NULL returns from getenv: add nothing, remove the $
// TODO Free var_name and value
char	*get_weak_param(const char *cmdline)
{
	char	*par;
	int	i;
	int	j;
    char	*var_name;
    char	*value;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	i = 0;
	j = 0;
	while (cmdline[i] != '\"')
		i++;
	i++;
	while (cmdline[i] != '\"')
	{
        if (cmdline[i] == '$')
        {
            printf("found variable to sub:");// HACK for debugging only
            var_name = get_var_name(&cmdline[i]);
            printf("\t%s", var_name);// HACK for debugging only
            value = getenv(var_name);
            printf("\tIts value is:%s", value);// HACK for debugging only
            // copy value to par
            ft_strlcat(par, value, ft_strlen(value) + 1);
            // move cmdline forward the length of var_name
            i = i + ft_strlen(var_name) + 1;	// NOTE +1 for the $
            j = j + ft_strlen(value);
        }
        else
            par[j++] = cmdline[i++];
	}
    printf("\nWeak quoting parameter to return: %s", par);	// HACK for debugging only
    return (par);
}

// return a 'strongly-quoted' parameter from cmdline.
// (Strongly quoted = no esscaping or substituting).
// - Find the first single quote
// - start copying characters until the next quote
// NOTE This version does not allow for escaped quotes!
// TODO Protect against hitting the end of the string.
// FIXME segfaults, probably as noted above...
char	*get_strong_param(const char *cmdline)
{
	char	*par;
	int	i;
	int	j;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	i = 0;
	j = 0;
	while (cmdline[i] != '\'')	// FIXME Invalid read here.
		i++;
	i++;
	while (cmdline[i] != '\'')
	{
		par[j++] = cmdline[i++];
	}
	return (par);
}

// better command line split / tokenising
// go char by char, consious of quoting and escaping
// 3 modes: raw, "weak" quoting and 'strong' quoting
// RAW: stop on a space, respect \escapes, substitute variables.
// WEAK: sub variables? respect escapes?
// STRONG: just go to the end of the quotes for our token?
// NOTE on each case what do we do with the leftover 'symbols'?
// TODO check quoting definitions and make them clear to us all.
// Take a command line and return a NULL-terminated array of its parameters.
// TODO Check definitions again! What is strong, what is weak?
char	**better_split(const char *cmdline)
{
	char		**params;
	int		i;
	t_quote	quote_mode;

	i = 0;
	params = malloc (sizeof (char *) * MAXARGS);
	if (!params)
		return (NULL);
	if (ft_strchr(cmdline, '\'') != NULL)
		quote_mode = STRONG;
	else if ((ft_strchr(cmdline, '\"') != NULL))
		quote_mode = WEAK;
	else
		quote_mode = RAW;
	if (quote_mode == RAW)
	{
		params[i] = get_param(cmdline);
		printf("added: %s\n", params[i]);	// HACK remove debugging statement
		while ((params[i] != NULL) && (i < MAXARGS))
		{
			i++;
			params[i] = get_param(cmdline);
			printf("added: %s\n", params[i]);	// HACK remove debugging statement
		}
	}
	else
    {
		printf("TODO: Quoting style not implemented\n");
        return (NULL);
    }
	return (params);
}

// Parse input from cmdline into a command struct
// Return values:
// 1 - ?
// TODO More sophisticated tokenisation / split needed.
// NOTE Understand why we have here both cmdline and cmd
// ...the tokens are put into a cmd struct, and *this* is
// what we then execute.
// TODO Perhaps the trim of cmdline should happen in better_split?
int	parse(const char *cmdline, t_command *cmd)
{
    char	**token;
    int	is_bg;
    char	*cmd_trim;
    
    is_bg = 0;	// HACK for compilation, remove later.
    cmd_trim = ft_strtrim(cmdline, " ");
    if (cmd_trim == NULL)
        perror("command line is NULL\n");
    token = better_split(cmd_trim);
//    token = ft_split(cmdline, ' ');
    if (!token)
        return (0);
    cmd->argc = 0;
    // NOTE Find out what this structure does.
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
