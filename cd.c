#include "minishell.h"

// TODO Add 42 header

// BUILTIN cd command
// Triggered by user action. Most work done in ms_export_cd
// TODO Error in CD should set g_procstatus?
void	ms_cd(t_command *cmd, char **envp)
{
	if (cmd->argc < 2)
		fprintf(stderr, "cd: missing argument\n");
	else
	{
		if (chdir(cmd->argv[1]) != 0)
			printf("wrong address\n");
		else
			ms_export_cd(cmd,envp);
	}
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
// Tests and edge cases we have to handle.
// [x] cd [directory that exists]			move to that place
// [x] cd [directory that does not exist]	"no such file or directory"
// [x] cd ..								move up one level
// [x] cd ../..							move up two levels
// [x] cd ../other_folder					Move to sibling folder
void	ms_export_cd(t_command *cmd, char **envp)
{
	char	*var_pwd = "PWD=";
	int		h;
	char	*wd = NULL;
	char	**new_envp;	// NOTE This is malloc'd, has to be freed.
	size_t	j;
	size_t	env_len;
	size_t	i;
	char	*oldpwd;
	char	*new_pwd;

	(void)cmd;
	h = 0;
	j = 0;
	wd = getcwd(wd, 0);
	if (!wd)
		return;

	// Unset OLD_PWD
	ms_unset_export("OLDPWD", envp);
	// Update PWD and OLDPWD
	env_len = 0;
	while (envp[env_len] != NULL) {
		env_len++;
	}

	new_envp = malloc(sizeof(char *) * (env_len + 3));
	if (new_envp == NULL) {
		perror("malloc");
		free(wd);
		return;
	}

	i = 0;
	while (i < env_len)
	{
		new_envp[i] = envp[i];
		i++;
	}

	// Set OLDPWD to current PWD
	h = find_env_var(envp,"PWD");
	oldpwd = ft_strjoin("OLD", new_envp[h]);
	if (oldpwd == NULL) {
		perror("ft_strjoin");
		free(new_envp);
		free(wd);
		return;
	}
	new_envp[i++] = oldpwd;

	// Set PWD to new working directory
	new_pwd = ft_strjoin(var_pwd, wd);
	if (new_pwd == NULL) {
		perror("ft_strjoin");
		free(new_envp);
		free(wd);
		free(oldpwd);
		return;
	}
	new_envp[i++] = new_pwd;

	new_envp[i] = NULL;
	free(wd);

	// Update original envp
	while (j < i)
	{
		envp[j] = new_envp[j];
		j++;
	}
	envp[i] = NULL;
	ms_unset_export("PWD",envp);
	free(new_envp);
}
