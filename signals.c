/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emedina- <emedina-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:38:21 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/25 19:14:19 by emedina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// catch CTRL-c / SIGINT
// FIXED This should return to the normal prompt / readline call
// ...I think this leads to duplicated prompt message.
// "CTRL-C displays a new prompt on a new line."
// Works like: https://stackoverflow.com/a/41917863
// - Move to a new line
// - regenerate prompt
// - clear previous text
// FIXME Should not use printf in signal handlers.
// It modifes the program's global state or something.
void	handle_sigint(int sig)
{
	if (sig)
	{
//		printf("\n");	// does the blank line
		rl_on_new_line();	// Withhout this, the cursor just goes to the start of theline,.
 		rl_replace_line("", 0);	// without thiss, the ^C stays there
		rl_redisplay();// Withhout this, the cursor just goes to the start of theline,.
	}
}

// When running a long process in a pipe (basically only heredocs for us),
// this handler takes over from the normal one.
// TODO Define necessary clearup
void	handle_sigint_in_hd()
{
	exit(EXIT_SUCCESS);
}

// Setup the functions to handle each signal we have to manage.
// DONE Implement signals handling SIGINT - needs fixed
// TODO Implement signals handling CTRL-D EOF
// DONE Implement signals handling CTRL-\ uncontrolled quit
int	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
