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
// DONE Give struct command a typedef name (and fit Norm).

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAXARGS
#  define MAXARGS 128
# endif

# include <stdio.h>	// perror
# include <stdlib.h>
# include <readline/readline.h>	// readline input
# include <readline/history.h>	// readline history
# include <stdlib.h>	// malloc and free
# include <fcntl.h>	// open
# include <unistd.h>	// access, pipe
# include <sys/wait.h>	// waitpid
# include <errno.h>	// error codes
# include <string.h>	// strerror
# include "libft/libft.h"

typedef struct s_command
{
	int	argc;
	char	*argv[MAXARGS];
	enum builtin_t
	{
		NONE, QUIT, JOBS, BG, FG
	} builtin;
}	t_command;

<<<<<<< HEAD
// builtins.c
char	*get_wd(char **envp);
int ms_pwd(char **envp);

=======
// parse.c - functions to read and interpret user input
int	parse(const char *cmdline, t_command *cmd);

// paths.c - find programs in PATH
>>>>>>> 83aa1aa3cd5c8481bb7d132aad01299e1092d5ef
char	**get_path(char **envp);
char	*find_command(char *cmd, char **envp);
#endif
