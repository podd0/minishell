/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:37:42 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 20:40:29 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_commands	checks(t_commands commands, t_mini *mini, int count, int status)
{
	if (count < 0 || !check_okay(commands))
	{
		free_commands(commands);
		mini->status_last = status;
		return ((t_commands){NULL, -1});
	}
	return (commands);
}

t_commands	to_command_array(t_token *tokens, t_mini *mini)
{
	t_command	*command_arr;
	int			count;
	int			i;
	t_commands	commands;
	int			ret;

	count = num_commands(tokens);
	command_arr = init_commands(count);
	i = 0;
	commands = (t_commands){command_arr, count};
	while (i < count)
	{
		ret = make_single_command(command_arr + i, tokens);
		if (ret == 2)
			clean_exit(mini, commands, 1);
		else if (ret != 0)
			return (checks(commands, mini, -1, ret));
		tokens = next_command(tokens);
		i++;
	}
	return (checks(commands, mini, count, 1));
}
