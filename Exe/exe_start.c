#include "../Include/MiniShell.h"

bool  shr_out_in_redi(t_Token *curr)
{
    t_Token     *shr_out; 

    shr_out = curr;
    while(shr_out != NULL && shr_out->next != NULL)
    {   
        if(shr_out->Token_str[0] == '|')
            return(true);
        if(shr_out->type == E_OUT || shr_out->type == E_APPEND)
            return(true);
        shr_out = shr_out->next;
    }    
    return(false);
}

int  cont_nb_in(t_Token *curr)
{
    t_Token     *shr_out; 
    int i = 0;

    shr_out = curr;
    while(shr_out != NULL && shr_out->next != NULL)
    {   
        if(shr_out->Token_str[0] == '|' || shr_out->type == E_OUT || shr_out->type == E_APPEND )
            return(i);
        if (shr_out->type == E_IN)
            i++;
        shr_out = shr_out->next;
    }    
    return(i);
}

bool search_redi(t_Token *curr)
{
    t_Token *temp = curr ;

    if(temp->next->type == E_OUT)
        return(true);
    if (temp->next->type == E_APPEND)
        return(true);
    return(false);
}

int check_builtins(t_data *data, t_Token *curr, char **envp)
{
    if (ft_strcmp(curr->Token_str, "echo") == 1)
            ft_echo(data, curr, envp);
    else if ((ft_strcmp(curr->Token_str, "cd") == 1))
            ft_cd(curr);
    else if (ft_strcmp(curr->Token_str, "pwd") == 1)
            ft_pwd();
    else if (ft_strcmp(curr->Token_str, "export") == 1)
            ft_export(data, curr);
    else if (ft_strcmp(curr->Token_str, "unset") == 1)
            ft_unset(data, curr);
    else if (ft_strcmp(curr->Token_str, "env") == 1)
            ft_env(envp);
    else if (ft_strcmp(curr->Token_str, "exit") == 1)
            ft_exit(data, curr);
    else
        return (1);
    return (0);
}

void execute_pipeline_redirection(t_data *data, t_Token *list_token) 
{
    t_Token *curr;
    data->last = data->nb_cmd;
    curr = list_token;
    data->index = 0;
    pid_t pid[data->last];
    
    while (curr != NULL)
    {   
        if (curr->type == E_WORD && curr->next != NULL && curr->next->type == E_HEREDOC && curr->next->next->type == E_OEF_HERE)    
                execute_double_in_1(data, &curr);
        else if ((curr->type == E_HEREDOC && curr->next != NULL && curr->next->type == E_OEF_HERE))
                execute_double_in_1(data , &curr);
        if (curr != NULL && curr->next != NULL && curr->next->next != NULL &&  ((curr->type == E_WORD && curr->next->type == E_IN && curr->next->next->type == E_FILE) 
                || (curr->type == E_IN && curr->next->type == E_FILE && curr->next->next->type == E_WORD)))
                execute_in(data, list_token, &curr, pid);
        if (curr != NULL && curr->next != NULL && curr->next->next != NULL && ((curr->type == E_WORD && search_redi(curr) && curr->next->next->type == E_FILE) || (curr->type == E_PIPE && curr->next->type == E_WORD && search_redi(curr->next) && curr->next->next->next->type == E_FILE) || (curr->type == E_OEF_HERE && search_redi(curr))))  
                execute_redi_out_and_append(data, &curr, pid);
        if ((curr != NULL && curr->type == E_WORD && ((curr->next != NULL && curr->next->type == E_PIPE) || (curr->next->next != NULL && curr->next->next->type == E_PIPE))) 
                || (curr != NULL && curr->type == E_PIPE && curr->next != NULL && curr->next->type == E_WORD  && curr->next->next == NULL))
                execute_pipe(data, list_token, &curr, pid);
        if (curr == NULL)
            break;    
        curr = curr->next;
    }
    wait_for_children(data, data->index, pid);
}


void ft_lanch_exe(t_data *data, t_Token *list_token) 
{ 
    
    int size = 0;
    size = ft_size_list_token(list_token);
        
        if (size < 2 && list_token->next == NULL)
        {    
            if(check_builtins(data, list_token, data->envp) == 1)
                ft_simple_cmd(data, list_token);
        }
        else
                execute_pipeline_redirection(data, list_token);
}    
