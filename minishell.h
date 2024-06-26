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
// DONE Separate the builtin_t definition
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

// FIXME The name for this should be better.
// TODO Understand what these values *should* be
typedef enum e_builtin
{
	NONE,
	QUIT,
	JOBS,
	BG,
	FG
}	t_builtin;

typedef struct s_command
{
	int			argc;
	char		*argv[MAXARGS];
	t_builtin	builtin;
}	t_command;

// various functions
void	exit_and_free(char **args, int fd_in, int fd_out);
void	clear_t_command(t_command *cmd);
void	run_command(t_command *cmd, char **envp);
void	eval(char *cmdline, char **envp);
char	*get_prompt(void);

// builtins.c
int		ms_pwd(void);

// parse.c - functions to read and interpret user input
int		parse(const char *cmdline, t_command *cmd);
void	executeBuiltin(t_command *cmd, char **envp);

// paths.c - find and direct programs in PATH
char	*find_command(char *cmd);
void	run_in_child(t_command *cmd, char **envp);
void	run_in_child_with_pipe(t_command *cmd, char **envp);
#endif
