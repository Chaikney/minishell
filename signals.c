#include "minishell.h"

// TODO Add 42 header

// this is to disable CRTL-\ and also the apparent EOF we get
// after a pipe ends
// "Ctrl-\ does nothing"
void	handle_sigquit()
{
	return ;
}

// catch CTRL-c / SIGINT
// TODO This should return to the normal prompt / readline call
// ...I think this leads to duplicated prompt message.
// "CTRL-C displays a new prompt on a new line."
void	handle_sigint(int sig)
{
	if (sig)
	{
		printf("\nwhat should i do? >");
	}
}

// Setup the functions to handle each signal we have to manage.
// TODO Implement signals handling SIGINT - needs fixed
// TODO Implement signals handling CTRL-D
// TODO Implement signals handling CTRL-\ uncontrolled quit
int	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);	// This works to supress ctrl-d but not the cmdline breakage
	return (0);
}
