#include "minishell.h"

int parseBuiltin(t_command *cmd)
{
    if (cmd->argc == 0) 
        return (0);
    if ((ft_strncmp(cmd->argv[0], "cd", 2) == 0) && (ft_strlen(cmd->argv[0]) == 2))
        return (1);
    else if ((ft_strncmp(cmd->argv[0], "exit", 4) == 0) && (ft_strlen(cmd->argv[0]) == 4) && (cmd->argv[1] == NULL))
        return (2);
    else if ((ft_strncmp(cmd->argv[0], "echo", 4) == 0)&& (ft_strlen(cmd->argv[0]) == 4))
    {
        if ((ft_strncmp(cmd->argv[1], "-n", 2) == 0)&& (ft_strlen(cmd->argv[0]) == 2))
            return (4);
        else
            return (3);
    }
    else if ((ft_strncmp(cmd->argv[0], "pwd", 3) == 0) && (ft_strlen(cmd->argv[0]) == 3) && (cmd->argv[1] == NULL))
        return (5);
    else if ((ft_strncmp(cmd->argv[0], "export", 6) == 0)&& (ft_strlen(cmd->argv[0]) == 6))
        return (6);
    else if ((ft_strncmp(cmd->argv[0], "unset", 5) == 0)&& (ft_strlen(cmd->argv[0]) == 5))
        return (7);
    else if ((ft_strncmp(cmd->argv[0], "env", 3) == 0)&& (ft_strlen(cmd->argv[0]) == 3))
        return (8);
    return (0);
}

// falta por añadir que hace cada 
// TODO? Need to pass envp here for some of the commands, or maybe not if we 
// FIXME cmd builtin == 1 I think its wrong
void executeBuiltin(t_command *cmd, char **envp)
{
    (void) envp;	// HACK for compilation, remove this or parameter later.
    int	i;

    while (1) 
    {
        if (cmd->builtin == 1) 
        {
            if (cmd->argc < 2) 
                fprintf(stderr, "cd: missing argument\n");
            else 
            {
                if (chdir(cmd->argv[1]) != 0) 
                {
                    perror("cd");
                    printf("wrong address\n");
                }
            }
            return;
        } 
        else if (cmd->builtin == 2) 
            exit(0);
        else if (cmd->builtin == 3) 
        {
            i = 1;
            while (i < cmd->argc)
            {
                printf("%s", cmd->argv[i]);
                if (i < cmd->argc - 1)
                    printf(" ");
                i++;
            }
            printf("\n");
            return;
        }
        else if (cmd->builtin == 4) 
        {
            i = 2;
            while (i < cmd->argc)
            {
                printf("%s", cmd->argv[i]);
                if (i < cmd->argc - 1)
                    printf(" ");
                i++;
            }
            return;
        }
        else if (cmd->builtin == 5)
        {
            ms_pwd();
            return ;
        }
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
            printf("Unknown builtin command\n");
            return;
        }
    }
}


// TODO Change strcspn calls to another function
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
    // NOTE Don't need both array and **token, they do the same,
//    static char 	array[MAXARGS];
//    const char 	delims [10] = "\t\r\n";
    //char line = array;
    char 	**token;
    //char *endline;
    int 	is_bg;

    is_bg = 0;	// HACK for compilation, remove later.
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
	// FIXME What does this try to do, the if statement?
    /* if (is_bg (*cmd->argv[cmd->argc-1])) //== '6')) != 0 */
    /*     cmd->argv[--cmd->argc] = NULL; */
    return (is_bg);
}
