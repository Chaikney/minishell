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

// Handle the display of ENV variables when EXPORT is called
// with 0 arguments.
//
// display "declare -x "
// display NAME
// if value, display "=VALUE"
// if no value, only newline
// DONE Handle empty values (if no equals, stop there.)
// FIXME Too many lines in ms_export_display
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
			while (c != line_split)
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
