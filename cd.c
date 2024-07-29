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
void	ms_cd(t_command *cmd, char **envp)
{
	int		pwd_posn;
	char	*oldpwd;
	char	*new_pwd;
	char	*wd;

	if (cmd->argc < 2)
		fprintf(stderr, "cd: missing argument\n");
	else
	{
		if (chdir(cmd->argv[1]) != 0)
			printf("wrong address\n");
		else
		{
			wd = NULL;
			wd = getcwd(wd, 0);
			pwd_posn = find_env_var(envp, "PWD");
			oldpwd = ft_strjoin("OLD", envp[pwd_posn]);
			new_pwd = ft_strjoin("PWD=", wd);
			free(wd);
			ms_export_cd(envp, oldpwd, new_pwd);
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
// TODO ms_export_cd is a misleading name
// NOTE Variables used:
// -  new_envp:	expanded temporary copy of envp (to be freed)
// -  OLDPWD	string to be written to OLDPWD (includes name=) (freeing TBC)
// -  new_pwd:	(string to be written to PWD (includes name=) (freeing TBC)
// - env_len:	Number of lines in the environment
void	ms_export_cd(char **envp, char *oldpwd, char *new_pwd)
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
		new_envp[env_len++] = oldpwd;
		new_envp[env_len++] = new_pwd;
		new_envp[env_len] = NULL;
		copy_envp(new_envp, envp);
		int_unset("PWD", envp);
		free(new_envp);
	}
}
