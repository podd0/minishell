/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:49:23 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/04 20:23:11 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <vector.h>

char	*skip_whitespace(char *line)
{
	while (*line == ' ')
		line++;
	return (line);
}

char	*match_until(char **line, char *charset)
{
	t_vch	*buf;
	char	*res;

	buf = vch_uninit(0);
	while (**line && !ft_strchr(charset, **line))
	{
		vch_push_back(buf, **line);
	}
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

t_token	*match_token(char **line, t_mini *mini)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	token->type = get_type(line);
	if (token->type == PIPE)
		return (token);
	if (**line == '\'')
	{
		(*line)++;
		token->value = match_until(line, "'");
	}
	else if (**line == '"')
	{
		(*line)++;
		token->value = match_until(line, "\"");
		token->value = subst_env(token->value, mini->env);
	}
	else
	{
		token->value = match_until(line, "\"' <>|");
		token->value = subst_env(token->value, mini->env);
	}
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
	while(line)
	{
		curr = match_token(&line, mini);
		if (!head)
			head = curr;
		if(tail)
		{
			tail->next = curr;
			curr->prev = tail;
		}
		tail = curr;
		line = skip_whitespace(line);
	}
	return (head);
}