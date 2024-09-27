/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:38:32 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/27 17:59:54 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return a part of the working directory to show in prompt
// linesize limits how many characters we show
// DONE make work with t_env
// NOTE I think I do NOT need to free full_wd as it points to a string in tenv
char	*get_shrt_wd(t_env *envt)
{
	char	*cut_wd;
	char	*ptr;
	int		i;
	int		linesize;
	char	*full_wd;

	linesize = 15;
	i = 0;
	full_wd = get_value_of_env("PWD", envt);
	cut_wd = malloc (sizeof(char) * linesize + 1);
	if (!full_wd)
		return (NULL);
	ptr = ft_strrchr(full_wd, '/');
	if (!ptr)
		return (NULL);
	while (*ptr != '\0')
		cut_wd[i++] = *ptr++;
	while (i < linesize)
		cut_wd[i++] = '\0';
	return (cut_wd);
}

// Return a coloured string with g_processtus
// i.e. the last process result to be used in the prompt
char	*get_status_for_prompt(void)
{
	char	*status;
	char	*prompt;
	char	*first_part;

	status = ft_itoa(g_procstatus);
	first_part = ft_strjoin(" [\033[31m", status);
	prompt = ft_strjoin(first_part, "\033[0m] > ");
	free (first_part);
	free (status);
	return (prompt);
}

// This returns a text string to be displayed by readline
// when waiting for user input
// NOTE Once a variable has been used in strjoin it can (should) be freed.
// IDEA Check that the terminal is colour-capable before using control codes
// FIXME Occasional (post heredoc?) memleak in the prompt = strjoin call
// NOTE Also seen in builtins (echo)
// It might be because g_procstatus is not being reset to zero
// TODO Easiest fix is to get rid of the feature, but...
// ...the variable is also wrong this way..
char	*get_prompt(t_env *envt)
{
	char	*prompt;
	char	*status;
	char	*tmp_part;
	char	*cwd;
	char	*tmp2;

	tmp_part = ft_strjoin(getenv("USER"), " in ");
	cwd = get_shrt_wd(envt);
	tmp2 = ft_strjoin(tmp_part, cwd);
	free (tmp_part);
	free(cwd);
	if (g_procstatus != 0)
	{
		status = get_status_for_prompt();
		printf ("%s\n", status);
		free (status);
	}
	prompt = ft_strjoin(tmp2, " ");
	free (tmp2);
	return (prompt);
}
