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
// TODO Separate the builtin_t definition
// TODO Give struct command a typedef name (and fit Norm).

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAXARGS
#  define MAXARGS 128
# endif

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

// parse.c - functions to read and interpret user input
int parse(const char *cmdline, struct command *cmd);

// paths.c - find programs in PATH
char	**get_path(char **envp);
char	*find_command(char *cmd, char **envp);
#endif
