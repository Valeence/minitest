/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zack <zack@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:41:48 by vandre            #+#    #+#             */
/*   Updated: 2024/05/29 18:28:27 by zack             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/MiniShell.h"
#define MAX_ARGS 128

// int check_egal(char *str)
// {
//     int i;

//     i = 0;
//     while (str[i] != '\0')
//     {
//         if (str[i] == '=')
//             return (1);
//         i++;
//     }
//     return (0);
// }

// void printf_egal(char *str)
// {
//     int i;

//     i = 0;
//     while (str[i] != '\0')
//     {
//         if (str[i] == '=')
//         {
//             while (str[i] != '\0')
//             {
//                 write(1, &str[i], 1);
//                 i++;
//             }
//         }
//         i++;
//     }
// }

// int print_expand_word_quote(char *str, int i, char **envp)
// {
//     int j;
//     char *var;

//     j = i;
//     if (str[++i] == ' ' || str[++i] == '\t' || str[++i] == '\0')
//     {
//         write(1, "$", 1);
//         return (i);
//     }
//     while(str[i] != '=' && str[i] != '\0')
//         i++;
//     var = ft_substr(str, j, i - j - 1);
//     if (check_egal(var) == 1)
//         return (printf_egal(var), i + 1);
//     while(*envp)
//     {
//         if (ft_strncmp(*envp, var, ft_strlen(var)) == 0)
//         {
//             write(1, *envp + ft_strlen(var) + 1, ft_strlen(*envp) - ft_strlen(var) - 1);
//             break;
//         }
//         envp++;
//     }
//     free(var);
//     return (i + 1);
// }

// int print_expand_word(char *str, int i, char **envp)
// {
//     int j;
//     char *var;

//     j = i;
//     if (str[++i] == ' ' || str[++i] == '\t' || str[++i] == '\0')
//     {
//         write(1, "$", 1);
//         return (i);
//     }
//     while (str[i] != ' ' && str[i] != '\t' && str[i] != '\0')
//         i++;
//     var = ft_substr(str, j, i - j);
//     while (*envp)
//     {
//         if (ft_strncmp(*envp, var, ft_strlen(var)) == 0)
//         {
//             write(1, *envp + ft_strlen(var) + 1, ft_strlen(*envp) - ft_strlen(var) - 1);
//             break;
//         }
//         envp++;
//     }
//     free(var);
//     return (i);
// }

// int print_double_quote(char *str, int i, char **envp)
// {
//     i++;
//     if (str[i] == '"')
//         return (i + 1);
//     while (str[i] != '"' && str[i] != '\0')
//     {
//         if (str[i] == '$' && str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0')
//         {
//             i = print_expand_word_quote(str, i + 1, envp);
//         }
//         else
//         {
//             write(1, &str[i], 1);
//             i++;
//         }
//     }
//     return (i + 1);
// }

// int check_flag(char *str, int *i)
// {
//     int initial_i;
    
//     initial_i = *i;
//     while (str[*i] == ' ' || str[*i] == '\t')
//         (*i)++;
//     if (str[*i] == '-')
//     {
//         (*i)++;
//         while (str[*i] == 'n')
//             (*i)++;
//         if (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\0')
//             return 1;
//         else
//             *i = initial_i;
//     }
//     return (0);
// }

// int init_token_str(char *str)
// {
//     int i;
//     i = 0;
//     while (str[i])
//     {
//         if (str[i] == 'e' && str[i+1] == 'c' && str[i+2] == 'h' && str[i+3] == 'o')
//         {
//             i += 4;
//             break;
//         }
//         i++;
//     }
//     return (i);
// }

// int print_single_quote(char *str, int i)
// {
//     i++;
//     while (str[i] != '\'' && str[i] != '\0')
//     {
//         write(1, &str[i], 1);
//         i++;
//     }
//     return (i + 1);
// }

// int print(char *str, int i, char **envp)
// {
//     while (str[i] != ' ' && str[i] != '\t' && str[i] != '\0')
//     {
//         if (str[i] == '$')
//             i = print_expand_word(str, i + 1, envp);
//         else
//         {
//             while (str[i] == '"' || str[i] == '\'')
//                 i++;
//             write(1, &str[i], 1);
//             i++;
//         }
//     }
//     return (i);
// }

// int ft_echo(t_Token *curr, char **envp)
// {
//     int i;
//     int flag;
//     int first_arg;

//     first_arg = 1;
//     i = init_token_str(curr->Token_str);
//     flag = check_flag(curr->Token_str, &i);
//     while (curr->Token_str[i])
//     {
//         while (curr->Token_str[i] == ' ' || curr->Token_str[i] == '\t')
//             i++;
//         if (curr->Token_str[i] == '\0')
//             break;
//         if (!first_arg && !(curr->Token_str[i] == '"' && curr->Token_str[i - 1] == '"'))
//             write(1, " ", 1);
//         if (curr->Token_str[i] == '"')
//             i = print_double_quote(curr->Token_str, i, envp);
//         else if (curr->Token_str[i] == '\'')
//             i = print_single_quote(curr->Token_str, i);
//         else
//             i = print(curr->Token_str, i, envp);
//         first_arg = 0;
//     }
//     if (!flag)
//         write(1, "\n", 1);
//     return 1;
// }

// void print_env(char *str, char **envp)
// {
//     int start;
//     int len;

//     start = 1;
//     while (str[start] != '\0')
//     {
//         len = 0;
//         while (str[start + len] && str[start + len] != '$' && str[start + len] != ' ' && str[start + len] != '\t' && str[start + len] != '\0')
//             len++;
//         print(str + start, len, envp);
//         start += len;
//         if (str[start] == '$')
//             start++;
//     }
// }

int check_flag(char *str)
{
    int i;

    i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    if (str[i] == '-')
    {
        i++;
        while (str[i] == 'n')
            i++;
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\0')
            return 1;
    }
    return (0);
}

void print(char *str, int len, char **envp)
{
    int i;
    char *env_name;

    i = 0;
    while (envp[i])
    {
        env_name = strchr(envp[i], '=');
        if (env_name && (env_name - envp[i]) == len && ft_strncmp(envp[i], str, len) == 0)
        {
            write(1, env_name + 1, ft_strlen(env_name + 1));
            break;
        }
        i++;
    }
}

void print_env(char *str, char **envp, t_data *data)
{
    int start;
    int len;

    start = 1;
    while (str[start] != '\0')
    {
        len = 0;
        while (str[start + len] && str[start + len] != '$' && str[start + len] != ' ' && str[start + len] != '\t' && str[start + len] != '\0')
            len++;
        if (str[start] == '?' && (str[start + 1] == ' ' || str[start + 1] == '\t' || str[start + 1] == '\0'))
        {
            ft_printf(1, "%d", data->statusp);
            return ;
        }
        else
        {
            print(str + start, len, envp);
            start += len;
        }
        if (str[start] == '$')
            start++;
    }
}

int ft_echo(t_data *data, t_Token *curr, char **envp)
{
    char **args;
    int i;
    int flag;

    args = ft_split(curr->Token_str, ' ');
    flag = check_flag(args[1]);
    i = 1 + flag;
    while (args[i])
    {
        if (args[i][0] == '$' && args[i][1] != ' ' && args[i][1] != '\t' && args[i][1] != '\0')
            print_env(args[i], envp, data);
        else
            write(1, args[i], ft_strlen(args[i]));
        if (args[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!flag)
        write(1, "\n", 1);
    return 1;
}

