#include "minishell.h"

// TODO Add 42 header

// Return the index if the command needs to
// have variable subsitution, i.e. has $
// If no sub is needed, return -1
// TODO Take into account "" and '' quoting to decide a sub is needed
// FIXED str is uninitialised and causes valgrind complaints
// ...call to ft_bzero in ms_strsub fixed that
int	needs_sub(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')	// FIXED Uninitialised var here
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	if (str[i] == '$')
		return (i);
	else
		return (-1);
}

// Subsitute the substr remove with replace in a string
// FIXED Invalid write here, causes segfault
// FIXED What is returned has not changed!
char	*ms_strsub(char *str, char *remove, char *replace)
{
	int		len;
	char	*new_str;
	char	*cptr;

	printf("\nWill replace: %s\twith: %s\t in: %s", remove, replace, str);
	len = ft_strlen(str) -
		ft_strlen(remove)
		+ ft_strlen(replace) + 1;	// FIXME problem with this part in "more than one substitution" case
	new_str = malloc(sizeof(char) * len);	// FIXED Complaints of unitialised variable come from here
	ft_bzero(new_str, len);
	if (!new_str)
		return (NULL);
	new_str[len - 1] = '\0';
	cptr = new_str;
	while (*str != '$')
	{
		*new_str++ = *str++;
	}
	while (*replace != '\0')
		*new_str++ = *replace++;
	while ((*str != ' ') && (*str != '\0'))
		str++;
	while (*str != '\0')
		*new_str++ = *str++;
//	printf("\nresult: %s", cptr);	// HACK For debugging
	return (cptr);
}

// Substitute a variable into its position in the command:
// - find position of a variable
// - read the name of that variable
// - fetch the value of the variable
// - put the value into the command
// - run again / recurse until we have no more things to sub
// NOTE cmd is assumed to be the unsplit input from readline
// FIXED If no substitution, we return garbage - problem is the new_cmd thing
// TODO Test with > 1 substitution in a command.
// FIXED reduce number of variables in this function, fails Norm.
char	*substitute_variables(char *cmd)
{
	int		sub_pos;
	int		sub_len;
	char	*var_name;
	char	*val;
	char	*new_cmd;

	printf("\nStart with: %s", cmd);
	sub_pos = needs_sub(cmd);
	new_cmd = NULL;
	printf("entering loop with sub_pos %i", sub_pos);
	while (sub_pos != -1)
	{
		sub_len = 0;
		while ((cmd[sub_pos + sub_len] != '\0')
			&& (cmd[sub_pos + sub_len] != ' '))
			sub_len++;
		var_name = ft_substr(cmd, sub_pos + 1, (sub_len));
		printf("searching for %s in env", var_name);
		val = getenv(var_name);
		printf("\tfound: %s", val);
		new_cmd = ms_strsub(cmd, var_name, val);
		free(var_name);
		sub_pos = needs_sub(new_cmd);	// FIXED Uninitd warning as well - new_cmd??
		cmd = new_cmd;
	}
//	printf("\tBecame: %s", new_cmd);	// HACK For debugging, causes invalid read warnings
	return (cmd);
}
