/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:33:55 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/05 20:04:47 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	num_commands(t_token *tokens)
{
	int	count;

	count = 1;
	while(tokens)
	{
		if (tokens->type == PIPE && tokens->next)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_command	*init_commands(int count)
{
	t_command	*commands;
	int			i;

	i = 0;
	commands = malloc(sizeof(t_command) * count);
	while (i < count)
	{
		commands[i].fd_in = STDIN_FILENO;
		commands[i].fd_out = STDOUT_FILENO;
		commands[i].separator = NULL;
		i++;
	}
	return commands;
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token -> type != PIPE)
	{
		if (token->type == ARG)
			count++;
		token = token->next;
	}
	return (count);
}

t_token	*next_command(t_token *token)
{
	while (token && token -> type != PIPE)
		token = token -> next;
	if (token)
		return token->next;
	return NULL;
}

t_token	*next_arg(t_token *token)
{
	if (token)
		token = token->next;
	while (token && token -> type != ARG)
		token = token -> next;
	return (token);
}

void	make_single_command(t_command *command, t_token *tokens)
{
	int	argc;
	int	i;

	argc = count_args(tokens);
	command->args = malloc((argc+1) * sizeof(char *));
	i = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == ARG)
		{
			command->args[i] = ft_strdup(tokens->value);
			i++;
		}
		else if (tokens->type == DOCUMENT)
			command->separator = ft_strdup(tokens->value);
		else if (tokens->type == IN && !command->separator)
			command->fd_in = open(tokens->value, O_RDONLY);
		else if (tokens->type == OUT)
			command->fd_out = open(tokens->value, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		else  if (tokens->type == APPEND)
			command->fd_out = open(tokens->value, O_WRONLY|O_CREAT|O_APPEND, 0666);
		tokens = tokens->next;
	}
	command->args[argc] = NULL;
}

int	check_okay(t_commands	commands)
{
	int	i;

	i = 0;
	while (i < commands.size)
	{
		if (commands.arr[i].args[0] == NULL)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

t_commands	to_command_array(t_token *tokens)
{
	t_command	*command_arr;
	int			count;
	int			i;
	t_commands	commands;

	count = num_commands(tokens);
	command_arr = init_commands(count);
	i = 0;
	while(i < count)
	{
		make_single_command(command_arr + i, tokens);
		tokens = next_command(tokens);
		i++;
	}
	commands = (t_commands){command_arr, count};
	if (!check_okay(commands))
	{
		free_commands(commands);
		return ((t_commands){NULL, -1});
	}
	return (commands);
}

void	free_command(t_command *command)
{
	free(command->separator);
	ft_split_free(command->args);
}

void	free_commands(t_commands commands)
{
	int	i;

	i = 0;
	while (i < commands.size)
	{
		free_command(commands.arr + i);
		i++;
	}
	free(commands.arr);
}
