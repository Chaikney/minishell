#include "minishell.h"
#include <stdio.h>

//TODO Add 42 header

// Some shell builtin functions, may split later.

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// FIXED Just use getcwd!
int ms_pwd(void)
{
	char    *wd;

	wd = NULL;
	wd = getcwd(wd, 0);     // Calling with NULL and 0 mallocs as much space as is needed.
	if (!wd)
		return (-1);
	printf("%s\n", wd);
	free (wd);
	return (0);
}

// Returns the line index number of PATH for the requested var string
// If not found, returns -1
int find_env_var(char **envp, const char *var)
{
    size_t len ;
	int i;

	i = 0;
    len = ft_strlen(var);
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
        {
            return (i);
        }
		i++;
    }
    return (-1);
}

// Set a new variable in env.
// If the variable name already exists, change it. (remove existing and then add?)
// ....this is basically the same functionality as UNSET ?!
// If the variable does not already exist, add it.
// NOTE I think we have to distinguish between the variable's NAME and VALUE
// FIXME This has variables defined in-line at time of use.
void ms_export(t_command *cmd, char **envp) {
    char *var;
    char *new_var;
    int var_index;

    if (cmd->argc < 2) 
    {
        fprintf(stderr, "export: missing argument\n");
        return;
    }
    var = cmd->argv[1];
    var_index = find_env_var(envp, var);

    if(var_index != -1)
        ms_unset(cmd,envp);
    else
    {
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
}


// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// FIXME This causes an endless loop, I guess in the while(envp[i])
// FIXME Also cause a segfault!
void ms_unset(t_command *cmd, char **envp) {
    if (cmd->argc < 2) 
    {
        fprintf(stderr, "unset: missing argument\n");
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
        fprintf(stderr, "unset: variable %s not found\n", var);
    }
}
