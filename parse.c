#include "minishell.h"

int parseBuiltin(struct command *cmd) 
{
    if (cmd->argc == 0) 
        return (0);
    if (ft_strncmp(cmd->argv[0], "cd", 2) == 0) 
        return (1);
    else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0) 
        return (2);
    else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
        return (3);
    else if (ft_strncmp(cmd->argv[0], "echo -n", 7) == 0)
        return (4);
    else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
        return (5);
    else if (ft_strncmp(cmd->argv[0], "export", 6) == 0)
        return (6);
    else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0)
        return (7);
    else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
        return (8);
    
    return (0);
}

// falta por añadir que hace cada 
// TODO? Need to pass envp here for some of the commands, or maybe not if we 
// cmd builtin == 1 I think its wrong
void executeBuiltin(struct command *cmd, char **envp) 
{
    while (1) 
    {
        if (cmd->builtin == 1) 
        {
            if (cmd->argc < 2) 
                fprintf(stderr, "cd: missing argument\n");
            else 
            {
                if (chdir(cmd->argv[1]) != 0) 
                    perror("cd");
            }
            return;
        } 
        else if (cmd->builtin == 2) 
            exit(0);
        else if (cmd->builtin == 3) 
        {
            
        }
        else if (cmd->builtin == 4) 
        {
        }
        else if (cmd->builtin == 5)  
            ms_pwd();
        else if (cmd->builtin == 6) 
        {
        }
        else if (cmd->builtin == 7) 
        {
        }
        else if (cmd->builtin == 8) 
        {
        }
        else 
        {
            fprintf(stderr, "Unknown builtin command\n");
            return;
        }
    }
}


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