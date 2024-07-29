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
// FIXME This does not get updated when we change directory?
// FIXME Too many lines in function.
// Yet ENV shows a new PWD...
char	*get_shrt_wd(void)
{
	char	*wd;
	char	*cut_wd;
	int		len;
	int		i;
	int		linesize;

	linesize = 15;
	cut_wd = malloc (sizeof(char) * linesize + 1);
	wd = getenv("PWD");
	len = ft_strlen(wd);
	if (len < linesize)
		cut_wd = wd;
	else
	{
		i = linesize;
		cut_wd[linesize] = '\0';
		while (i != 0)
		{
			cut_wd[i] = wd[len];
			i--;
			len--;
		}
		cut_wd[0] = '.';
		cut_wd[1] = '.';
	}
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
// TODO Shorten the wd.
// FIXME Too many variables.
// FIXME Too many lines
// DONE Add username to prompt
// TODO Check that the terminal is colour-capable before using control codes
// This returns a text string to be dsiplayed by readline
// when waiting for user input
// NOTE Once a variable has been used in strjoin it can (should) be freed.
char	*get_prompt(void)
{
	char	*prompt;
	char	*status;
	char	*tmp_part;
	char	*uname;
	char	*cwd;
	char	*tmp2;

	uname = getenv("USER");
	if (uname)
		tmp_part = ft_strjoin(uname, " in ");
	cwd = get_shrt_wd();
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
	{
		prompt = ft_strjoin(tmp2, " > ");
	}
	free (tmp2);
	return (prompt);
}
