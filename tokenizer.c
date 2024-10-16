/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:49:23 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/15 19:44:43 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*token_init(char **line)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = get_type(line);
	token->value = NULL;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

char	*match_subtoken(char **line, t_mini *mini, t_token *token)
{
	char	*subtoken;

	if (**line == '\'')
	{
		(*line)++;
		subtoken = match_until(line, "'", 1);
	}
	else if (**line == '"')
	{
		(*line)++;
		subtoken = match_until(line, "\"", 1);
		if (token->type != DOCUMENT)
			subtoken = subst_env(subtoken, mini->env->arr, mini);
	}
	else
	{
		subtoken = match_until(line, "\"'\t <>|", 0);
		if (token->type != DOCUMENT)
			subtoken = subst_env(subtoken, mini->env->arr, mini);
	}
	return (subtoken);
}

void	match_and_sub(t_token *token, char **line, t_mini *mini)
{
	char	*subtoken;
	t_vch	*buf;

	buf = vch_uninit(0);
	while (**line && !ft_strchr(" \t<>|", **line))
	{
		subtoken = match_subtoken(line, mini, token);
		vch_cat(buf, subtoken);
		free(subtoken);
	}
	vch_push_back(buf, '\0');
	token->value = buf->arr;
	free(buf);
}

t_token	*match_token(char **line, t_mini *mini)
{
	t_token	*token;

	token = token_init(line);
	if (token->type == PIPE)
		return (token);
	match_and_sub(token, line, mini);
	return (token);
}

void	tokenize(char *line, t_mini *mini)
{
	t_token	*tail;
	t_token	*curr;

	mini->tokens = NULL;
	tail = NULL;
	line = skip_whitespace(line);
	while (*line)
	{
		curr = match_token(&line, mini);
		if (!curr)
			break ;
		if(curr->type == ARG && ft_strlen(curr->value) == 0)
			free_tokens(curr);
		else
		{
			curr->next = NULL;
			if (!mini->tokens)
				mini->tokens = curr;
			curr->prev = tail;
			if (tail)
				tail->next = curr;
			tail = curr;
		}
		line = skip_whitespace(line);
	}
}
