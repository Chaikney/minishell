#include "minishell.h"

// TODO Add 42 header

// this is to disable CRTL-\ and also the apparent EOF we get
// after a pipe ends
// "Ctrl-\ does nothing"
// FIXME This should do *something* no? Or should it be sigdelete?
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
// TODO Implement signals handling CTRL-D
// TODO Implement signals handling CTRL-\ uncontrolled quit
int	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);	// This works to supress ctrl-d but not the cmdline breakage
	return (0);
}
