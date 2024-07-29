/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:38:21 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/29 12:38:24 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this is to disable CRTL-BACKSLASH
// "Ctrl-\ does nothing"
void	handle_sigquit(int sig)
{
	(void) sig;
	return ;
}

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

// Setup the functions to handle each signal we have to manage.
// DONE Implement signals handling SIGINT - needs fixed
// TODO Implement signals handling CTRL-D EOF
// DONE Implement signals handling CTRL-\ uncontrolled quit
int	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	return (0);
}
