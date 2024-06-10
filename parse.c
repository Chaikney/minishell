#include "minishell.h"

// TODO Add 42 header to parse.c

// TODO parse has to recognise redirection tokens: | < > << >>
// DONE Change strcspn calls to another function
// TODO Implement parseBuiltIn - simple match against the list of builtins?
//  - echo with -n (newline or not)
//  - cd with only a relative or absolute path
//  - pwd (no options)
//  - unset (no options)
//  - env, no options or args
//  - exit no options.
// Parse input from cmdline into a command struct
// Return values:
// 1 - ?
int	parse(const char *cmdline, t_command *cmd)
{
    static char 	array[MAXARGS];
    const char 	delims [10] = "\t\r\n";
    //char line = array;
    char 	**token;
    //char *endline;
    int 	is_bg;

    if (cmdline == NULL)
        perror("command line is NULL\n");
    //(void) ft_strncpy(line, cmdline, MAXARGS);
    //endline = line + ft_strlen(line);
    token = ft_split(cmdline, ' ');
    cmd->argc = 0;
    while (token[cmd->argc] != NULL) 
    {
        cmd->argv[cmd->argc] = token[cmd->argc];
        cmd->argc++;

        if (cmd->argc >= MAXARGS - 1) {
            break;
        }
    }
    //while (line < endline) 
    //{

      //  line += strcspn (line, delims);
        //if (line > endline) 
          //  break;

 /*        token = line + strcspn (line, delims);

        *token = '\0';

        cmd->argv[cmd->argc++] = line;

        if (cmd->argc > MAXARGS - 1) 
            break;
        line =  token + 1
    } */
    cmd->argv[cmd->argc] = NULL;
    free(token);
    if (cmd->argc == 0)
        return (1);
    cmd->builtin = parseBuiltin (cmd);

    if (is_bg (*cmd->argv[cmd->argc-1])) //== '6')) != 0
        cmd->argv[--cmd->argc] = NULL;
    return (is_bg);
}
