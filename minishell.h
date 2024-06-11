/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:48:26 by chaikney          #+#    #+#             */
/*   Updated: 2024/06/06 13:48:36 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO Remove un-needed includes, check against minishell forbidden functions.
// TODO Add libft, or the needed functions.

#ifndef MINISHELL_H
# define MINISHELL_H
#define MAXARGS 128
# include "libft/libft.h"
# include <stdlib.h>	// malloc and free
# include <stdio.h>	// perror
# include <fcntl.h>	// open
# include <unistd.h>	// access, pipe
# include <sys/wait.h>	// waitpid
# include <errno.h>	// error codes
# include <string.h>	// strerror


struct command 
{
    int argc;
    char *argv[MAXARGS];
    enum builtin_t 
    {
        NONE, QUIT, JOBS, BG, FG 
    } builtin;
};

// builtins.c
char	*get_wd(char **envp);
int ms_pwd(char **envp);

char	**get_path(char **envp);
char	*find_command(char *cmd, char **envp);
#endif
