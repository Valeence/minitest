#include "../Include/MiniShell.h"

void ft_free_tabtab(char **split_result) 
{
    int i = 0;

    if (split_result == NULL)
        return;
    while (split_result[i] != NULL) 
    {   
        free(split_result[i]);
        i++;
    }
    free(split_result);
}

void ft_simple_cmd(t_data *data, t_Token *list_token)
{
    pid_t pid;
    int status;

    data->path_exe = ft_get_reading(data, list_token);
    if (!data->path_exe)
    {
        ft_printf(2, "bash: %s: command not found \n", data->cmd[0]);
        ft_free_tabtab(data->cmd);
        gvalret = 127; // Setting the global return value when command is not found
        data->statusp = gvalret;
        return;
    }
    else
    {
        pid = fork();                   
        if (pid == 0)                   
        {
            if (execve(data->path_exe, data->cmd, data->envp) == -1) 
                exit(127);   
        }
        else                        
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) 
                gvalret = WEXITSTATUS(status);
            else
                gvalret = 1;
            data->statusp = gvalret;
            ft_free_tabtab(data->cmd); 
            free(data->path_exe);
        }
    }
}
