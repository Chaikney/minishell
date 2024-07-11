#include "minishell.h"

// TODO Add 42 header

// Take one cmd and split its arguments up until a |
// (assumed to be present).
// No worrying about redirects; copy as-is.
// NOTE No allocation neededc for t_command, but maybe for the args?
// TODO Wrap this in a loop
// TODO Set the other relevant t_command variables.
t_command	*split_pipe(t_command *cmd)
{
    int	i;
    t_command	*new_cmd;

    printf("\nAttempting to split a command\n");
    new_cmd = malloc(sizeof(t_command));
    i = 0;
    new_cmd->argc = 0;
    while (ft_strncmp(cmd->argv[i], "|", 1) != 0)
    {
        printf("copying: %s\t", cmd->argv[i]);
        new_cmd->argv[i] = cmd->argv[i];
        new_cmd->argc++;
        i++;
    }
    print_cmd_parts(cmd);
    print_cmd_parts(new_cmd);
    return(new_cmd);
}
