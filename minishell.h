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

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAXARGS
#  define MAXARGS 128
# endif

# include <stdio.h>	// perror
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

// global variable to hold exit codes
extern int	g_procstatus;

// An enum covering the builtin functions that our shell provides.
typedef enum e_builtin
{
	NONE,
	CD,
	EXIT,
	ECHO,
	ECHON,
	PWD,
	EXP,
	UNSET,
	ENV
}	t_builtin;

// Holds details of command:
// - argc - number of arguments
// - argv - the command and its arguments
// - builtin - whether the command is one of our builtin functions
// - next - NULL, or a pointer to the next command in a pipeline.
typedef struct s_command
{
	struct s_command	*next;
	int			argc;
	char		*argv[MAXARGS];
	t_builtin	builtin;
}	t_command;

// various functions
void	eval(char *cmdline, char **envp);
int		determine_output(t_command *cmd);

// substitute.c - variable substitution
char	*ms_strsub(char *str, char *remove, char *replace);
char	*substitute_variables(char *cmd);
char	*get_var_name(const char *str);
int		needs_sub(char *str);

// tokens.c - break up line according to "quote" 'styles'
char	*get_raw_param(const char *cmd, int *posn);
char	*get_weak_param(const char *cmdline, int *posn);
char	*get_strong_param(const char *cmdline, int *posn);
char	*grab_control_seq(const char *cmd, int *posn);

// builtins.c
int			ms_pwd(void);
void		ms_export(t_command *cmd, char **envp);
void		ms_unset(t_command *cmd, char **envp);
t_builtin	parse_builtin(t_command *cmd, int posn);
void		ms_echo(t_command *cmd);
void		ms_unset_export(char *unset_var, char **envp);
void		ms_export_display(char **envp);

// parse.c - functions to read and interpret user input
t_command	*parse(char *cmdline);
char	**quote_aware_split(const char *cmdline);
void	direct_complex_command(t_command *cmd, char **envp);

// input.c - functions for redirecting input
int		stopword_input(t_command *cmd);
int		setup_input(t_command *cmd, int i_lvl);
int		determine_input(t_command *cmd);

// paths.c - find and direct programs in PATH
void	run_final_cmd(t_command *cmd, char **envp, int i_file, int o_file);
void	run_in_pipe(t_command *cmd, char **envp, int *i_file);
void	remove_cmd_parts(t_command *cmd, char *target);

// helpers.c - finder and helper functions
char	*search_in_path(char *cmd);
void	goto_stop_char(const char *cmdline, int *posn);
int		find_flow_control(t_command *cmdset);
int		find_env_var(char **envp, const char *var);
int		is_legal_name(char *str);
int		is_control_char(char c);

// pipes.c
t_command	*split_pipe(t_command *cmd);
t_command	*make_cmd_list(t_command *to_split, int n);
t_command	*init_new_command(void);

// signals.c - catch and manage signals like CTRL-C
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
int		setup_signals(void);

// exit.c - freeing memory and exiting cleanly
void	clear_t_command(t_command *cmd);
void	ms_exit(t_command *cmd);
void	exit_successful_pipe(t_command *cmd);
void	exit_failed_pipe(t_command *cmd, int fd_in, int fd_out);

// env.c - just that for now
void	ms_env(char **envp);

// cd.c - change wd, set new value of PWD
void	ms_cd(t_command *cmd, char **envp);
void	ms_export_cd(char **envp, char *oldpwd, char *newpwd);

// prompt.c - functions to enhance the readline prompt
char	*get_shrt_wd(void);
char	*get_status_for_prompt(void);
char	*get_prompt(void);

// debug.c - functions to use in development
void	print_tokens(char **arr);
void	print_cmd_parts(t_command *cmd);

// execute.c -functions to execute commands
void	run_command(t_command *cmd, char **envp);
void	execute_builtin(t_command *cmd, char **envp);
#endif
