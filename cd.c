/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:01:42 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 15:03:21 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	norm_path(t_vch *res, char *path)
{
	int	ndot;

	ndot = 0;
	res->size = 0;
	while (*path)
	{
		if (*path == '/' && ndot <= 2)
		{
			while (res->size > 1 && ndot > 0)
				if (vch_pop_back(res) == '/')
					ndot--;
		}
		if (*path == '.')
			ndot++;
		else
			ndot = 0;
		vch_push_back(res, *path);
		path++;
	}
	while (res->size > 1 && ndot > 0 && ndot <= 2)
		if (vch_pop_back(res) == '/')
			ndot--;
	vch_push_back(res, '\0');
}

char	*path_join(t_vch *pwd, char *path)
{
	t_vch	*new_path;
	char	*total_path;

	if (path[0] != '/')
	{
		new_path = vch_copy(pwd);
		if (new_path->size > 2)
		{
			vch_pop_back(new_path);
			vch_push_back(new_path, '/');
			vch_push_back(new_path, '\0');
		}
		total_path = ft_strjoin(new_path->arr, path);
		norm_path(new_path, total_path);
		free(total_path);
		path = new_path->arr;
		free(new_path);
		return (path);
	}
	return (ft_strdup(path));
}

int	cd_core(t_mini *mini, char *path)
{
	int		len;
	char	*actual_path;
	char	*name_val[2];
	char	*assignment;

	actual_path = path_join(mini->pwd, path);
	if (chdir(actual_path) < 0)
	{
		free(actual_path);
		return (1);
	}
	vch_set_string(mini->pwd, actual_path);
	len = mini->pwd->size;
	if (len > 2 && mini->pwd->arr[len - 2] == '/')
	{
		mini->pwd->arr[len - 2] = '\0';
		vch_pop_back(mini->pwd);
	}
	name_val[0] = "PWD";
	name_val[1] = mini->pwd->arr;
	assignment = join_name_value("PWD", mini->pwd->arr);
	replace_or_add_variable(name_val, mini, assignment);
	free(assignment);
	free(actual_path);
	return (0);
}

void	cd(t_command *command, t_mini *mini)
{
	char	*home_path;

	if (!command->args[1])
	{
		home_path = find_var("HOME", mini->env->arr);
		mini->status_last = cd_core(mini, home_path);
		if (mini->status_last)
			perror("cd");
	}
	else
	{
		mini->status_last = cd_core(mini, command->args[1]);
		if (mini->status_last)
		{
			ft_putstr_fd("cd ", 2);
			perror(command->args[1]);
		}
	}
}

void	print_pwd(t_command *command, t_mini *mini)
{
	ft_putendl_fd(mini->pwd->arr, command->fd_out);
}
