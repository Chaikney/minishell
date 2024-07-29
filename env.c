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
//
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
