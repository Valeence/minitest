#include "../Include/MiniShell.h"

bool is_digit(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] < 48 || str[i] > 57)
            return(false);
        i++;
    }
    return(true);
}   

void ft_exit(t_data *data, t_Token *curr)
{
    char **str;
    int code;

    ft_printf(2, "exit\n");
    str = ft_split(curr->Token_str, ' ');
    
    if (str!= NULL && str[1]!= NULL && str[2] == NULL)    
    {    
        if(is_digit(str[1]) == false)
        {
            ft_printf(2, "bash: exit: %s: numeric argument required", str[1]);
            data->statusp = 2;
        }
    }
    else if (str!= NULL && str[1]!= NULL)
    {
        code = ft_atoi(str[1]);
        if(code > 255)
            code = 255;
        data->statusp = code;
    }
    free_token(curr);
    ft_free_tabtab(str);
    exit(data->statusp);
}
