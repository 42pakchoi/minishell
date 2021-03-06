/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpak <cpak@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 05:38:27 by cpak              #+#    #+#             */
/*   Updated: 2022/05/11 14:35:40 by cpak             ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*change_str(char *str, char *str_tar, char *str_src, int idx)
{
	char	*new;
	int		len_tar;
	int		len_src;
	int		i;

	i = -1;
	len_tar = ft_strlen(str_tar);
	len_src = ft_strlen(str_src);
	new = (char *)ft_calloc(
			ft_strlen(str) - len_tar + len_src + 1, sizeof(char));
	if (!new)
	{
		free(str);
		return (0);
	}
	while (++i < idx)
		new[i] = str[i];
	i += ft_strlcpy(new + i, str_src, len_src + 1);
	i += ft_strlcpy(new + i, str + i - len_src + len_tar,
			ft_strlen(str + i - len_src + len_tar) + 1);
	new[ft_strlen(str) - len_tar + len_src] = 0;
	free(str);
	return (new);
}

static char	*trans_env_name_to_value(char *str, int *start, int *end)
{
	char	*env_name;
	char	*env_value;
	char	*parsed_str;

	env_name = ft_strndup(str + *start, *end - *start);
	if (!env_name)
		return (0);
	if (env_name[1] == '?')
		env_value = ft_itoa(g_mini.exit_status);
	else
		env_value = find_var_value(env_name + 1);
	parsed_str = change_str(str, env_name, env_value, *start);
	if (!parsed_str)
		free(str);
	*start = *end + ft_strlen(env_value) - ft_strlen(env_name);
	if (env_name[1] == '?')
		free(env_value);
	free(env_name);
	return (parsed_str);
}

static int	is_valid_env_name(char c)
{
	if (ft_isalpha(c) || ft_isdigit(c) || c == '_')
		return (1);
	return (0);
}

static int	get_next_env_point(char *str, int *start, int *end)
{
	while (str[*start] && str[*start] != '$')
		*start += 1;
	*end = *start + 1;
	if (str[*end] && str[*end] == '?')
	{
		*end += 1;
		return (1);
	}
	if (str[*end] && ft_isdigit(str[*end]))
	{
		*end += 1;
		return (1);
	}
	while (str[*end] && is_valid_env_name(str[*end]))
		*end += 1;
	if (!str[*start])
		return (0);
	return (1);
}

int	trans_all_env(char **str)
{
	int		start;
	int		end;
	char	*tmp;

	start = 0;
	end = 0;
	while (get_next_env_point(*str, &start, &end))
	{
		if (end - start == 1)
			start = end;
		else
		{
			tmp = trans_env_name_to_value(*str, &start, &end);
			if (tmp == 0)
			{
				if (!(*str))
					free(*str);
				return (0);
			}
			*str = tmp;
		}
	}
	return (1);
}
