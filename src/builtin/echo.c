/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarchoi <sarchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 16:59:56 by sarchoi           #+#    #+#             */
/*   Updated: 2022/04/05 01:14:00 by sarchoi          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	is_n_option;

	is_n_option = FT_FALSE;
	if (args[0] && ft_strcmp(args[0], "-n") == 0)
	{
		is_n_option = FT_TRUE;
		args++;
	}
	while (*args)
	{
		ft_putstr_fd(*args, 1);
		if (*(args + 1))
			ft_putstr_fd(" ", 1);
		args++;
	}
	if (!is_n_option)
		ft_putstr_fd("\n", 1);
	return (FT_SUCCESS);
}
