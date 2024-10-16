/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:06:02 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 18:25:39 by apuddu           ###   ########.fr       */
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

void	export_many_params(char **var, t_mini *mini)
{
	char	**splitted;

	while (*var)
	{
		splitted = split_var(*var);
		if (!splitted)
		{
			ft_putstr_fd("export: '", 2);
			ft_putstr_fd(*var, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			mini->status_last = 1;
			var++;
			continue ;
		}
		replace_or_add_variable(splitted, mini, *var);
		ft_split_free(splitted);
		var++;
	}
}

void	export(t_command *command, t_mini *mini)
{
	t_vstr	*copy;
	int		i;
	int		eqidx;
	
	mini->status_last = 0;
	if (command->args[1])
	{
		export_many_params(command->args + 1, mini);
		return ;
	}
	copy = vstr_copy(mini->env);
	merge_sort(copy);
	i = 0;
	while (i < copy->size - 1)
	{
		ft_putstr_fd("declare -x\t", command->fd_out);
		eqidx = ft_strchr(copy->arr[i], '=') - copy->arr[i];
		write(command->fd_out, copy->arr[i], eqidx + 1);
		ft_putchar_fd('"', command->fd_out);
		ft_putstr_fd(copy->arr[i] + eqidx + 1, command->fd_out);
		ft_putendl_fd("\"", command->fd_out);
		i++;
	}
	vstr_free(copy);
}
