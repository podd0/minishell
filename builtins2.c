/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:33:46 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 20:37:03 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_pwd(t_command *command, t_mini *mini)
{
	ft_putendl_fd(mini->pwd->arr, command->fd_out);
}

void	set_oldpwd(char *old_pwd, t_mini *mini)
{
	char	*arr[2];

	arr[0] = ft_strjoin("OLDPWD=", old_pwd);
	arr[1] = NULL;
	export_many_params(arr, mini);
	free(arr[0]);
	free(old_pwd);
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
