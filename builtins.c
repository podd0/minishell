/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:54:10 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/24 20:20:18 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	echo(t_command *command, t_mini *mini)
{
	int	out;
	int	i;
	int	nl;
	int	last;

	last = 0;
	nl = 1;
	out = command->fd_out;
	i = 1;
	while (command->args[i])
	{
		if (ft_strncmp("-n", command->args[i], 3) == 0)
			nl = 0;
		else
		{
			if (last)
				ft_putstr_fd(" ", out);
			last = 1;
			ft_putstr_fd(command->args[i], out);
		}
		i++;
	}
	if (nl)
		ft_putstr_fd("\n", out);
	mini->status_last = 0;
}

int	check_good_number(char *str)
{
	int	len;

	len = 0;
	while (' ' == *str)
		str++;
	if (*str == '-')
		str++;
	while (ft_isdigit(*str))
	{
		len++;
		str++;
	}
	while (' ' == *str)
		str++;
	return (len < 11 && *str == '\0');
}

void	mini_exit(t_command *command, t_mini *mini)
{
	mini->status_last = 0;
	if (command->args[1])
	{
		if (!check_good_number(command->args[1]))
		{
			ft_putendl_fd("exit : numeric argument required", 2);
			mini->status_last = 2;
		}
		else if (command->args[2])
		{
			ft_putendl_fd("exit : too many arguments", 2);
			mini->status_last = 1;
			return ;
		}
		else
		{
			ft_putendl_fd("exit", 2);
			mini->status_last = ft_atoi(command->args[1]) % 256;
		}
	}
	clean_exit(mini, mini->commands, mini->status_last);
}

void	env(t_command *command, t_mini *mini)
{
	int	i;

	i = 0;
	while (i < mini->env->size - 1)
	{
		ft_putendl_fd(mini->env->arr[i], command->fd_out);
		i++;
	}
}

int		find_var_index(char *name, t_vstr *env)
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

void	unset(t_command *command, t_mini *mini)
{
	char	**var;
	int		i;

	var = command->args + 1;
	while (*var)
	{
		i = find_var_index(*var, mini->env);
		var++;
		if (i == -1)
			continue;
		if (ft_strncmp(var[-1], "PATH", 5) == 0)
		{
			ft_split_free(mini->path);
			mini->path = calloc(sizeof(char **), 1);
		}
		free(mini->env->arr[i]);
		while (i < mini->env->size - 1)
		{
			mini->env->arr[i] = mini->env->arr[i + 1];
			i++;
		}
		vstr_pop_back(mini->env);
	}
}

// if missing, add '=' at the end of var and dup it
char	*add_equals(char *var)
{
	char	*it;
	it = var;
	while(*it && *it != '=')
		it++;
	if (*it == '=')
		return ft_strdup(var);
	return ft_strjoin(var, "=");
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
	res = malloc(3*sizeof(char *));
	res[0] = ft_substr(key_val, 0, len);
	res[1] = ft_substr(key_val, len+1, ft_strlen(key_val + len + 1));
	res[2] = NULL;
	return (res);
}

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
			continue;
		replace_or_add_variable(splitted, mini, var[-1]);
		ft_split_free(splitted);
	}
}

void	export(t_command *command, t_mini *mini)
{
	if (command->args[1])
	{
		export_many_params(command, mini);
		return ;
	}
	merge_sort(mini->env);
	env(command, mini);
}

void	norm_path(t_vch *res, char *path)
{
	int	ndot;

	ndot = 0;
	res->size = 0;
	while(*path)
	{
		if(*path == '/' && ndot <= 2)
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
	if(len > 2 && mini->pwd->arr[len - 2] == '/')
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

	if(!command->args[1])
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
