/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epiacent <epiacent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:23:18 by epiacent          #+#    #+#             */
/*   Updated: 2024/10/02 15:27:24 by epiacent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_commands(t_commands commands)
{
	int	i;

	i = 0;
	while (i < commands.size)
	{
		ft_split_free(commands.arr[i].args);
		free(commands.arr[i].pipe_in);
		i++;
	}
	free(commands.arr);
}
