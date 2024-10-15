/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:54:10 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/15 18:34:43 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	is_n(char *s)
{
	if(s[0] != '-')
		return (0);
	while(*(++s))
		if(*s != 'n')
			return (0);
	return (1);
}

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
	while(command->args[i] && is_n(command->args[i]))
	{
		i++;
		nl = 0;
	}
	while (command->args[i])
	{
		if (last)
			ft_putstr_fd(" ", out);
		last = 1;
		ft_putstr_fd(command->args[i], out);
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
			continue ;
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
