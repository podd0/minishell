/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:12:49 by epiacent          #+#    #+#             */
/*   Updated: 2024/10/08 17:41:09 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	num_commands(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (tokens->next == NULL)
			{
				ft_putendl_fd("Error: commands can't end with a pipe", 2);
				return (-1);
			}
			count++;
		}
		tokens = tokens->next;
	}
	return (count);
}

t_command	*init_commands(int count)
{
	t_command	*commands;
	int			i;

	if (count < 0)
		return (NULL);
	i = 0;
	commands = malloc(sizeof(t_command) * count);
	while (i < count)
	{
		commands[i].fd_in = STDIN_FILENO;
		commands[i].fd_out = STDOUT_FILENO;
		commands[i].has_document = 0;
		commands[i].args = NULL;
		commands[i].pipe_in = NULL;
		commands[i].pipe_out = NULL;
		commands[i].pid = 0;
		i++;
	}
	return (commands);
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == ARG)
			count++;
		token = token->next;
	}
	return (count);
}

t_token	*next_command(t_token *token)
{
	while (token && token->type != PIPE)
		token = token->next;
	if (token)
	{
		return (token->next);
	}
	return (NULL);
}

t_token	*next_arg(t_token *token)
{
	if (token)
		token = token->next;
	while (token && token->type != ARG)
		token = token->next;
	return (token);
}
