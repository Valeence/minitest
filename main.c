#include "./Include/MiniShell.h"

int gvalret;

void init_data(t_data *data)
{
 	data->pipe_bf_redi = 0;
    data->redi_out_bf_pipe = 0;
	data->first = 0;
	data->nb_cmd = 0;
	data->nb_file_in = 0;
	data->redi_and_here = false;
	data->str_here = NULL;
}

// void    signal_handle()
// {
//     printf("\n");
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

char **copy_env(char **envp)
{
    int i = 0;
    while (envp[i])
        i++;
    
    char **new_env = malloc(sizeof(char *) * (i + 1));
    if (!new_env)
        return NULL;

    i = 0;
    while (envp[i])
    {
        new_env[i] = strdup(envp[i]);
        if (!new_env[i])
        {
            // Libérer la mémoire allouée en cas d'échec
            while (i > 0)
                free(new_env[--i]);
            free(new_env);
            return NULL;
        }
        i++;
    }
    new_env[i] = NULL;

    return new_env;
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	t_Token	*list_token = NULL;
	char **envp_copy;

	(void)ac;
	(void)av;
	// signal(SIGINT, signal_handle);
	// signal(SIGQUIT, SIG_IGN);	// char **envp_copy
	envp_copy = copy_env(env);
	data.envp = envp_copy;
	data.statusp = 0;
	while (1)
	{
		gvalret = 0;
		list_token = NULL;
		init_data(&data);
		data.pronpt = readline("Minishell$ ");
		if (!data.pronpt)
			break;
		if (!*data.pronpt)
			continue ;
		if (syntaxize_moi_ca(data.pronpt) == 1)
		{
			ft_lexer(&data, &list_token);
			//print_list(list_token);
			if(list_token != NULL)	
			{	
				ft_lanch_exe(&data, list_token);
				add_history(data.pronpt);
				free(data.pronpt);
				free_token(list_token);
			}
		}
	}
	// free_token(list_token);
	// free(data.str);
	// rl_clear_history();
	return (0);
}

/*
[ls][|][ls][>][avion][|][grep a] 
nbmaxpipe =0
while(node)
{
	while (i != nbmaxpipe)
	{
		if (node->type == PIPE)
			i++;
		node = node->next;
	}
	pipe(pipe)
	// if node->type == COMMAND
	if node->type == PIPE && node->next->type == WORD ou i = 0 && node->type == WORD 
		i++;
}
*/
/*
[ls][|][cat > avion][|][grep a] 
while(node)
{
	if (node->type == PIPE)
		node = node->next;
	pipe(pipe)
	fork
	//
		while (node && nbmaxpipe != numerodecommande)
		{
			if (node->type == PIPE)
				nbmaxpipe++
			node = node->next;
		}
	node = node->next;
	numerodecommande++;
}
*/