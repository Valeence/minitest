#include "../Include/MiniShell.h"

int check_env(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == 'e' && str[i + 1] == 'n' && str[i + 2] == 'v')
        {
            while (str[i] != ' ' && str[i] != '\t')
                i++;
            if (str[i] == '\0' )
                return 1;
        }
        i++;
    }
    return 0;
}

// int	ft_isalnum_env(int c)
// {
// 	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A'
// 			&& c <= 'Z') || c == '_')
// 	{
// 		return (1);
// 	}
// 	return (0);
// }