#include "minishell.h"

// TODO Add 42 header

// BUILTIN cd command
// Triggered by user action. Most work done in ms_export_cd
// TODO Error in CD should set g_procstatus?
// TODO This looks like it only changes the OLDPWD variables if chdir fails!
void	ms_cd(t_command *cmd, char **envp)
{
	if (cmd->argc < 2)
		fprintf(stderr, "cd: missing argument\n");
	else
	{
		if (chdir(cmd->argv[1]) != 0)
			printf("wrong address\n");
		else
			ms_export_cd(envp);
	}
	return ;
}

// free whatever vars need to be freed.
// Display appropriate error message
// Static because it is specific to the CD function
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
// - update $PWD and $OLDPWD
// - - how? Looks like copying the entire envp?
// FIXME Too many variables in ms_export_cd
// FIXED Variables defined and set in line
// FIXME Too many lines in ms_export_cd, needs refactor
// TODO Unify error handling in ms_export_cd
// DONE REmove cmd from call signature, un-needed
// TODO Variables to be freed:
// [  ] new_envp
// [  ] NOT wd! (TBC)
// [  ]  OLDPWD
// Tests and edge cases we have to handle.
// [x] cd [directory that exists]			move to that place
// [x] cd [directory that does not exist]	"no such file or directory"
// [x] cd ..								move up one level
// [x] cd ../..							move up two levels
// [x] cd ../other_folder					Move to sibling folder
void	ms_export_cd(char **envp)
{
	int		pwd_posn;
	char	*wd;
	char	**new_envp;	// NOTE This is malloc'd, has to be freed.
	size_t	env_len;	// Number of lines in envp
	char	*oldpwd;
	char	*new_pwd;

	pwd_posn = 0;
	wd = NULL;
	wd = getcwd(wd, 0);
	if (!wd)
		return;

	// Unset OLDPWD
	ms_unset_export("OLDPWD", envp);
	// Update PWD and OLDPWD
	env_len = 0;
	while (envp[env_len] != NULL)
		env_len++;
	// Create space for a larger copy of envp
	new_envp = malloc(sizeof(char *) * (env_len + 3));
	if (new_envp == NULL)
	{
		cd_error("malloc", wd, NULL, NULL);
		return ;
	}

	copy_envp(envp, new_envp);

	// Create strings for old and new PWD
	// TODO Handle PWD not found error (i.e. find_env_var returns -1)
	pwd_posn = find_env_var(envp,"PWD");
	oldpwd = ft_strjoin("OLD", new_envp[pwd_posn]);
	// Set PWD to new working directory
	new_pwd = ft_strjoin("PWD=", wd);
	if ((oldpwd == NULL) || (new_pwd == NULL))
	{
		cd_error("ft_strjoin", wd, new_envp, oldpwd);
		return;
	}
	// Adding new lines to the envp
	new_envp[env_len++] = oldpwd;
	new_envp[env_len++] = new_pwd;
	new_envp[env_len] = NULL;	// Terminate envp with NULL
	free(wd);

	copy_envp(new_envp, envp);	// HACK Experiment part 2 -- ALSO WORKS
	// NOTE This is needed to remove the extra (first) PWD entry in envp
	ms_unset_export("PWD",envp);
	free(new_envp);
}
