/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:31:38 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 15:40:07 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*find_var(char *name, char **env)
{
	int	name_len;

	name_len = ft_strlen(name);
	while (*env)
	{
		if (ft_strncmp(name, *env, name_len) == 0 && (*env)[name_len] == '=')
		{
			return ((*env) + name_len + 1);
		}
		env++;
	}
	return ("");
}

t_vch	*get_var_name(char *s)
{
	t_vch	*name;

	name = vch_uninit(0);
	if (ft_isalpha(*s) || *s == '_')
	{
		while (is_variable_name(*s))
		{
			vch_push_back(name, *s);
			s++;
		}
	}
	vch_push_back(name, '\0');
	return (name);
}

int	handle_dollar(t_mini *mini, char **env, char *line, t_vch *res)
{
	char	*s;
	t_vch	*name;
	int		len;

	line++;
	if (*line == '?')
	{
		s = ft_itoa(mini->status_last);
		vch_cat(res, s);
		len = 2;
		free(s);
	}
	else
	{
		name = get_var_name(line);
		len = name->size;
		vch_cat(res, find_var(name->arr, env));
		vch_free(name);
	}
	return (len);
}

char	*subst_env(char *line, char **env, t_mini *mini)
{
	t_vch	*res;
	char	*s;
	char	*line_start;

	if (!ft_strchr(line, '$'))
		return (line);
	res = vch_uninit(0);
	line_start = line;
	while (*line)
	{
		if (*line != '$')
		{
			vch_push_back(res, *line);
			line++;
		}
		else
		{
			line += handle_dollar(mini, env, line, res);
		}
	}
	vch_push_back(res, '\0');
	s = res->arr;
	free(res);
	free(line_start);
	return (s);
}

void	set_env(t_mini *mini, char **env)
{
	t_vstr	*copy;

	copy = vstr_uninit(0);
	while (*env)
	{
		vstr_push_back(copy, ft_strdup(*env));
		env++;
	}
	vstr_push_back(copy, NULL);
	mini->env = copy;
}
