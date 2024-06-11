/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vandre <vandre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:43:28 by vandre            #+#    #+#             */
/*   Updated: 2024/06/11 01:37:48 by vandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/MiniShell.h"

// void free_tokens(char **tokens, char *str)
// {
//     int i;

//     i = 0;
// 	if (str)
// 		free(str);
//     while (tokens[i])
//     {
//         free(tokens[i]);
//         i++;
//     }
//     free(tokens);
// }

// int add_new_var(t_data *data, char *str)
// {
//     int env_size;
//     char **new_env;

//     env_size = 0;
//     while (data->envp[env_size])
//         env_size++;
//     new_env = malloc(sizeof(char *) * (env_size + 2));
//     if (!new_env)
//         return -1;
//     env_size = 0;
//     while (data->envp[env_size])
//     {
//         new_env[env_size] = data->envp[env_size];
//         env_size++;
//     }
//     new_env[env_size] = ft_strdup(str);
//     if (!new_env[env_size])
//     {
//         free(new_env);
//         return -1;
//     }
//     new_env[env_size + 1] = NULL;
//     free(data->envp);
//     data->envp = new_env;
//     return 0;
// }


// int update_existing_var(t_data *data, char *str, int name_len)
// {
//     int j;

//     j = 0;
//     while (data->envp[j])
//     {
//         if (ft_strncmp(data->envp[j], str, name_len) == 0 && data->envp[j][name_len] == '=')
//         {
//             free(data->envp[j]);
//             data->envp[j] = ft_strdup(str);
//             if (!data->envp[j])
//                 return -1;
//             return 1;
//         }
//         j++;
//     }
//     return 0;
// }

// int find_name_len(char *str)
// {
//     int name_len;

//     name_len = 0;
//     while (str[name_len] && str[name_len] != '=')
//         name_len++;
//     return name_len;
// }

// char *remove_quotes(char *str)
// {
//     int len;
//     int i, j;
//     char *new_str;

//     len = strlen(str);
//     new_str = malloc(len + 1);
//     if (!new_str)
//         return NULL;

//     i = 0;
//     j = 0;
//     while (str[i])
//     {
//         if (str[i] != '"')
//         {
//             new_str[j] = str[i];
//             j++;
//         }
//         i++;
//     }
//     new_str[j] = '\0';
//     return new_str;
// }

// int	check_str(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (ft_strchr(str, '=') == NULL)
// 		return (ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str), 0);
// 	if (str[i] == '=')
// 	{
// 		ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
// 		return (0);
// 	}
// 	if (ft_isdigit(str[0]))
// 	{
// 		ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
// 		return (0);
// 	}
// 	while (str[i] && str[i] != '=')
// 	{
// 		if (!ft_isalnum_env(str[i]) && str[i] != '=')
// 		{
// 			ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (1);
// }


// int ft_export_utils(char **tokens, int i, t_data *data)
// {
//     int	res;
// 	int	name_len;
// 	char	*token_without_quotes;

// 	while (tokens[i])
//     {
//         if (check_str(tokens[i]) == 0)
// 			return (free_tokens(tokens, NULL), -1);
// 		name_len = find_name_len(tokens[i]);
//         token_without_quotes = remove_quotes(tokens[i]);
//         res = update_existing_var(data, token_without_quotes, name_len);
//         if (res == 0)
//         {
//             if (add_new_var(data, token_without_quotes) != 0)
//                 return (free_tokens(tokens, token_without_quotes), -1);
//         }
//         else if (res == -1)
// 			return (free_tokens(tokens, token_without_quotes), -1);
//         free(token_without_quotes);
//         i++;
//     }
//     free_tokens(tokens, NULL);
//     return 1;
// }

// int ft_export(t_data *data, t_Token *curr)
// {
// 	char **tokens;
// 	int i;

// 	tokens = ft_split_quoted(curr->Token_str);
// 	if (!tokens)
// 		return -1;
// 	i = 1;
// 	ft_export_utils(tokens, i, data);
// 	return 1;
// }

char **ft_split_quoted(char const *s)
{
    char **result;
    int i, j, k, in_quotes;

    result = malloc((strlen(s) / 2 + 2) * sizeof(char *));
    if (!result)
        return NULL;

    i = 0;
    k = 0;
    while (s[i])
    {
        while (s[i] == ' ')
            i++;
        j = i;
        in_quotes = 0;
        while (s[j] && (in_quotes || (s[j] != ' ')))
        {
            if (s[j] == '"')
                in_quotes = !in_quotes;
            j++;
        }
        result[k] = malloc(j - i + 1);
        if (!result[k])
            return NULL;
        strncpy(result[k], s + i, j - i);
        result[k][j - i] = '\0';
        k++;
        i = j;
    }
    result[k] = NULL;
    return result;
}

int	ft_isalnum_env(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A'
			&& c <= 'Z') || c == '_')
	{
		return (1);
	}
	return (0);
}

void free_tokens(char **tokens, char *str)
{
    int i;

    i = 0;
	if (str)
		free(str);
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

int add_new_var(t_data *data, char *str)
{
    int env_size;
    char **new_env;

    env_size = 0;
    while (data->envp[env_size])
        env_size++;
    new_env = malloc(sizeof(char *) * (env_size + 2));
    if (!new_env)
        return -1;
    env_size = 0;
    while (data->envp[env_size])
    {
        new_env[env_size] = data->envp[env_size];
        env_size++;
    }
    new_env[env_size] = ft_strdup(str);
    if (!new_env[env_size])
    {
        free(new_env);
        return -1;
    }
    new_env[env_size + 1] = NULL;
    free(data->envp);
    data->envp = new_env;
    return 0;
}


int update_existing_var(t_data *data, char *str, int name_len)
{
    int j;

    j = 0;
    while (data->envp[j])
    {
        if (ft_strncmp(data->envp[j], str, name_len) == 0 && data->envp[j][name_len] == '=')
        {
            free(data->envp[j]);
            data->envp[j] = ft_strdup(str);
            if (!data->envp[j])
                return -1;
            return 1;
        }
        j++;
    }
    return 0;
}

int find_name_len(char *str)
{
    int name_len;

    name_len = 0;
    while (str[name_len] && str[name_len] != '=')
        name_len++;
    return name_len;
}

char *remove_quotes(char *str)
{
    int len;
    int i, j;
    char *new_str;

    len = strlen(str);
    new_str = malloc(len + 1);
    if (!new_str)
        return NULL;

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] != '"')
        {
            new_str[j] = str[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0';
    return new_str;
}

int	check_str(char *str)
{
	int	i;

	i = 0;
	if (ft_strchr(str, '=') == NULL)
		return (ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str), 0);
	if (str[i] == '=')
	{
		ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (0);
	}
	if (ft_isdigit(str[0]))
	{
		ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
		return (0);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum_env(str[i]) && str[i] != '=')
		{
			ft_printf(2, "minishell: export: `%s': not a valid identifier\n", str);
			return (0);
		}
		i++;
	}
	return (1);
}


int ft_export_utils(char **tokens, int i, t_data *data)
{
    int	res;
	int	name_len;
	char	*token_without_quotes;

	while (tokens[i])
    {
        if (check_str(tokens[i]) == 0)
			return (free_tokens(tokens, NULL), -1);
		name_len = find_name_len(tokens[i]);
        token_without_quotes = remove_quotes(tokens[i]);
        res = update_existing_var(data, token_without_quotes, name_len);
        if (res == 0)
        {
            if (add_new_var(data, token_without_quotes) != 0)
                return (free_tokens(tokens, token_without_quotes), -1);
        }
        else if (res == -1)
			return (free_tokens(tokens, token_without_quotes), -1);
        free(token_without_quotes);
        i++;
    }
    free_tokens(tokens, NULL);
    return 1;
}

int ft_export(t_data *data, t_Token *curr)
{
	char **tokens;
	int i;

	tokens = ft_split_quoted(curr->Token_str);
	if (!tokens)
		return -1;
	i = 1;
	ft_export_utils(tokens, i, data);
	return 1;
}