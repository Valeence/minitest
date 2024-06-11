/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vandre <vandre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:43:34 by vandre            #+#    #+#             */
/*   Updated: 2024/06/09 22:54:32 by vandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/MiniShell.h"

int find_env_var(char **envp, const char *var)
{
    int i;
    size_t len;

    i = 0;
    len = strlen(var);
    while (envp[i])
    {
        if (strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
            return i;
        i++;
    }
    return -1;
}

void freetokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

char **create_new_env(int env_size)
{
    char **new_env;

    new_env = malloc(sizeof(char *) * env_size);
    if (!new_env)
        return NULL;
    return new_env;
}

void copy_env_excluding_index(char **new_env, char **envp, int index)
{
    int i = 0, j = 0;
    while (envp[i])
    {
        if (i != index)
        {
            new_env[j] = envp[i];
            j++;
        }
        else
            free(envp[i]);
        i++;
    }
    new_env[j] = NULL;
}

void remove_env_var(t_data *data, int index)
{
    int env_size = 0;
    char **new_env;

    while (data->envp[env_size])
        env_size++;
    new_env = create_new_env(env_size);
    if (!new_env)
        return;
    copy_env_excluding_index(new_env, data->envp, index);
    free(data->envp);
    data->envp = new_env;
}


int ft_unset(t_data *data, t_Token *curr)
{
    char **tokens;
    int i;
    int index;

    tokens = ft_split_quoted(curr->Token_str);
    if (!tokens)
        return -1;

    i = 1;
    while (tokens[i])
    {
        index = find_env_var(data->envp, tokens[i]);
        if (index != -1)
            remove_env_var(data, index);
        i++;
    }
    freetokens(tokens);
    return 0;
}