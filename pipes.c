/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:56:28 by chaikney          #+#    #+#             */
/*   Updated: 2024/07/16 17:56:32 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Take one cmd and split its arguments up until a |
// (assumed to be present).
// No worrying about redirects; copy as-is.
// NOTE No allocation needed for t_command, but maybe for the args?
// DONE Wrap this in a loop
// DONE Check to see what cmd->builtin should be
// FIXED Static to keep track of position in argv, but when do we reset it?
// FIXME < test | rev causes a segfault (when the is_alpha check takes place?)
// Because input is piped NOT a cmd...
// NOTE Also this is an invalid pipe (though seems OK) - no command before it!
// So really should just fail gracefully, Make sure | is not the first char?
// FIXME function split_pipe is too long.
t_command	*split_pipe(t_command *cmd)
{
	static int		i;
	int				j;
	t_command		*new_cmd;

	j = 0;
	printf("\nAttempting to split a command starting at posn: %i\n", i);	// HACK for debugging
	print_cmd_parts(cmd);	// HACK for debugging
	if (ft_strncmp(cmd->argv[0], "|", 1) == 0)
		perror("bad command format");	// TODO Throw appropriate error here.
	new_cmd = malloc(sizeof(t_command));	// FIXME Memory allocated here is not freed
	new_cmd->argc = 0;
	new_cmd->builtin = NONE;
	new_cmd->next = NULL;
//	printf("\nI can't crash here! this is %s", cmd->argv[i]);	// HACK for debugging
	while ((cmd->argv[i]) && (ft_strncmp(cmd->argv[i], "|", 1) != 0))
	{
//		printf("copying: %s\t", cmd->argv[i]);	// HACK for debugging
		new_cmd->argv[j] = cmd->argv[i];
		new_cmd->argc++;
		i++;
		j++;
	}
	new_cmd->argv[new_cmd->argc] = NULL;	// End the cmd with a NULL
	if (i >= cmd->argc - 1)	// reset if we have reached the end of the argvs
		i = 0;
	else
		i++;	// step over the pipe char
	printf("\nSplit a coomand to:");
	print_cmd_parts(new_cmd);	// HACK for debugging
	if ((cmd->argv[0]) && (ft_isalpha(new_cmd->argv[0][1]) == 1))
		new_cmd->builtin = parse_builtin(new_cmd, 0);
	else
		new_cmd->builtin = parse_builtin(new_cmd, 2);
	return (new_cmd);
}

// Split a command into n + 1 commands (n is the number of pipe chars)
// - assign some kind of structure to hold the list.
// - get a new command with split_pipe
// - free to_split?
// DONE Add a list pointer to t_command
// DONE Fix numbering this will overflow.
t_command	*make_cmd_list(t_command *to_split, int n)
{
	t_command	*cmd_head;
	t_command	*next_cmd;
	t_command	*cmd_ptr;
	int			i;

	i = 1;
	cmd_head = split_pipe(to_split);
	cmd_ptr = cmd_head;
	while (i <= n)
	{
		next_cmd = split_pipe(to_split);
		cmd_ptr->next = next_cmd;
		i++;
		cmd_ptr = cmd_ptr->next;
	}
	return (cmd_head);
}
