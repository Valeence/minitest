#include "../Include/MiniShell.h"
                                         
int  cont_nb_here(t_Token *curr)
{
    t_Token     *shr_here; 
    int i = 0;

    shr_here = curr;
    while(shr_here != NULL)
    {   
        if (shr_here->type == E_HEREDOC)
            i++;
        if(shr_here->Token_str[0] == '|' || shr_here->type == E_OUT || shr_here->type == E_APPEND)
            break;
        shr_here = shr_here->next;
    }    
    return(i);
}

void exe_cmd(t_data *data, t_Token *lst_tok)
{
    if (execve(data->path_exe, data->cmd,  data->envp) == -1) 
    {
        ft_free_tabtab(data->cmd);
        free_token(lst_tok);
        free(data->path_exe);
        free(data->str);
        exit(127);
    }
    //ft_free_tabtab(data->cmd);
    free_token(lst_tok);
    free(data->path_exe);     
}


pid_t execute_command_in_child(t_data *data, t_Token *curr) 
{
        pipe(data->here_fd);
        pid_t pid = fork();
        
        if (pid == 0) 
        {   
            close(data->here_fd[1]); 
            dup2(data->here_fd[0], STDIN_FILENO);  
            close(data->here_fd[0]);
            data->path_exe = ft_get_reading(data, curr);
            if (!data->path_exe) 
            {
                ft_printf(2, "bash: %s: command not found \n", data->cmd[0]);
                ft_free_tabtab(data->cmd);
                free_token(curr);
                free(data->str);
                gvalret = 127;
                return(100); 
            }
            exe_cmd(data, curr);
            free(data->str);
        } 
        else 
        {
            close(data->here_fd[0]);  
            if(data->redi_and_here == false)
                write(data->here_fd[1], data->str, ft_strlen(data->str));
            else
                data->str_here = ft_strndup(data->str, ft_strlen(data->str));
            close(data->here_fd[1]);
            waitpid(pid, 0, 0);
        }
    return pid;
}

int fork_here(t_Token *curr, t_data *data, t_here *here) 
{   
    data->str = malloc(1);
    if (data->str == NULL) 
        return (-1);
    data->str[0] = '\0';
    char *line;
    line = NULL;
    size_t len = 0; 
    char *new_str;

    while (1) 
    { 
        line = readline (">" );
        if (ft_strcmp(line, here->delim[0]) == 1) 
        {
            free(line);
            break;
        }
        len += ft_strlen(line) + 1;  // +1 pour le '\n'
        new_str = (char*)ft_calloc(len + 1, sizeof(char)); 
        if (new_str == NULL) 
        {
            free(data->str);
            free(line);
            return -1;
        }
        strcpy(new_str, data->str);  //refaire strc
        free(data->str);
        data->str = new_str;
        ft_strcat(data->str, line);
        ft_strcat(data->str, "\n"); 
    }
    if (here->exe == 1)
    {
        if(execute_command_in_child(data, curr) == 100)
        {
            gvalret = 127;
            return 2;
        }
    }   
    free(data->str);
    return (0);
}


void init_here_doc(t_data *data, t_Token *curr, int nb, t_here *here)
{
    int i = 0;
    t_Token *tmp = curr;
    int temp_i;
    temp_i = 0;
    while (i < nb)
    { 
        if (tmp->type == E_WORD && tmp->next->type == E_HEREDOC) 
        {  
                here[i].delim = ft_split(tmp->next->next->Token_str, ' ');
               if (tmp->next->next->next != NULL && (tmp->next->next->next->type == E_HEREDOC))
                {
                    tmp = tmp->next->next;
                    data->flag_here = 1;
                    here[i].exe = 0;
                }
                else
                    here[i].exe = 1;
            i++;
        }
        else if(tmp->type == E_HEREDOC)
        {
            here[i].delim = ft_split(tmp->next->Token_str, ' ');
            temp_i = i + 1;
            if(data->flag_here == 1 && temp_i == nb)
                here[i].exe = 1;
            else
                here[i].exe = 0;
            i++;
        }
        tmp = tmp->next;
    }
}


void lanch_her(t_Token *curr, t_data *data)
{
    data->curr_here_tmp = curr;
    int grab_hd = cont_nb_here(curr);
    data->here = malloc(sizeof(t_here) * grab_hd);
    init_here_doc(data, curr, grab_hd, data->here);
    int i = 0;
    
    while (i < grab_hd)
    {   
        if(fork_here(curr, data, &data->here[i]) == 2)
            break;
        i++;
    }
    grab_hd = i; 
    for (i = 0; i < grab_hd; i++)
        ft_free_tabtab(data->here[i].delim);
    free(data->here);
}