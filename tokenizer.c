/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:49:23 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/06 17:40:18 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*skip_whitespace(char *line)
{
	while (*line == ' ')
		line++;
	return (line);
}

char	*match_until(char **line, char *charset, int skip)
{
	t_vch	*buf;
	char	*res;

	buf = vch_uninit(0);
	while (**line && !ft_strchr(charset, **line))
	{
		vch_push_back(buf, **line);
		(*line)++;
	}
	if (**line)
		(*line) += skip;
	vch_push_back(buf, '\0');
	res = buf->arr;
	free(buf);
	return res;
}

int	check_out(char **line)
{
	char	*s;

	s = *line;
	if (s[0] == '>')
	{
		if(s[1] == '>')
		{
			*line = skip_whitespace(*line+2);
			return (APPEND);
		}
		*line = skip_whitespace(*line + 1);
		return (OUT);
	}
	return (ARG);

}
int	get_type(char **line)
{
	char	*s;

	s = *line;
	if (ft_strchr("'\"", **line))
		return (ARG);
	if (s[0] == '<')
	{
		if(s[1] == '<')
		{
			*line = skip_whitespace(*line+2);
			return (DOCUMENT);
		}
		*line = skip_whitespace(*line + 1);
		return (IN);
	}
	if(s[0] == '|')
	{
		*line = skip_whitespace(*line + 1);
		return (PIPE);
	}
	return (check_out(line));
}
t_token	*token_init(char **line)
{
	t_token *token;
	
	token = malloc(sizeof(t_token));
	token->type = get_type(line);
	token->value = NULL;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	match_and_sub(t_token *token, char **line, t_mini *mini)
{
	if (**line == '\'')
	{
		(*line)++;
		token->value = match_until(line, "'", 1);
	}
	else if (**line == '"')
	{
		(*line)++;
		token->value = match_until(line, "\"", 1);
		if (token->type != DOCUMENT)
			token->value = subst_env(token->value, mini->env->arr, mini);
	}
	else
	{
		token->value = match_until(line, "\"' <>|", 0);
		if (token->type != DOCUMENT)
			token->value = subst_env(token->value, mini->env->arr, mini);
	}
}

t_token	*match_token(char **line, t_mini *mini)
{
	t_token *token;

	token = token_init(line);
	if (token->type == PIPE)
		return (token);
	if(**line == '\0')
		return (NULL);
	match_and_sub(token, line, mini);
	return (token);
}

t_token	*tokenize(char *line, t_mini *mini)
{
	t_token	*head;
	t_token	*tail;
	t_token	*curr;

	head = NULL;
	tail = NULL;
	line = skip_whitespace(line);
	while(*line)
	{
		curr = match_token(&line, mini);
		curr->next = NULL;
		if (!curr)
			break;
		if (!head)
			head = curr;
		curr->prev = tail;
		if(tail)
		{
			tail->next = curr;
		}
		tail = curr;
		line = skip_whitespace(line);
	}
	return (head);
}

void	free_tokens(t_token *token)
{
	if(!token)
		return;
	free_tokens(token->next);
	free(token->value);
	free(token);
}