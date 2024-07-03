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

// Return one single parameter from a command line.
// TODO Add variable substitution.
// - allocate arbitrary space
// - if we are at the end, reset the static and return NULL to finish.
// - step over initial spaces, avoiding the end marker
// - if we find a $ substitute it NO, later
// - copy characters
// - null-term and return
// TODO Tests for other characters?
char	*get_raw_param(const char *cmd, int *posn)
{
	int		i;
	char		*par;

	if ((cmd[*posn] == ' ') || (cmd[*posn] == '\0'))
		printf("\t*** entered raw_param at incorrect char: %c", cmd[*posn]);
	i = 0;
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	// TODO Consider if this should stop at a ' or " (they would be misplaced, but...)
	while ((cmd[*posn] != '\0') && (cmd[*posn] != ' '))
	{
		par[i] = cmd[*posn];
		i++;
		(*posn)++;
	}
	par[i] = '\0';	// TODO Make the other 2 null-terminate their param
	// TODO Does raw param need to step over anything?
//	printf("returning: %s\t", par);
	return (par);
}

// Substitute a variable's value into the parameter par being prepared.
// par is assumed to have enough space for the strlcat call.
// And was mallocated outside of this call.
// Returns should be the cursor position to continue reading cmdline from.
// or to move it
// FIXME Get clear what to return, this gives nonsense back.
// Is is because par is not null-terminated at the time we pass it?
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

// Return a "weakly-quoted" parameter from cmdline.
// (Weakly quoted = parameter expansion happens, space separation doesn't)
// - Find the first double quote
// - Start copying characters until the end quote
// - Expand any variable found
// -- get its name and value
// -- if value isn't null, copy value to par
// -- move cmdline forward the length of var_name
// TODO var sub part has to be handled somewhere.
char	*get_weak_param(const char *cmdline, int *posn)
{
	char	*par;
	int	j;

	if (cmdline[*posn] != '\"')
		printf("\t*** entered weak_param at incorrect char: %c", cmdline[*posn]);
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	j = 0;
	(*posn)++;	// Step past the first
	while ((cmdline[*posn] != '\"')&& (cmdline[*posn] != '\0'))
	{
		par[j] = cmdline[*posn];
		j++;
		(*posn)++;
	}
	if (cmdline[*posn] != '\0')
		(*posn)++;	// NOTE step past the final ' if safe
//	printf("\nWeak quoting parameter to return: %s", par);	// HACK for debugging only
	return (par);
}

// return a 'strongly-quoted' parameter from cmdline.
// (Strongly quoted = no escaping or substituting).
// - Find the first single quote
// - start copying characters until the next quote
// NOTE This version does not allow for escaped quotes!
char	*get_strong_param(const char *cmdline, int *posn)
{
	char	*par;
	int	j;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	j = 0;
	if (cmdline[*posn] != '\'')
		printf("\t*** entered strong_param at incorrect char: %c", cmdline[*posn]);
	(*posn)++;	// NOTE Step past the first ' to start copying
	while ((cmdline[*posn] != '\'') && (cmdline[*posn] != '\0'))
	{
		par[j] = cmdline[*posn];
		j++;
		(*posn)++;
	}
	if (cmdline[*posn] != '\0')
		(*posn)++;	// NOTE step past the final ' if safe
	return (par);
}

// Moves the *posn pointer forward through a command line
// until it reaches a stop character (which is badly named because we
// use this to find a place to *start* parsing).
// The stop chars are null or any non-space character.
// (The quote styles are matched for in the main loop.)
void	find_stop_char(const char *cmdline, int *posn)
{
	while ((cmdline[*posn] != '\0') && (cmdline[*posn] == ' '))
		(*posn)++;
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
		find_stop_char(cmdline, &cmd_pos);
//		printf("stop char found: %c\n", cmdline[cmd_pos]);
		if (cmdline[cmd_pos] == '\0')
			params[p_num] = NULL;
		else if (cmdline[cmd_pos] == '\"')
		{
			params[p_num] = get_weak_param(cmdline, &cmd_pos);
			// also call var sub?
		}
		else if (cmdline[cmd_pos] == '\'')
			params[p_num] = get_strong_param(cmdline, &cmd_pos);
		else
		{
			params[p_num] = get_raw_param(cmdline, &cmd_pos);
			// also call var sub?
		}
//		printf("added: %s\n", params[p_num]);	// HACK remove debugging statement
//		printf("Stop pos is now %i (%c)", cmd_pos, cmdline[cmd_pos]);
		if (params[p_num] == NULL)
			break ;
		p_num++;
	}
	return (params);
}

// Parse input from cmdline into a command struct
// Return values:
// 1 - ?
// NOTE Understand why we have here both cmdline and cmd
// ...the tokens are put into a cmd struct, and *this* is
// what we then execute.
// TODO Perhaps the trim of cmdline should happen in quote_aware_split?
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
