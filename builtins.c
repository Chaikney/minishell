#include "minishell.h"
#include <stdio.h>

//TODO Add 42 header

// Some shell builtin functions, may split later.

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// NOTE Calling getcwd with NULL and 0 mallocs as much space as is needed
int ms_pwd(void)
{
	char    *wd;
	wd = NULL;
	wd = getcwd(wd, 0);
	if (!wd)
		return (-1);
	printf("%s\n", wd);
	free (wd);
	return (0);
}

// Shell's exit clears the relevant variables and ends the program.
// - free cmd
// - anything else?
// TODO If this is called from main, we have a char* to free
void	ms_exit(t_command *cmd)
{
	printf("Agurrrr....\n");
	clear_t_command(cmd);
	exit(EXIT_SUCCESS);
}

// Set a new variable in env.
// If the variable name already exists, change it. (remove existing and then add?)
// ....this is basically the same functionality as UNSET ?!
// If the variable does not already exist, add it.
// NOTE I think we have to distinguish between the variable's NAME and VALUE
// FIXME This has variables defined in-line at time of use.
void ms_unset_export(char *unset_var, char **envp) {
    int var_index = 0; 
    var_index = find_env_var(envp, unset_var);

    if (var_index >= 0) 
    {
        // Liberar la memoria de la variable de entorno

        // Eliminar la variable de entorno del arreglo envp
        int i = var_index;
        while (envp[i] != NULL) 
        {
            envp[i] = envp[i + 1];
            i++;
        }
    } 
    else 
    {
        printf("unset: variable %s not found\n", unset_var);
    }
}

// Set a new variable in env.
// If the variable name already exists, change it. (remove existing and then add?)
// ....this is basically the same functionality as UNSET ?!
// If the variable does not already exist, add it.
// NOTE I think we have to distinguish between the variable's NAME and VALUE
// FIXME This has variables defined in-line at time of use.
void ms_export(t_command *cmd, char **envp) 
{
    char *var;
    char *new_var;
    int len_unset;
    char *unset_var;
    len_unset = 0;
    int j = 0;
    if (cmd->argc < 2) 
    {
        printf("export: missing argument\n");
        return;
    }
    var = cmd->argv[1];
    while (cmd->argv[1][len_unset] != '=')
            len_unset++;
    unset_var = malloc(sizeof(char) * len_unset + 1);
    while (j <= len_unset -1 )
    {
        unset_var[j] = var[j];
        j++;
    }
    
        printf("\n\n\n-----%s------\n\n\n",unset_var);
        ms_unset_export(unset_var,envp);
        size_t env_len = 0;
        while (envp[env_len] != NULL) 
        {
            env_len++;
        }

        char **new_envp = malloc(sizeof(char *) * (env_len + 2));
        if (new_envp == NULL) 
        {
            perror("malloc");
            return;
        }

        size_t i = 0;
        while (i < env_len) 
        {
            new_envp[i] = envp[i];
            i++;
        }

        new_var = ft_strdup(var);
        if (new_var == NULL) 
        {
            perror("ft_strdup");
            free(new_envp);
            return;
        }
        new_envp[env_len] = new_var;
        new_envp[env_len + 1] = NULL;
        i = 0;
        while (i < env_len + 1) 
        {
            envp[i] = new_envp[i];
            i++;
        }
        envp[env_len + 1] = NULL;

        free(new_envp);
}


// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// FIXME This causes an endless loop, I guess in the while(envp[i])
// FIXME Also cause a segfault!
void ms_unset(t_command *cmd, char **envp) {
    if (cmd->argc < 2) 
    {
        printf("unset: missing argument\n");
        return;
    }
    char *var = cmd->argv[1];
    int var_index = find_env_var(envp, var);

    if (var_index >= 0) 
    {
        // Liberar la memoria de la variable de entorno
        free(envp[var_index]);

        // Eliminar la variable de entorno del arreglo envp
        int i = var_index;
        while (envp[i] != NULL) 
        {
            envp[i] = envp[i + 1];
            i++;
        }
    } 
    else 
    {
        printf("unset: variable %s not found\n", var);
    }
}

void ms_export_cd(t_command *cmd, char **envp) {
    char *var_pwd = "PWD=";
    int h;
    char *wd = NULL;
    (void)cmd;
    h = 0;
    wd = getcwd(wd, 0);
    if (!wd)
        return;

    // Unset OLD_PWD
    ms_unset_export("OLDPWD", envp);
    // Update PWD and OLDPWD
    size_t env_len = 0;
    while (envp[env_len] != NULL) {
        env_len++;
    }

    char **new_envp = malloc(sizeof(char *) * (env_len + 3));
    if (new_envp == NULL) {
        perror("malloc");
        free(wd);
        return;
    }

    size_t i = 0;
    for (i = 0; i < env_len; i++) {
        new_envp[i] = envp[i];
    }

    // Set OLDPWD to current PWD
    h = find_env_var(envp,"PWD");
    char *oldpwd = ft_strjoin("OLD", new_envp[h]);
    printf("%s", oldpwd);
    if (oldpwd == NULL) {
        perror("ft_strjoin");
        free(new_envp);
        free(wd);
        return;
    }
    new_envp[i++] = oldpwd;

    // Set PWD to new working directory
    char *new_pwd = ft_strjoin(var_pwd, wd);
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
    for (size_t j = 0; j < i; j++) {
        envp[j] = new_envp[j];
    }
    envp[i] = NULL;
    ms_unset_export("PWD",envp);
    free(new_envp);
}
