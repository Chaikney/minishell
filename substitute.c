#include "minishell.h"

// TODO Add 42 header

void	step_over(char **str, char *skipthis)
{
	while (**str == *skipthis)
	{
		str++;
		skipthis++;
	}
}

// Return the index of the first $ if command needs to
// have variable subsitution.
// If no sub is needed, return -1
// TODO Take into account "" and '' quoting to decide a sub is needed
// If there is a $ between '' then we must ignore it.
// A "$" gets subd as normal though
// NOTE The '' and "" characters need to be stripped though.
// FIXME This cannot handle the ' correctly, it only skips it if present
int	needs_sub(char *str)
{
	int	i;
	char	*ptr;

	i = 0;
	ptr = ft_strchr(str, '\'');
	if (!ptr)	// NOTE Simple case, substitute $VARs
	{
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
	else	// NOTE we have to skip over a set of comillas
	{
		/* while (str[i] != '\''/\* ) *\/ */
		/* /\* 	i++; *\/ */
		/* /\* // Then find the second *\/ */
		/* /\* if (str[i] == '\'')	// NOTE Skipping past parts in comillas *\/ */
		/* /\* { *\/ */
		/* /\* 	while (str[i] == '\'') *\/ */
		/* /\* 		i++; *\/ */
		/* } */
	return (-1);
	}
}

// Subsitute the substr remove with replace in a string
// DONE If replace or remove are null, still have to get rid of the $
// (or we get an endless loop in substitute_variables)
// TODO Have to remove " and ' characters from the final cmd
// NOTE Does that get cut out of what we do here, or passed to the command to let that handle it¿???
// FIXME Adds a space for missing values, not needed? bash collapses it to a single space
char	*ms_strsub(char *str, char *old_sub, char *new_sub)
{
	int		len;
	char	*new_str;
	char	*cptr;

	if (!old_sub)
		return (str);
	len = ft_strlen(str) - ft_strlen(old_sub) + ft_strlen(new_sub) + 1;
	new_str = malloc(sizeof(char) * len);
	ft_bzero(new_str, len);
	if (!new_str)
		return (NULL);
	new_str[len - 1] = '\0';
	cptr = new_str;
	while (*str != '$')	// copy until the $
		*new_str++ = *str++;
	if (new_sub)
	{
		while (*new_sub != '\0')
			*new_str++ = *new_sub++;
	}
	while ((*str != ' ') && (*str != '\0'))	// Here we step over the variable name in the command
		str++;
	if (*str == ' ')
		str++;		// NOTE tries to avoid the extra space issue; probably dangerous though
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
// FIXED Causes an infinite loop if there is no variable found.
char	*substitute_variables(char *cmd)
{
	int		sub_pos;
	int		sub_len;
	char	*var_name;
	char	*val;
	char	*new_cmd;

	sub_pos = needs_sub(cmd);
	new_cmd = NULL;
	while (sub_pos != -1)
	{
		sub_pos++;	// NOTE Start the count *after* the $
		sub_len = 0;
		while ((cmd[sub_pos + sub_len] != '\0')
			&& (cmd[sub_pos + sub_len] != ' '))
			sub_len++;
		var_name = ft_substr(cmd, sub_pos, (sub_len));
//		printf("searching for %s in env", var_name);
		val = getenv(var_name);
//		printf("\tfound: %s", val);
		new_cmd = ms_strsub(cmd, var_name, val);	// NOTE and this then segfaults.
		free(var_name);
		sub_pos = needs_sub(new_cmd);
		cmd = new_cmd;
	}
	return (cmd);
}
