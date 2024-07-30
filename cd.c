/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:39:41 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:39:44 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Examine the argument passed to our CD builtin and return the
// target to use.
// NOTE This is mainly so we can use ~ as an alias for $HOME
// ....but other things might be possible?
static char	*get_cd_target(t_command *cmd)
{
	char 	*target;

	if (ft_strncmp(cmd->argv[1], "~", 2) == 0)
		target = getenv("HOME");
	else
		target = cmd->argv[1];
	return (target);
}

// BUILTIN cd command
// Triggered by user action. Most work done in ms_export_cd
// TODO Error in CD should set g_procstatus?
// NOTE Check both oldpwd and newpwd for memleaks!
/// Tests and edge cases we have to handle.
// [x] cd [directory that exists]			move to that place
// [x] cd [directory that does not exist]	"no such file or directory"
// [x] cd ..								move up one level
// [x] cd ../..							move up two levels
// [x] cd ../other_folder					Move to sibling folder
// [x] cd ~									go to $HOME
// FIXED? oldpwd needs freed (but not immediately!)
// FIXED? new_pwd needs freed (but not immediately!)
// FIXME cd followed by a non-existent command leaks memory (e.g. cd .. then grp)
// FIXME ms_cd has too many lines!
void	ms_cd(t_command *cmd, char **envp)
{
	int		pwd_posn;
	char	*oldpwd;
	char	*new_pwd;
	char	*wd;
	char	*target;

	if (cmd->argc != 2)
		fprintf(stderr, "cd: One single argument required\n");
	else
	{
		target = get_cd_target(cmd);
		if (chdir(target) != 0)
			printf("wrong address\n");
		else
		{
			wd = NULL;
			wd = getcwd(wd, 0);
			pwd_posn = find_env_var(envp, "PWD");
			oldpwd = ft_strjoin("OLD", envp[pwd_posn]);
			new_pwd = ft_strjoin("PWD=", wd);
			free(wd);
			update_pwd(envp, oldpwd, new_pwd);
			free (oldpwd);
			free(new_pwd);
		}
	}
	return ;
}

// free whatever vars need to be freed.
// Display appropriate error message
// Static because it is specific to the CD function
// TODO Add newpwd to cd_error
// TODO Remove envp from newpwd to cd_error ?
static void	cd_error(char *errmsg, char *wd, char **new_envp, char *oldpwd)
{
	perror(errmsg);
	if (wd)
		free (wd);
	if (new_envp)
		free(new_envp);
	if (oldpwd)
		free(oldpwd);
}

// Copy the envp from src_envp to dst_envp
// Ensures null-termination
// but does *not* check that dst_envp is big enough!
void	copy_envp(char **src_envp, char **dst_envp)
{
	int	i;

	i = 0;
	while (src_envp[i] != NULL)
	{
		dst_envp[i] = src_envp[i];
		i++;
	}
	dst_envp[i] = NULL;
	return ;
}

// After cd is issued, this updates the PWD and OLDPWD variables.
// (cd happens in executeBuiltin when chdir is called)
// - unset existing $OLDPWD
// - make space for copy of envp and 2 more variables
// - copy envp to new_envp
// - add $PWD and $OLDPWD to the end of new_envp
// - copy new_envp (back) to envp
// - unset PWD to remove the first (old) PWD variable in envp
// free new_envp
// TODO Unify error handling in ms_export_cd
// NOTE Variables used:
// -  new_envp:	expanded temporary copy of envp (to be freed)
// -  OLDPWD	string to be written to OLDPWD (includes name=) (freeing TBC)
// -  new_pwd:	(string to be written to PWD (includes name=) (freeing TBC)
// - env_len:	Number of lines in the environment
void	update_pwd(char **envp, char *oldpwd, char *new_pwd)
{
	char	**new_envp;
	size_t	env_len;

	if ((oldpwd == NULL) || (new_pwd == NULL))
		cd_error("Missing value for PWD update.", NULL, NULL, oldpwd);
	else
	{
		int_unset("OLDPWD", envp);
		env_len = 0;
		while (envp[env_len] != NULL)
			env_len++;
		new_envp = malloc(sizeof(char *) * (env_len + 3));
		if (new_envp == NULL)
		{
			cd_error("malloc", NULL, NULL, NULL);
			return ;
		}
		copy_envp(envp, new_envp);
		new_envp[env_len++] = ft_strdup(oldpwd);
		new_envp[env_len++] = ft_strdup(new_pwd);
		new_envp[env_len] = NULL;
		copy_envp(new_envp, envp);
		int_unset("PWD", envp);
		free(new_envp);
	}
}
