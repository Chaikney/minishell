#include "minishell.h"

// TODO Add 42 header

// Shell's exit clears the relevant variables and ends the program.
// This handles USER-REQUESTED exits only.
// NOTE This is different from exits in pipes or failed commands.
// - free cmd
// - what about the readline input variable?
// TODO If this is called from main, we have a char* to free
void	ms_exit(t_command *cmd)
{
	printf("Agurrrr....\n");
	clear_t_command(cmd);
	exit(EXIT_SUCCESS);
}

// Call this when we need to exit from a successful process that
// ran in a pipe but hasn't otherwise (i.e. via execve) been exited.
// SO, the builtins, basicallyy.
// TODO Ensure this works as expected and clears everything.
// (File descriptors were handled before calling this.)
void	exit_pipe(t_command *cmd)
{
	clear_t_command(cmd);
	exit (EXIT_SUCCESS);
}

// Clear the pathparts and close fds created by piping before exit.
// TODO This probably needs to be expanded and / or replaced for minishell use.
// TODO Should this be merged into or called by ms_exit?
// TODO The freeing of args here would be better handled by clear_t_command
void	exit_and_free(char **args, int fd_in, int fd_out)
{
	int	i;

	perror(strerror(errno));
	if ((fd_in) && (fd_in != -1))
		close(fd_in);
	if ((fd_out) && (fd_out != -1))
		close(fd_out);
	i = 0;
	if (args)
	{
		while (args[i])
			free(args[i++]);
	}
	exit (EXIT_FAILURE);
}

// Free the memory allocated to a t_command and all those following it.
// Frees all argv, including the final NULL
void	clear_t_command(t_command *cmd)
{
	int	i;

	if (cmd)
	{
		while (cmd->next != NULL)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv[cmd->argc]);
			cmd = cmd->next;
		}
		i = 0;
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv[cmd->argc]);
	}
}
