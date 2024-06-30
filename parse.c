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


// DONE Change strcspn calls to another function
// DONE Change strcspn calls to another function
// DONE Implement parseBuiltIn - simple match against the list of builtins?
// Parse input from cmdline into a command struct
// Return values:
// 1 - ?
int	parse(const char *cmdline, t_command *cmd)
{
    char 	**token;
    int 	is_bg;
    
    is_bg = 0;	// HACK for compilation, remove later.
    if (cmdline == NULL)
        perror("command line is NULL\n");
    
    token = ft_split(cmdline, ' ');
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
    return (is_bg);
}
