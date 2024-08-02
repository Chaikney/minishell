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

// Allocate and safely set up a t_command struct (object).
// The memory allocated here is completely freed in clear_t_command
t_command	*init_new_command(void)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	new_cmd->argc = 0;
	new_cmd->builtin = NONE;
	new_cmd->next = NULL;
	return (new_cmd);
}

// Assign tokens to parts of a command struct, include with list.
// When it reaches a pipe character it stops.
// Outside the function must handle ->next
// Return NULL when we finish.
// This retains the I/O redir but removes the pipes.
// (The pipes are replaced with the ->next)
// NOTE We copy the values to make it easier to can wipe all tokens.
t_command	*build_command(char **tokens)
{
	static int	i;
	int			j;
	int			num_tokens;
	t_command	*new_cmd;

	j = 0;
	num_tokens = count_tokens(tokens);
	new_cmd = init_new_command();
	while ((i < num_tokens) && (tokens[i])
		&& (ft_strncmp(tokens[i], "|", 1) != 0))
	{
		new_cmd->argv[j++] = ft_strdup(tokens[i++]);
		new_cmd->argc++;
	}
	new_cmd->argv[new_cmd->argc] = NULL;
	if (tokens[i] == NULL)
		i = 0;
	else
		i++;
	print_tokens(tokens);
	// NOTE here we try to decide if we have a redirection and route around it
	if ((new_cmd->argv[0]) &&
		(ft_isalpha(new_cmd->argv[0][1]) == 1))
		new_cmd->builtin = parse_builtin(new_cmd, 0);
	else
		new_cmd->builtin = parse_builtin(new_cmd, 2);
	return (new_cmd);
}

t_command	*build_command_list(char **tokens)
{
	int			num_pipes;
	t_command	*next_cmd;
	t_command	*cmd_ptr;
	t_command	*cmd_head;

	num_pipes = count_pipes(tokens);
	cmd_head = build_command(tokens);
	cmd_ptr = cmd_head;
	while (num_pipes-- > 0)
	{
		next_cmd = build_command(tokens);
		cmd_ptr->next = next_cmd;
		cmd_ptr = cmd_ptr->next;
	}
	return (cmd_head);
}

// Take an array of strings (tokens from the split cmdline)
// Return the number of pipe characters it contains.
int	count_pipes(char **arr)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	while (arr[i] != NULL)
		if (ft_strncmp(arr[i++], "|", 1) == 0)
			n++;
	return (n);
}
