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
// TODO static may be better in the outer loop?
// - allocate arbitrary space
// - if we are at the end, reset the static and return NULL to finish.
// - step over initial spaces, avoiding the end marker
// - if we find a $ substitute it
// - copy chaarcters  - FIXME?
// - null-term and return
// TODO report position using the posn pointer
char	*get_raw_param(const char *cmd, int *posn)
{
	int		i;
	static int	j;
	char		*par;
    int	step;

    (void) posn;
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
        if (cmd[j] == '$')
        {
            step = var_sub(&par[i], (char *) &cmd[j]);
            j = j + step;
            printf("\npar is now: %s", par);
            // need to move par (abck?) to the end.
        }
        // TODO Tests for other characters
		par[i++] = cmd[j++];
	}
	par[i] = '\0';
	printf("returning: %s\t", par);
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
// FIXME Function too long (probably)
// TODO The var sub part really should be standalone.
// ...how to handle the skipping forward though?
// FIXME doesn't work with "$PWD" - odd extra characters either side?
// TODO What does this one do with spaces?
// TODO report position using the posn pointer
char	*get_weak_param(const char *cmdline, int *posn)
{
	char	*par;
	int	i;
	int	j;
	/* char	*var_name; */
	/* char	*value; */
    int	step;

    (void) posn;	// HACK to compile
	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	ft_bzero(par, 256);
	i = 0;
	j = 0;
	while (cmdline[i] != '\"')
		i++;
	i++;
	while (cmdline[i] != '\"')
	{
		if (cmdline[i] == '$')
		{
            step = var_sub(&par[i], (char *) &cmdline[j]);
            j = j + step;
            printf("\npar is now: %s", par);
 		/* 	printf("found variable to sub:");// HACK for debugging only */
		/* 	var_name = get_var_name(&cmdline[i]); */
		/* 	printf("\t%s", var_name);// HACK for debugging only */
		/* 	if ((value = getenv(var_name))) */
		/* 	{ */
		/* 		printf("\tIts value is:%s", value);// HACK for debugging only */
		/* 		ft_strlcat(par, value, ft_strlen(value) + 1); */
		/* 		j = j + ft_strlen(value); */
		/* 	} */
		/* 	i = i + ft_strlen(var_name) + 1;	// NOTE +1 for the $ */
		/* 	free (var_name); */
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
// TODO report position using the posn pointer
// FIXME Here a space should not signal a split (might be outerloop prob)
char	*get_strong_param(const char *cmdline, int *posn)
{
	char	*par;
	int	j;

	par = malloc(sizeof(char) * 256);
	if (!par)
		return (NULL);
	j = 0;
	while (cmdline[*posn] != '\'')	// FIXME Invalid read here.
		(*posn)++;
    (*posn)++;
	while ((cmdline[*posn] != '\'') && (cmdline[*posn] != '\0'))	// FIXED? Created an epic number of read errors
	{
		par[j++] = cmdline[*posn];
        (*posn)++;
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
// Take a command line and return a NULL-terminated array of its parameters.
// FIXME The loop and selection are disastrous
char	**better_split(const char *cmdline)
{
	char	**params;
	int	p_num;
    int	cmd_pos;

	p_num = 0;
    cmd_pos = 0;
	params = malloc (sizeof (char *) * MAXARGS);
	if (!params)
		return (NULL);
   // FIXME Will need to handle stepping forward - pointer to cmd_pos?
    while ((p_num < MAXARGS))
    {
        if (cmdline[cmd_pos] == '\"')
            params[p_num] = get_weak_param(cmdline, &cmd_pos);
        else if (cmdline[cmd_pos] == '\'')
            params[p_num] = get_strong_param(cmdline, &cmd_pos);
        else
            params[p_num] = get_raw_param(cmdline, &cmd_pos);
        printf("added: %s\n", params[p_num]);	// HACK remove debugging statement
        if (params[p_num] == NULL)
            break ;
        p_num++;
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
