/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:54:10 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/20 20:08:51 by apuddu           ###   ########.fr       */
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

void	export_many_params(t_command *command, t_mini *mini)
{
	char	**var;
	char	**splitted;
	int		i;

	var = command->args + 1;
	while (*var)
	{
		splitted = split_var(*var);
		var++;
		if (!splitted)
			continue;
		i = find_var_index(splitted[0], mini->env);
		if (i == -1)
		{
			*vstr_back_ptr(mini->env) = ft_strdup(var[-1]);
			vstr_push_back(mini->env, NULL);
		}
		else
		{
			free(mini->env->arr[i]);
			mini->env->arr[i] = ft_strdup(var[-1]);
		}
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

int	cd_core(char *path)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

void	cd(t_command *command, t_mini *mini)
{
	char	*home_path;

	if(!command->args[1])
	{
		home_path = subst_env(ft_strdup("$HOME"), mini->env->arr, mini);
		mini->status_last = cd_core(home_path);
		free(home_path);
	}
	else
	{
		mini->status_last = cd_core(command->args[1]);
	}
}
