#include "../Include/MiniShell.h"

void free_for_fail_exe(t_data *data, t_Token *curr)
{
    if (data->cmd[0] != NULL && data->cmd[0][0] != '\0')
        ft_free_tabtab(data->cmd);
    free(data->path_exe);
    free_token(curr);
}

void close_fd_out(t_data *data)
{
    int i = 0;
    
    while(i < data->nb_file_out)
    {    
        close(data->output_fd[i]);
        i++;
    }
    free(data->output_fd);
}

int *creat_fd_out(t_data *data, t_Token *curr)
{
    int i;
    i = 0;
    t_Token *tmp = curr;

    while(i < data->nb_file_out)
    { 
        if (tmp->next->next->type == E_FILE) 
        {       
            tmp->next->next->file = ft_split(tmp->next->next->Token_str, ' ');
            if(tmp->next->type == E_OUT)
            {
                data->output_fd[i] =  open(tmp->next->next->file[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if(data->output_fd[i] == -1)
                {
                    printf("bash: %s: Permission denied\n", curr->next->next->file[0]);
                    gvalret = 1;
                    exit(1);
                }
            }
            if (tmp->next->type == E_APPEND)
            {
                data->output_fd[i] =  open(tmp->next->next->file[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
                if(data->output_fd[i] == -1)
                {
                    printf("bash: %s: Permission denied\n\n", curr->next->next->file[0]);
                    gvalret = 1;
                    exit(1);
                }
            }
            if (tmp->next->next->file[1] != NULL && tmp->next->next->file[1][0] != '\0')
                data->cmd = copy_array_arrays(tmp->next->next->file);
            i++;
        }
        tmp = tmp->next->next;
    }
    return(data->output_fd);
}

pid_t fork_process_redi_out(t_data *data, t_Token *curr)
{
    pid_t pid = fork(); 
    data->output_fd = malloc(data->nb_file_out * sizeof(int)); 

    if (pid == 0)
    {   
        if(data->redi_out_bf_pipe == 1)
        { 
            dup2(data->input_fd, STDIN_FILENO);
            close(data->input_fd);
        }
        data->output_fd = creat_fd_out(data, curr);
        dup2(data->output_fd[data->nb_file_out -1], STDOUT_FILENO);
        if(data->redi_and_here == true)
        {
            printf("%s", data->str_here); 
            free(data->str_here);
            close_fd_out(data);
            free_token(data->curr_here_tmp);
            exit(0);
        }
        if(check_builtins(data, curr, data->envp) == 0)
        {
            close_fd_out(data);
            free_token(curr);
            exit(0);
        }   
        data->path_exe = ft_get_reading(data, curr);
        if(!data->path_exe)
            ft_no_patch(data, curr);
        if(execve(data->path_exe, data->cmd, data->envp) == -1)
        {
            close_fd_out(data);
            free_for_fail_exe_exit(data, curr);
        }
        free_for_fail_exe(data, curr);
        close_fd_out(data);
    }
    return (pid);
}