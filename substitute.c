#include "minishell.h"

// Return the index if the command needs to
// have variable subsitution, i.e. has $
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
char	*ms_strsub(char *str, char *remove, char *replace)
{
    int	len;
    char	*new_str;

    len = ft_strlen(str) - ft_strlen(remove) + ft_strlen(replace) + 1;
    new_str = malloc(sizeof(char) * len);
    if (!new_str)
        return (NULL);
    new_str[len] = '\0';
    while (*str != '$')
    {
        *new_str++ = *str++;
    }
    // now we have the start of the variable we instead copy replace
    // TODO But how do we find the end of the area to replace??
    while (*replace != '\0')
        *new_str++ = *replace++;
    while ((*str != ' ') || (*str != '\0'))
        str++;
    while (*str != '\0')
        *new_str++ = *str++;
    return (new_str);
}

// Substitute a variable into its position in the command:
// - find position of a variable
// - read the name of that variable
// - fetch the value of the variable
// - put the value into the command
// - run again / recurse until we have no more things to sub
// NOTE cmd is assumed to be the unsplit input from readline
char *substitute_variables(char *cmd)
{
    int	sub_pos;
    int	sub_len;
    char	*var_name;
    char	*val;
    char	*old_cmd;

    sub_pos = needs_sub(cmd);
    while (sub_pos != -1)
    {
        sub_len = 0;
        while ((cmd[sub_pos + sub_len] != '\0') && (cmd[sub_pos + sub_len] != ' '))
            sub_len++;
        var_name = ft_substr(cmd, sub_pos + 1, (sub_len));
        val = getenv(var_name);
        // Subsitute strings: sub_pos, cmd, val
        old_cmd = cmd;
        cmd = ms_strsub(cmd, var_name, val);
        free (old_cmd);
        free(var_name);
        free(val);
        sub_pos = needs_sub(cmd);
    }
    return (cmd);
}
