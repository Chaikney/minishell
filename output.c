#include "minishell.h"

// TODO Add 42 header

// Make the calls and checks needed to set up output,
// whether in a pipe or at the end of it.
// Designed to be called from a child process after a pipe has been set up.
// NOTE when there is no tube to be passed to we leave STDOUT as is
void	wire_up_output(int o_file, int *tube)
{
	// NOTE Check here is same as final_cmd - a valid output file gets STDOUT
	if ((o_file >= 0) && (o_file != STDOUT_FILENO))
	{
		dup2(o_file, STDOUT_FILENO);
		close (o_file);
	}
	else if (tube)
		dup2(tube[1], STDOUT_FILENO);	// otherwise, the output goes to the pipe as usual
	else
		return ;
}

// Read a command (which may be part of a pipe sequence)
// work out where the output goes to, by checking for > symbol(s)
// open the file with appropriate perms and return the fd to it.
// - Find the marker.
// - Set file perms for level (create or append)
// - find file name
// - check access to file, open it
// NOTE file mode is set to have NO EXEC permission so we don't...
// ...attempt to execute any file with the same name as a program in the wd
// TODO Would be good to check validity of o_path here...
// ....how do we know what a valid o_path would be?
// TODO Should we reject an i that has 3 or more chars?
// Can this be called on *any* command? I think yes, the problem before was
// the operation made 0 sense. (Still the case, but now we are doing it...)
int	determine_output(t_command *cmd)
{
	int		i;
	int		o_fd;
	int		perms;
	char	*o_path;

	i = cmd->argc;
	o_fd = STDOUT_FILENO;
	perms = 0;
	while ((i-- > 0))
	{
		if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
		{
			perms = O_WRONLY | O_CREAT | O_TRUNC;
			if (ft_strncmp(cmd->argv[i], ">>", 2) == 0)
				perms = O_WRONLY | O_CREAT | O_APPEND;
			o_path = cmd->argv[i + 1];
			o_fd = open(o_path, perms, 0666);
			if (o_fd == -1)
			{
				g_procstatus = errno;
				perror("Could not open output file");
			}
		}
	}
	remove_cmd_parts(cmd, ">");
	return (o_fd);
}
