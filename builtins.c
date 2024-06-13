#include "minishell.h"

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
