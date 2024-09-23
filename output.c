/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:34:13 by chaikney          #+#    #+#             */
/*   Updated: 2024/09/23 10:34:21 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Make the calls and checks needed to set up output,
// whether redirected, in a pipe or at the end of it.
// Designed to be called from a child process after a pipe has been set up.
// - A valid o_file that is *not* STDOUT is patched to STDOUT.
// - If we have been passed pipe fds (tube) we patch STDOUT to that.
// - When there is no tube to be passed to we leave STDOUT as is
// (This is the why run_final_cmd passes NULL to this function.)
void	wire_up_output(int o_file, int *tube)
{
	if ((o_file >= 0) && (o_file != STDOUT_FILENO))
	{
		dup2(o_file, STDOUT_FILENO);
		close (o_file);
	}
	else if (tube)
		dup2(tube[1], STDOUT_FILENO);
	else
		return ;
}

// Take the output marker (> or >>) and the path (the next term)
// Open a file with the appropriate permissions.
// Return its file descriptor, or -1 if failed.
// NOTE file mode is set to have NO EXEC permission so we don't...
// ...attempt to execute any file with the same name as a program in the wd
// TODO Would be good to check validity of o_path here...
// ....how do we know what a valid o_path would be?
int	get_output_fd(char *mark, char *o_path)
{
	int		perms;
	int		o_fd;

	perms = O_WRONLY | O_CREAT | O_TRUNC;
	if (ft_strncmp(mark, ">>", 2) == 0)
		perms = O_WRONLY | O_CREAT | O_APPEND;
	o_fd = open(o_path, perms, 0666);
	if (o_fd == -1)
	{
		g_procstatus = errno;
		perror("Could not open output file");
	}
	return (o_fd);
}

// Read a command (which may be part of a pipe sequence)
// work out where the output goes to, by checking for > symbol(s)
// open the file with appropriate perms and return the fd to it.
// - Find the marker.
// - Send marker and next term to get_output_fd
// TODO Should we reject an i that has 3 or more chars?
// TODO Should we guard here aagainst > only having NULL in front?
int	determine_output(t_command *cmd)
{
	int		i;
	int		o_fd;

	i = cmd->argc;
	o_fd = STDOUT_FILENO;
	while ((i-- > 0))
	{
		if (ft_strncmp(cmd->argv[i], ">", 1) == 0)
			o_fd = get_output_fd(cmd->argv[i], cmd->argv[i + 1]);
	}
	remove_cmd_parts(cmd, ">");
	return (o_fd);
}
