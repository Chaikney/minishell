#include "minishell.h"

// TODO Add 42 header

// Return the index of the first $ if command needs to
// have variable subsitution.
// If no sub is needed, return -1
// TODO Take into account "" and '' quoting to decide a sub is needed
int	needs_sub(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
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
// TODO If replace or remove are null, still have to get rid of the $
// (or we get an endless loop in substitute_variables)
char	*ms_strsub(char *str, char *remove, char *replace)
{
	int		len;
	char	*new_str;
	char	*cptr;

	if ((!replace) || (!remove))
		return (str);
	len = ft_strlen(str) - ft_strlen(remove) + ft_strlen(replace) + 1;
	new_str = malloc(sizeof(char) * len);
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
// DONE Test with > 1 substitution in a command.
// FIXME Function has too many lines
char	*substitute_variables(char *cmd)
{
	int		sub_pos;
	int		sub_len;
	char	*var_name;
	char	*val;
	char	*new_cmd;

	sub_pos = needs_sub(cmd);
	new_cmd = NULL;
	while (sub_pos != -1)	// NOTE Endless loop if replace or remove are null (Should remove the $ at least)
	{
		sub_pos++;	// NOTE Start the count *after* the $
		sub_len = 0;
		while ((cmd[sub_pos + sub_len] != '\0')
			&& (cmd[sub_pos + sub_len] != ' '))
			sub_len++;
		var_name = ft_substr(cmd, sub_pos, (sub_len));
		printf("searching for %s in env", var_name);
		val = getenv(var_name);	// NOTE This comes back null (when 2 requests)
		printf("\tfound: %s", val);
		new_cmd = ms_strsub(cmd, var_name, val);	// NOTE and this then segfaults.
		free(var_name);
		sub_pos = needs_sub(new_cmd);
		cmd = new_cmd;
	}
	return (cmd);
}
