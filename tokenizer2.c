/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epiacent <epiacent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:05:41 by epiacent          #+#    #+#             */
/*   Updated: 2024/10/02 15:10:01 by epiacent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*skip_whitespace(char *line)
{
	while (*line == ' ' || *line == '\t')
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
	return (res);
}

int	check_out(char **line)
{
	char	*s;

	s = *line;
	if (s[0] == '>')
	{
		if (s[1] == '>')
		{
			*line = skip_whitespace(*line + 2);
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
		if (s[1] == '<')
		{
			*line = skip_whitespace(*line + 2);
			return (DOCUMENT);
		}
		*line = skip_whitespace(*line + 1);
		return (IN);
	}
	if (s[0] == '|')
	{
		*line = skip_whitespace(*line + 1);
		return (PIPE);
	}
	return (check_out(line));
}

void	free_tokens(t_token *token)
{
	if (!token)
		return ;
	free_tokens(token->next);
	free(token->value);
	free(token);
}
