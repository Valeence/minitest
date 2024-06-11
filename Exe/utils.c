#include "../Include/MiniShell.h"

void wait_for_children(t_data *data, int last, pid_t pid[]) 
{
	int j = 0; 
    int status = 0;
    
	while (j < last) 
    {
        waitpid(pid[j], &status, 0);
        if (WIFEXITED(status)) 
        {
            if (WEXITSTATUS(status) != 0 && gvalret == 0)
                gvalret = WEXITSTATUS(status);
			else
                gvalret = status;
        }  
        j++;
    }
	data->statusp = gvalret;
}


char *ft_strcat(char *dest, char *src)
{
	int i;
	int j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void	*ft_realloc(void *ptr, size_t newsize)
{
	char	*newptr;
	size_t	cursize;

	if (ptr == 0)
		return (malloc(newsize));
	cursize = sizeof(ptr);
	if (newsize <= cursize)
		return (ptr);
	newptr = malloc(newsize);
	ft_memcpy(ptr, newptr, cursize);
	free(ptr);
	return (newptr);
}

// char **ft_split_quoted(char const *s)
// {
//     char **result;
//     int i, j, k, in_quotes;

//     result = malloc((strlen(s) / 2 + 2) * sizeof(char *));
//     if (!result)
//         return NULL;

//     i = 0;
//     k = 0;
//     while (s[i])
//     {
//         while (s[i] == ' ')
//             i++;
//         j = i;
//         in_quotes = 0;
//         while (s[j] && (in_quotes || (s[j] != ' ')))
//         {
//             if (s[j] == '"')
//                 in_quotes = !in_quotes;
//             j++;
//         }
//         result[k] = malloc(j - i + 1);
//         if (!result[k])
//             return NULL;
//         strncpy(result[k], s + i, j - i);
//         result[k][j - i] = '\0';
//         k++;
//         i = j;
//     }
//     result[k] = NULL;
//     return result;
// }
