/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sarchoi <sarchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 02:33:43 by sarchoi           #+#    #+#             */
/*   Updated: 2022/04/03 02:33:49 by sarchoi          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	name_len(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == '=')
			break;
	}
	return (i);
}

static t_var	*var_new(char *var, int scope)
{
	t_var	*new;

	new = (t_var *)malloc(sizeof(t_var));
	if (!new)
		return ((t_var *)NULL);
	new->var = var;
	new->scope = scope;
	new->next = (t_var *)NULL;
	return (new);
}

static t_var	*var_find(t_var *list, char *name)
{
	while (list)
	{
		if (ft_strncmp(list->var, name, name_len(list->var)) == 0)
			return (list);
		list = list->next;
	}
	return ((t_var *)NULL);
}

void	add_var(t_var *list, char *name_and_value, int scope)
{
	t_var	*new;

	new = var_new(ft_strdup(name_and_value), scope);
	if (!list->var || !list)
	{
		list = new;
		return ;
	}
	while (list->next)
		list = list->next;
	list->next = new;
}

/*
** get var valye by name
*/
char	*find_var_value(t_var *list, char *name)
{
	t_var	*tmp;

	tmp = var_find(list, name);
	if (!tmp)
		return ((char *)NULL);
	return (tmp->var + name_len(tmp->var) + 1);
}

void	update_var(t_var *list, char *name, char *new_value)
{
	t_var	*tmp;
	char	*new;

	tmp = var_find(list, name);
	if (!tmp)
		return ;
	new = ft_strjoin(name, "=");
	new = ft_strjoin(new, new_value);
	free(tmp->var);
	tmp->var = new;
}

void	remove_var(t_var *list, char *name)
{
	t_var	*tmp;
	t_var	*before_tmp;

	tmp = list;
	while (tmp)
	{
		if (!ft_strncmp(name, tmp->var, name_len(tmp->var)))
			break ;
		before_tmp = tmp;
		tmp = tmp->next;
	}
	before_tmp->next = tmp->next;
	free(tmp->var);
	free(tmp);
}
