/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:09:07 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 15:09:39 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// if missing, add '=' at the end of var and dup it
char	*add_equals(char *var)
{
	char	*it;

	it = var;
	while (*it && *it != '=')
		it++;
	if (*it == '=')
		return (ft_strdup(var));
	return (ft_strjoin(var, "="));
}

char	**split_var(char *key_val)
{
	char	**res;
	int		len;

	len = 0;
	if (!ft_isalpha(key_val[0]) && key_val[0] != '_')
		return (NULL);
	while (is_variable_name(key_val[len]))
	{
		len++;
	}
	if (key_val[len] != '=')
	{
		return (NULL);
	}
	res = malloc(3 * sizeof(char *));
	res[0] = ft_substr(key_val, 0, len);
	res[1] = ft_substr(key_val, len + 1, ft_strlen(key_val + len + 1));
	res[2] = NULL;
	return (res);
}

int	find_var_index(char *name, t_vstr *env)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env->arr[i])
	{
		if (ft_strncmp(name, env->arr[i], len) == 0 && env->arr[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
