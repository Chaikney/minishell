#include "minitalk.h"


// Some shell builtin functions, may split later.

// Find the line in env that has PWD and return it.
// Works similar to get_path
// Returns a pointer to the location where the var begins. We would have to read
// onwards to a new line when pwd is called.
// ...would an allocated var be better?
// This will be used in the PWD builtin, and in the prompt.
// DONE Change this to allocate and return a variable.
// TODO This is probably obsolete since we can use getcwd. Remove it.
char	*get_wd(char **envp)
{
    char    *wd;
    char    *ptr;
    int     len;
    int     i;

    len = 0;
    i = 0;
	while (*envp != NULL)
	{
		if (ft_strncmp(*envp, "PWD=", 4) == 0)
			break ;
		envp++;
	}
    ptr = envp;
    while (*ptr++ != '\n')
        len++;
    wd = malloc(sizeof(char) * (len + 1));
    wd[len] = '\0';
    // FIXME I think this copy is wrong, pointer is at the end. Go backwards instead? Or use libft
    while (i < len)
    {
        wd[i] = *ptr;
        i++;
        ptr++;
    }
	return (wd);
}

// Minishell builtin, our version of pwd.
// Calls getcwd, displays wd then frees the variable.
// FIXME Just use getcwd!
int ms_pwd(void)
{
    char    *wd;

    wd = NULL
    wd = getcwd(wd, 0);     // Calling with NULL and 0 mallocs as much space as is needed.
    if (!wd)
        return (-1);
    printf("%s\n", wd);
    free (wd);
    return (0);
}