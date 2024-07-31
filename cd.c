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

// Examine the argument passed to our CD builtin
// and return the target to use.
// NOTE This is mainly so we can use ~ as an alias for $HOME
// ....but other things might be possible?
static char	*get_cd_target(t_command *cmd)
{
	char	*target;

	if (cmd->argc != 2)
	{
		perror("cd: One single argument required\n");
		if (cmd->argc > 2)
			g_procstatus = E2BIG;
		else
			g_procstatus = EINVAL;
		return (NULL);
	}
	else if (ft_strncmp(cmd->argv[1], "~", 2) == 0)
		target = getenv("HOME");
	else
		target = cmd->argv[1];
	return (target);
}

// BUILTIN cd command
// Triggered by user action. Most work done in ms_export_cd
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
// FIXME cd followed by a non-existent command leaks memory
// 		(e.g. cd .. then grp)
// DONE Adapt to t_env
// TODO Check memory use of new form.
void	ms_cd(t_command *cmd, char **envp, t_env *envt)
{
//	int		pwd_posn;
	char	*oldpwd;
	char	*new_pwd;
//	char	*wd;
	char	*target;


	oldpwd = NULL;
	oldpwd = getcwd(oldpwd, 0);
	target = get_cd_target(cmd);
	if (!target)
		return ;
	if (chdir(target) != 0)
		printf("wrong address\n");
	else
	{
		new_pwd = NULL;
		new_pwd = getcwd(new_pwd, 0);
		/* pwd_posn = find_env_var(envp, "PWD"); */
		/* oldpwd = ft_strjoin("OLD", envp[pwd_posn]); */
		/* new_pwd = ft_strjoin("PWD=", wd); */
		/* free(wd); */
		update_pwd(envp, oldpwd, new_pwd, envt);
		/* free (oldpwd); */
		/* free(new_pwd); */
	}
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
// TODO Probably will be obsolete later
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
// TODO Unify error handling in update_pwd
// NOTE Variables used:
// -  new_envp:	expanded temporary copy of envp (to be freed)
// -  OLDPWD	string to be written to OLDPWD (includes name=) (freeing TBC)
// -  new_pwd:	(string to be written to PWD (includes name=) (freeing TBC)
// - env_len:	Number of lines in the environment
// NOTE I don't know whether strdup or pointers are better for adding to env
// Either way seems impossible to free the memory in certain cases....
// ...how do we *know*? The original envp pieces are statically allocated.
// ...should ours be too??
void	update_pwd(char **envp, char *oldpwd, char *new_pwd, t_env *envt)
{

	(void) envp;	//  HACK for compilation, remove var
	if ((oldpwd == NULL) || (new_pwd == NULL))
		cd_error("Missing value for PWD update.", NULL, NULL, oldpwd);
	else
	{
		int_unset("OLDPWD", envp, envt);
		int_unset("PWD", envp, envt);
		t_add_new_env_var("PWD", new_pwd, envt);
		t_add_new_env_var("OLDPWD", oldpwd, envt);
	}
}
