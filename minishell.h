/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:48:26 by chaikney          #+#    #+#             */
/*   Updated: 2024/08/08 13:14:13 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAXARGS
#  define MAXARGS 128
# endif

# ifndef MAXPARAM
#  define MAXPARAM 512
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
# include <sys/stat.h>	// stat, lstat and associated macros.

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
	int					argc;
	char				*argv[MAXARGS];
	t_builtin			builtin;
}	t_command;

typedef struct s_env
{
	char			*vname;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

// Early stage functions
void		eval(char *cmdline, t_env *envt);
t_env		*parse_env(char **envp);

// substitute.c - variable substitution
char		*ms_strsub(char *str, char *remove, char *replace);
char		*get_var_name(const char *str);
int			add_value_to_par(char **par, int *pos, const char *cmd, t_env *env);
int			examine_var(char **par, int *r_pos, const char *cmd, t_env *envt);

// tokens.c - break up line according to "quote" 'styles'
char		*grab_control_seq(const char *cmd, int *posn);
char		*get_any_parameter(const char *cmdline, int *posn, t_env *envt);

// builtins.c
int			ms_pwd(void);
t_builtin	parse_builtin(t_command *cmd, int posn);
void		ms_unset(t_command *cmd, t_env *enviro);
void		ms_echo(t_command *cmd);
int			echon(t_command *cmd, int i);
void		int_unset(char *unset_var, t_env *enviro);

// ms_export.c - and related helpers
void		ms_export_display_t(t_env *envt);
void		ms_export_t(t_command *cmd, t_env **envt);

// parse.c - functions to read and interpret user input
t_command	*parse_input(char *cmdline, t_env *envt);
char		**quote_aware_split(const char *cmdline, t_env *envt);
int			count_tokens(char **arr);
void		wipe_tokens(char **arr);

// checks.c - sanity checks on token order
int			check_tokens(char **arr);
int			proximity_test(char **arr);

// input.c - functions for redirecting input
int			stopword_input(t_command *cmd, int fd[2], int posn);
int			setup_input(t_command *cmd, int i_lvl, int posn);
int			determine_input(t_command *cmd);

// paths.c - find and direct programs in PATH
void		run_final_cmd(t_command *cmd, int i_file, int o_file, t_env *envt);
void		direct_complex_command(t_command *cmd, t_env *envt);
int			run_in_pipe(t_command *cmd, int *i_file, t_env *envt);
void		remove_cmd_parts(t_command *cmd, char *target);
int			determine_output(t_command *cmd);

// helpers.c - finder and helper functions
void		goto_stop_char(const char *cmdline, int *posn);
int			is_legal_name(char *str);
int			is_control_char(char c);
char		*get_export_name(char *str);
char		*get_export_value(char *str);

// pipes.c
t_command	*init_new_command(void);
t_command	*build_command(char **tokens);
t_command	*build_command_list(char **tokens);
int			count_pipes(char **arr);

// signals.c - catch and manage signals like CTRL-C
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
int			setup_signals(void);

// exit.c - freeing memory and exiting cleanly
void		clear_t_command(t_command *cmd);
void		ms_exit(t_command *cmd, t_env *envt);
void		clear_environment(t_env *envt);
void		exit_successful_pipe(t_command *cmd);
void		exit_failed_pipe(t_command *cmd, int in, int out, t_env *envt);

// env.c - ENV builtin and environment variable helpers
char		*make_env_string(char *name, char *value);
void		ms_env_t(t_env *environ);
char		**serialise_env(t_env *env);
t_env		*init_new_env(char *str);
void		add_to_env_list(t_env *lsthead, t_env *to_add);

// env_vars.c - Query and manipulate ENV variables
int			is_in_envt(char *name, t_env *envt);
void		t_add_new_env_var(char *name, char *value, t_env *env);
void		t_replace_env_var(char *name, char *value, t_env **envt);
char		*get_value_of_env(char *name, t_env *envt);

// cd.c - change wd, set new value of PWD
void		ms_cd(t_command *cmd, t_env *envt);
void		update_pwd(char *oldpwd, char *newpwd, t_env *envt);

// prompt.c - functions to enhance the readline prompt
char		*get_shrt_wd(t_env *envt);
char		*get_status_for_prompt(void);
char		*get_prompt(t_env *envt);

// execute.c -functions to execute commands
void		run_command(t_command *cmd, t_env *envt);
char		*search_in_path(char *cmd, t_env *envt);
void		execute_builtin(t_command *cmd, t_env *env);

//sort.c
void		swap(t_env *a, t_env *b);
void		sort_env(t_env *head);
void		append(t_env **head_ref, char *new_vname, char *new_value);
void		print_list(t_env *node);
void		remove_node(t_env **first, t_env *target);

// debug.c TODO delete later
void		print_tokens(char **arr);
void		print_cmd_parts(t_command *cmd);
#endif
