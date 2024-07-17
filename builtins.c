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
void ms_unset_export(char *unset_var, char **envp) 
{
    int var_index = 0; 
    var_index = find_env_var(envp, unset_var);

    if (var_index >= 0) 
    {
        // Liberar la memoria de la variable de entorno

        // Eliminar la variable de entorno del arreglo envp

        while (envp[var_index] != NULL) 
        {
            envp[var_index] = envp[var_index + 1];
            var_index++;
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
    int len1 = 0;
    char *var2;
    int len = 0;
    int j = 0;
    int h = 1;
    if (cmd->argc < 2) 
    {
        printf("export: missing argument\n");
        return;
    }
    while(cmd->argv[h] != NULL)
    {
        if(cmd->argv[h][0] == '=')
        {
            var2 = cmd->argv[h];
            len1= ft_strlen(var2);
            var2[len1]= '\0';
            printf("export : %s not a valid identifier",var2);
            h++;
        }
        var = cmd->argv[h];
        while (cmd->argv[1][len_unset] != '=' && cmd->argv[1][len_unset] != '\0')
            len_unset++;
        unset_var = malloc(sizeof(char) * len_unset + 1);
        while (j <= len_unset - 1)
        {
            unset_var[j] = var[j];
            j++;
        }
        unset_var[j] = '\0';
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
            len = ft_strlen(new_var);
            if(new_var[len - 1] == '=')
                new_var = ft_strjoin(new_var,"\"\"");
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
            h++;
    }
    
        
}


// Remove the variable in cmd from ENV, if present.
// If not present, take no action.
// FIXME This causes an endless loop, I guess in the while(envp[i])
// FIXME Also cause a segfault!
void ms_unset(t_command *cmd, char **envp) 
{
    int i;

    i = 0;
    if (cmd->argc < 2) 
    {
        printf("unset: missing argument\n");
        return;
    }
    i = ft_strlen(cmd->argv[1]);
    char *var = malloc(sizeof (char *) * i);
    var = cmd->argv[1];
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
    size_t j = 0;
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
    while (i < env_len) 
    {
        new_envp[i] = envp[i];
        i++;
    }

    // Set OLDPWD to current PWD
    h = find_env_var(envp,"PWD");
    char *oldpwd = ft_strjoin("OLD", new_envp[h]);
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
    while (j < i) 
    {
        envp[j] = new_envp[j];
        j++;
    }
    envp[i] = NULL;
    ms_unset_export("PWD",envp);
    free(new_envp);
}
