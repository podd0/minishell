/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:54:10 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/12 18:26:24 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	echo(t_command *command, t_mini *mini)
{
	int out;
	int i;
	int nl;
	int last;

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
	int len;

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
			return;
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
	while (i < mini->env->size-1)
	{
		ft_putendl_fd(mini->env->arr[i], command->fd_out);
		i++;
	}
}
