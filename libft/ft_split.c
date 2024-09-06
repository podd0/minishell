/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 20:22:02 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/06 18:07:49 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int	count(const char *s, char c)
{
	int	cnt;

	cnt = 0;
	while (*s)
	{
		while (*s == c)
		{
			s++;
		}
		if (*s)
			cnt++;
		while (*s && *s != c)
			s++;
	}
	return (cnt + 1);
}

char	**populate(char **arr, const char *s, char c)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (*s)
	{
		while (*s == c)
		{
			s++;
		}
		if (!*s)
			break ;
		len = 1;
		while (s[len] && s[len] != c)
		{
			len++;
		}
		arr[i] = ft_substr(s, 0, len);
		if (!arr[i])
			return (NULL);
		i++;
		s = s + len;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	arr = malloc(count(s, c) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (populate(arr, s, c));
}

void	ft_split_free(char **split)
{
	char	**it;

	if (split == NULL)
		return ;
	it = split;
	while (*it)
	{
		free(*it);
		it++;
	}
	free(split);
}
