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

// free whatever vars need to be freed.
// Display appropriate error message
// Static because it is specific to the CD function
// Variables are freed outside of this function.
static void	cd_error(char *errmsg, int err)
{
	perror(errmsg);
	g_procstatus = err;
}

// Examine the argument passed to our CD builtin
// and return the target to use.
// NOTE This is mainly so we can use ~ as an alias for $HOME
// ....but other things might be possible?
// NOTE The return does not need to  be freed because it is either:
// - assigned in getenv
// - freed as part of clear_t_command.
static char	*get_cd_target(t_command *cmd)
{
	char	*target;

	if (cmd->argc != 2)
	{
		if (cmd->argc > 2)
			cd_error("cd: One single argument required", E2BIG);
		else
			cd_error("cd: One single argument required", EINVAL);
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
void	ms_cd(t_command *cmd, t_env *envt)
{
	char	*oldpwd;
	char	*new_pwd;
	char	*target;

	oldpwd = NULL;
	oldpwd = getcwd(oldpwd, 0);
	target = get_cd_target(cmd);
	if (!target)
		return ;
	if (chdir(target) != 0)
		cd_error("Target not found", ENOENT);
	else
	{
		new_pwd = NULL;
		new_pwd = getcwd(new_pwd, 0);
		update_pwd(oldpwd, new_pwd, envt);
		free (new_pwd);
		g_procstatus = 0;
	}
	free (oldpwd);
}

// After cd is issued, this updates the PWD and OLDPWD variables.
// They are first unset, then added back.
// NOTE Variables used:
// - OLDPWD:	string to be written to OLDPWD (freed on return to ms_cd)
// - new_pwd:	(string to be written to PWD (freed on return to ms_cd)
void	update_pwd(char *oldpwd, char *new_pwd, t_env *envt)
{
	if ((oldpwd == NULL) || (new_pwd == NULL))
		cd_error("Missing value for PWD update.", EINVAL);
	else
	{
		int_unset("OLDPWD", envt);
		int_unset("PWD", envt);
		t_add_new_env_var("PWD", new_pwd, envt);
		t_add_new_env_var("OLDPWD", oldpwd, envt);
	}
}
