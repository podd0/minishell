/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:14:22 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 15:42:07 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_vch	*vch_from_string(char *s)
{
	int		len;
	int		i;
	t_vch	*res;

	i = 0;
	len = ft_strlen(s) + 1;
	res = vch_uninit(len);
	while (i < len)
	{
		res->arr[i] = s[i];
		i++;
	}
	return (res);
}

void	vch_set_string(t_vch *v, char *s)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(s) + 1;
	if (len > v->size)
		vch_resize(v, len);
	while (i < len)
	{
		v->arr[i] = s[i];
		i++;
	}
	v->size = len;
}

void	free_commands(t_commands commands)
{
	int	i;

	i = 0;
	while (i < commands.size)
	{
		ft_split_free(commands.arr[i].args);
		free(commands.arr[i].pipe_in);
		i++;
	}
	free(commands.arr);
}

// no null terminator
void	vch_cat(t_vch *res, char *str)
{
	while (*str)
	{
		vch_push_back(res, *str);
		str++;
	}
}

int	is_variable_name(char c)
{
	return (ft_isalnum(c) || c == '_');
}
