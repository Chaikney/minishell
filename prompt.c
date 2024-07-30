/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:38:32 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:38:34 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return a part of the working directory to show in prompt
// linesize limits how many characters we show
// FIXED This does not get updated when we change directory?
// FIXME Too many lines in function.
// Yet ENV shows a new PWD...
char	*get_shrt_wd(char **envp)
{
	int		wd_line;
	char	*cut_wd;
	char	*ptr;
	int		i;
	int		linesize;

	linesize = 15;
	i = 0;
	cut_wd = malloc (sizeof(char) * linesize + 1);
	wd_line = find_env_var(envp, "PWD");
	if (wd_line == -1)
		return (NULL);
	ptr = ft_strrchr(envp[wd_line], '/');
	if (!ptr)
		return (NULL);
	while (*ptr != '\0')
	{
		cut_wd[i++] = *ptr++;
	}
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

// DONE Add (a part of) the wd to prompt
// DONE Shorten the wd.
// FIXED Too many variables.
// DONE Add username to prompt
// TODO Check that the terminal is colour-capable before using control codes
// This returns a text string to be dsiplayed by readline
// when waiting for user input
// NOTE Once a variable has been used in strjoin it can (should) be freed.
char	*get_prompt(char **envp)
{
	char	*prompt;
	char	*status;
	char	*tmp_part;
	char	*cwd;
	char	*tmp2;

	tmp_part = ft_strjoin(getenv("USER"), " in ");
	cwd = get_shrt_wd(envp);
	tmp2 = ft_strjoin(tmp_part, cwd);
	free (tmp_part);
	free(cwd);
	if (g_procstatus != 0)
	{
		status = get_status_for_prompt();
		prompt = ft_strjoin(tmp2, status);
		free (status);
	}
	else
		prompt = ft_strjoin(tmp2, " > ");
	free (tmp2);
	return (prompt);
}
