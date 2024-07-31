/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:39:10 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:39:13 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Builtin ENV command.
// FIXED env should not display empty variables, unlike export.
// DONE Ensure bash-compatibility
// NOTE running env > bashenv and env > msenv is an OK test
// ...but order and exact variables are different...
void	ms_env(char **envp)
{
	while (*envp)
	{
		if (ft_strchr(*envp, '='))
			printf("%s\n", *envp);
		envp++;
	}
	return ;
}

void	ms_env_t(t_env *environ)
{
	t_env	*ptr;

	ptr = environ;
	while (ptr->next != NULL)
	{
		printf("%s=%s\n", ptr->vname, ptr->value);
		ptr = ptr->next;
	}
	printf("%s=%s\n", ptr->vname, ptr->value);
}

t_env	*init_new_env(char *str)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));	// FIXED? Memory allocated here is not freed
	new_env->next = NULL;
	new_env->vname = get_export_name(str);
	new_env->value = get_export_value(str);
	return (new_env);
}

// read the Starting environment into a variable to access.
// for line in envp
// - find name (all up to =)
// - find value
// - set marker to next t_env
t_env	*parse_env(char **envp)
{
	t_env	*ptr;
	t_env	*first;
	t_env	*next;

	if (*envp)
	{
		first = init_new_env(*envp);
		ptr = first;
		envp++;
		while (*envp)
		{
			next = init_new_env(*envp);
			ptr->next = next;
			ptr = ptr->next;
			envp++;
		}
		return (first);
	}
	return (NULL);
}

// Display a line part for ms_export_display when the variable is
// not empty.
// NAME="VALUE"
static void	ms_ed_with_var(char *c, char *split_point)
{
	while (c != split_point)
	{
		printf("%c", *c);
		c++;
	}
	printf("%c", *c);
	c++;
	printf("\"");
	while (*c != '\0')
	{
		printf("%c", *c);
		c++;
	}
	printf("\"\n");
}

// Handle the display of ENV variables when EXPORT is called
// with 0 arguments.
// display "declare -x "
// display NAME
// if value, display "=VALUE"
// if no value, only newline
// DONE Handle empty values (if no equals, stop there.)
// FIXED Too many lines in ms_export_display
void	ms_export_display(char **envp)
{
	char	*line_split;
	char	*c;

	while (*envp)
	{
		c = *envp;
		line_split = ft_strchr(*envp, '=');
		printf("declare -x ");
		if (line_split)
		{
			ms_ed_with_var(c, line_split);
		}
		else
		{
			while (*c != '\0')
			{
				printf("%c", *c);
				c++;
			}
			printf("\n");
		}
		envp++;
	}
}

// Takes a  name and a value; returns a string formatted suitable
// for storing as an environment variable.
// Variables:
// len:	length needed for string. 1 for null and 1 for the =
// name:	of the environment variable
// value:	of the environment variable
char	*make_env_string(char *name, char *value)
{
	char	*env_str;
	char	*tmp;

	if (!name)
		return (NULL);
	if (!value)
		return (ft_strdup(name));
	else
	{
		tmp = ft_strjoin(name, "=");
	}
	env_str = ft_strjoin(tmp, value);
	free (tmp);
	return (env_str);
}

// Returns the line index number (in eviornment)
// for the requested var name.
// If variable not found, returns -1
// FIXED This can SEGFAULT if envp is not there.
// FIXME This does not locate our added variables.
//
int	find_env_var(char **envp, const char *var)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	len = ft_strlen(var);
	if (!envp)
		return (-1);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		else if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] != '=')
			return (i);
		i++;
	}
	return (-1);
}
