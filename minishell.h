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
// DONE Add libft, or the needed functions.
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
# include <signal.h>	// catch CRTL-c, CTRL-d, other signals

extern int g_procstatus;	// global variable to hold exit codes

// TODO The name for this should be better.
// TODO SHould the values include pipe, redirect info? Or elsewhere?
typedef enum e_builtin
{
	NONE,
	QUIT,
	JOBS,
	BG,
	FG,
	CD,
	EXIT,
	ECHO,
	ECHON,
	PWD,
	EXP,
	UNSET,
	ENV
}	t_builtin;

// TODO What if we flag command types - system, builtin, what?
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

// substitute.c - variable substitution
char	*ms_strsub(char *str, char *remove, char *replace);
char	*substitute_variables(char *cmd);
char	*get_var_name(const char *str);
int		needs_sub(char *str);

// tokens.c - break up line according to "quote" 'styles'
char	*get_raw_param(const char *cmd, int *posn);
char	*get_weak_param(const char *cmdline, int *posn);
char	*get_strong_param(const char *cmdline, int *posn);

// builtins.c
int		ms_pwd(void);
void	ms_export(t_command *cmd, char **envp);
void	ms_unset(t_command *cmd, char **envp);
void	ms_exit(t_command *cmd);
int		parseBuiltin(t_command *cmd);

// parse.c - functions to read and interpret user input
int		parse(const char *cmdline, t_command *cmd);
void	executeBuiltin(t_command *cmd, char **envp);
char	**quote_aware_split(const char *cmdline);
void	handle_complex_command_structure(t_command *cmd, char **envp);

// paths.c - find and direct programs in PATH
void	run_in_child(t_command *cmd, char **envp, int i_file, int o_file);
void	run_in_child_with_pipe(t_command *cmd, char **envp);
int		direct_output(t_command *cmd, int o_lvl);
int		setup_input(t_command *cmd, int i_lvl);
void	remove_cmd_parts(t_command *cmd, char *target);

// helpers.c - finder and helper functions
char	*search_in_path(char *cmd);
void	goto_stop_char(const char *cmdline, int *posn);
int		find_flow_control(t_command *cmdset);
int		find_env_var(char **envp, const char *var);
void	print_cmd_parts(t_command *cmd)	;

// pipes.c
t_command	*split_pipe(t_command *cmd);
#endif
