/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:38:21 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/26 18:22:47 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this is to disable CRTL-BACKSLASH
// "Ctrl-\ does nothing"

// catch CTRL-c / SIGINT
// FIXED This should return to the normal prompt / readline call
// ...I think this leads to duplicated prompt message.
// "CTRL-C displays a new prompt on a new line."
// Works like: https://stackoverflow.com/a/41917863
// - Move to a new line
// - regenerate prompt
// - clear previous text
void	handle_sigint(int sig)
{
	if (sig)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	handle_sigint_child(int sig)
{
	if (sig)
	{
		return ;
	}
}

// Setup the functions to handle each signal we have to manage.
// DONE Implement signals handling SIGINT - needs fixed
// TODO Implement signals handling CTRL-D EOF
// DONE Implement signals handling CTRL-\ uncontrolled quit
int	setup_signals(int is_child)
{
	if(is_child)
	{
		signal(SIGINT, handle_sigint_child);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	return (0);
}
