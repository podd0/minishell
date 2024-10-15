/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:06:02 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/15 15:48:48 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**vstr_back_ptr(t_vstr *vec)
{
	return (&vec->arr[vec->size - 1]);
}

void	replace_or_add_variable(char **splitted, t_mini *mini, char *var)
{
	int	i;

	i = find_var_index(splitted[0], mini->env);
	if (i == -1)
	{
		*vstr_back_ptr(mini->env) = ft_strdup(var);
		vstr_push_back(mini->env, NULL);
	}
	else
	{
		free(mini->env->arr[i]);
		mini->env->arr[i] = ft_strdup(var);
	}
	if (ft_strncmp(splitted[0], "PATH", 5) == 0)
	{
		ft_split_free(mini->path);
		mini->path = get_path(mini->env->arr);
	}
}

char	*join_name_value(char *name, char *value)
{
	t_vch	*buf;
	char	*res;

	buf = vch_from_string(name);
	vch_pop_back(buf);
	vch_push_back(buf, '=');
	vch_cat(buf, value);
	vch_push_back(buf, '\0');
	res = buf->arr;
	free(buf);
	return (res);
}

void	export_many_params(t_command *command, t_mini *mini)
{
	char	**var;
	char	**splitted;

	var = command->args + 1;
	while (*var)
	{
		splitted = split_var(*var);
		var++;
		if (!splitted)
			continue ;
		replace_or_add_variable(splitted, mini, var[-1]);
		ft_split_free(splitted);
	}
}

void	export(t_command *command, t_mini *mini)
{
	t_vstr	*copy;
	int		i;
	
	if (command->args[1])
	{
		export_many_params(command, mini);
		return ;
	}
	copy = vstr_copy(mini->env);
	merge_sort(copy);
	i = 0;
	while (i < copy->size - 1)
	{
		printf("declare -x\t%s\n", copy->arr[i]);
		i++;
	}
	vstr_free(copy);
}
