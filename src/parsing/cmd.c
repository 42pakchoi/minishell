/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpak <cpak@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 04:54:47 by cpak              #+#    #+#             */
/*   Updated: 2022/05/09 02:11:58 by cpak             ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_args_to_prev_cmd(char **strarr)
{
	t_cmd	*node;
	char	**new_arr;

	node = find_last_cmd();
	if (node)
	{
		new_arr = ft_strarr_join(node->strarr, strarr);
		free(node->strarr);
		free(strarr);
		node->strarr = new_arr;
		return ;
	}
	add_cmd(strarr, 1);
}

static int	set_new_cmd(char **strarr, int sep)
{
	char	**shifted;

	if (sep != 1)
	{
		shifted = ft_strarr_shift(&strarr);
		set_args_to_prev_cmd(strarr);
		if (!add_cmd(shifted, sep))
			return (0);
	}
	else
	{
		if (!add_cmd(strarr, sep))
			return (0);
	}
	return (1);
}

static void	set_next_op(char **str, int *sep)
{
	*str += get_len_to_next(str, is_op);
	*str += is_op(*str, sep);
}

static int	set_strarr_to_list(char **arr, char *str, int sep)
{
	char	**strarr;
	int		idx;

	idx = 0;
	while (arr[idx])
	{
		strarr = split_with_quote(arr[idx], is_white_space);
		if (!strarr)
			return (0);
		if (!parse_cmd(&strarr))
			return (0);
		if (!set_new_cmd(strarr, sep))
			return (0);
		set_next_op(&str, &sep);
		idx += 1;
		if (sep != 0 && arr[idx] == 0)
			g_mini.exit_status = (unsigned char)95;
	}
	return (1);
}

int	set_cmd_list(char *str)
{
	char	**arr;
	int		sep;

	while (is_white_space(str, &sep))
		str += 1;
	is_op(str, &sep);
	if (sep == 1)
		g_mini.exit_status = (unsigned char)95;
	else if (sep == 0)
		sep = 1;
	arr = split_with_quote(str, is_op);
	if (ft_strarr_len(arr) == 0)
		g_mini.exit_status = (unsigned char)95;
	if (!set_strarr_to_list(arr, str, sep))
		return (0);
	ft_free_arr(arr);
	return (1);
}
