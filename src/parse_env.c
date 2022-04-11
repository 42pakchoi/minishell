/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpak <cpak@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:41:00 by cpak              #+#    #+#             */
/*   Updated: 2022/04/07 16:41:03 by cpak             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_parsed_str(char *str, char *env_name, char *env_value)
{
    char    *new;
    int     len;
    int     i;

    i = 0;
    len = ft_strlen(str) - ft_strlen(env_name) + ft_strlen(env_value);
    new = malloc(sizeof(char) * len);
    if (!new)
        return (0);
    new[len] = 0;
    while (str[i] != '$')
    {
        new[i] = str[i];
        i += 1;
    }
    while (*env_value)
    {
        new[i] = *env_value;
        i += 1;
        env_value += 1;
    }
    while (str[i - ft_strlen(env_value) + ft_strlen(env_name) - 2])
    {
        new[i] = str[i - ft_strlen(env_value) + ft_strlen(env_name) - 2];
        i += 1;
    }
    free(str);
    return (new);
}

int parse_str_env(char **str)
{
    int     i;
    int     start;
    char    *env_name;
    char    *env_value;

    i = 0;
    start = -1;
    while ((*str)[i])
    {
        if ((*str)[i] == '$')
            start = i;
        else if (start != -1 && !ft_isalpha((*str)[i]) && !ft_isdigit((*str)[i]) && (*str)[i] != '_')
        {
            env_name = ft_strndup((*str) + start, i - start);
            env_value = find_var_value(env_name + 1);
            *str = get_parsed_str(*str, env_name, env_value);
            free(env_name);

            i = start;
            start = -1;
        }
        i += 1;
    }
    if (start != -1)
    {
        env_name = ft_strndup((*str) + start, i - start);
        env_value = find_var_value(env_name + 1);
        *str = get_parsed_str(*str, env_name, env_value);
        free(env_name);
    }
    return (1);
}

int parse_cmd_env(char **cmd)
{
    int idx;

    idx = 0;
    while (cmd[idx])
    {
        parse_str_env(&cmd[idx]);
        idx += 1;
    }
    return (1);
}