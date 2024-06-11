#include "../Include/MiniShell.h"

bool shr_out_in_here(t_Token *lst)
{
    t_Token     *shr_out = lst; 

    while(shr_out != NULL && shr_out->next != NULL)
    {   
        if(shr_out->Token_str[0] == '|' || shr_out->type == E_IN)
            return(false);
        if (shr_out->type == E_OUT || shr_out->type == E_APPEND)
            return(true);
        shr_out = shr_out->next;
    }    
    return(false);
}

int  cont_nb_out(t_Token *curr)
{
    t_Token     *shr_out; 
    int i = 0;

    shr_out = curr;
    while(shr_out != NULL && shr_out->next != NULL)
    {   
        if(shr_out->Token_str[0] == '|' || shr_out->type == E_IN)
            return(i);
        if (shr_out->type == E_OUT || shr_out->type == E_APPEND)
            i++;
        shr_out = shr_out->next;
    }    
    return(i);
}

void    push_position(t_Token **curr, int grab_hd)
{
    int i;
    i = 0;

    while (*curr != NULL)
    {
        if((*curr)->type == E_HEREDOC)
            i += 1;
        if(i == grab_hd)
            break;
        if((*curr)->next == NULL || (*curr)->next->type == E_PIPE || search_redi((*curr)))
            break;
        else
            (*curr) = (*curr)->next;
    }

}

void    execute_double_in_1(t_data *data, t_Token **curr)
{
    int grab_hd = cont_nb_here(*curr);
    data->redi_and_here = shr_out_in_here(*curr);
    lanch_her(*curr, data);
    push_position(curr, grab_hd);  
}

/*peut la supp on en opti la condiotn dans exe_simple
void    execute_double_in_2(t_data *data, t_Token **curr)
{       
    pid[data->index] = -1;
    int grab_hd = cont_nb_here(*curr);
    lanch_her(*curr, data);
    push_position(curr, grab_hd);
    data->index++;    
+
}
*/
void    execute_in(t_data *data, t_Token *list_token, t_Token **curr, pid_t *pid)
{
    if(verif_pipe(*curr) == true)
    {
        pid[data->index] = -1;
        data->redi_out_bf_pipe = 1;
        create_pipe(data->pipe_fd, data->index, data->last);
        pid[data->index] = fork_process_redi_in(data, (*curr),  list_token);
        close_pipes_in_parent(data->index, data, &data->input_fd);
        data->index++;
    }
    else
    {
        pid[data->index] = -1;
        pid[data->index] = fork_process_redi_in(data, (*curr), list_token);
        data->index++;
    }
    if((*curr)->type == E_IN && (*curr)->next->type == E_FILE && (*curr)->next->next->type == E_WORD)
        (*curr) = (*curr)->next->next->next;
    else
        (*curr) = (*curr)->next;
}

void execute_redi_out_and_append(t_data *data, t_Token **curr, pid_t *pid)
{
    if(((*curr)->type == E_PIPE && (*curr)->next->type == E_WORD && search_redi((*curr)->next)) && (*curr)->next->next->next->type == E_FILE)
        (*curr) = (*curr)->next;
    data->nb_file_out = cont_nb_out(*curr);
    if(verif_pipe(*curr) == true)
    {
        data->redi_out_bf_pipe = 1;
        pid[data->index] = -1;
        create_pipe(data->pipe_fd, data->index, data->last);
        pid[data->index] = fork_process_redi_out(data, (*curr));
        close_pipes_in_parent(data->index, data, &data->input_fd);
        data->index++;
    }
    else 
    {   
        pid[data->index] = -1;
        pid[data->index] = fork_process_redi_out(data,  (*curr));
        data->index++;
    }
    free(data->output_fd);
    data->pipe_bf_redi = 0;
    if(data->redi_and_here == true)
        free(data->str_here);
    (*curr) = (*curr)->next;
    while ((*curr) != NULL)
    {
        if((*curr)->type == E_WORD || (*curr)->next == NULL || (*curr)->type == E_PIPE)
            break;
        else
            (*curr) = (*curr)->next;
    }
}

void    execute_pipe(t_data *data, t_Token *list_token, t_Token **curr, pid_t *pid)
{
    if ((*curr) != NULL && (*curr)->type == E_PIPE && (*curr)->next != NULL && (*curr)->next->type == E_WORD  && (*curr)->next->next == NULL)
            (*curr) = (*curr)->next;
    pid[data->index] = -1;
    create_pipe(data->pipe_fd, data->index, data->last);
    pid[data->index] = fork_process_pipe(data->index, data, (*curr), list_token);
    close_pipes_in_parent(data->index, data, &data->input_fd);
    data->index++;
    if ((curr != NULL && (*curr)->next != NULL && (*curr)->next->next != NULL && (*curr)->next->next->next != NULL) 
        && ((*curr)->next->next->next->type == E_OUT || (*curr)->next->next->next->type == E_APPEND))
            data->pipe_bf_redi = 1;
    data->redi_out_bf_pipe = 0;
}
