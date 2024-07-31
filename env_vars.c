#include "minishell.h"

// Query and manipulate ENV variables
//
//TODO add 42 header
//
// return 1 if the variable name is present in the env
// TODO There are places where this should be called to check (e.g unset)
int	is_in_envt(char *name, t_env *envt)
{
	t_env	*ptr;
	int	len;

	len = ft_strlen(name);
	ptr =  envt;
	while (ptr->next != NULL)
	{
		if (ft_strncmp(name, ptr->vname, len) == 0)
			return (1);
		ptr = ptr->next;
	}
	if (ft_strncmp(name, ptr->vname, len) == 0)
		return (1);
	return (0);
}

// Take a name change the value in the envp to the one given.
// TODO If value is NULL, what should replace_env_var do?
void	t_replace_env_var(char *name, char *value, t_env **envt)
{
	t_env	*ptr;
	int		len;

	ptr = *envt;
	len = ft_strlen(name);
	while (ptr->next != NULL)
	{
		if (ft_strncmp(name, ptr->vname, len) == 0)
		{
			printf("\nupdating %s with %s", name, value);	// HACK for debugging
			ptr->value = ft_strdup(value);
			break ;
		}
		ptr = ptr->next;
	}
	if (ft_strncmp(name, ptr->vname, len) == 0)
	{
		printf("\nupdating %s with %s", name, value);	// HACK for debugging
		ptr->value = ft_strdup(value);
	}
}

// Take a name and value and add this  to the envp.
// the Named value should not already exist.
// New version using t_env
void	t_add_new_env_var(char *name, char *value, t_env *env)
{
	char	*to_write;
	t_env	*new_entry;

	to_write = make_env_string(name, value);
	new_entry = init_new_env(to_write);
	add_to_env_list(env, new_entry);
}
