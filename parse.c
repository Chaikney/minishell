#include "minishell.h"

// TODO Change strcspn calls to another function
int parse(const char *cmdline, struct command *cmd) 
{
    static char array[MAXLINE];
    const char delims [10] = "\t\r\n";
    //char line = array;
    char **token;
    //char *endline;
    int is_bg;
    if (cmdline == NULL)
        error("command line is NULL\n");
    //(void) ft_strncpy(line, cmdline, MAXLINE);
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
    if (cmd->argc = 0)
        return (1);
    cmd->builtin = parseBuiltin (cmd);

    if (is_bg (*cmd->argv[cmd->argc-1])) //== '6')) != 0
        cmd->argv[--cmd->argc] = NULL;
    return (is_bg);
}