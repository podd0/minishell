/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vu_1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 02:39:21 by apuddu            #+#    #+#             */
/*   Updated: 2024/08/17 02:39:21 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vector.h"
#include <stdlib.h>
#include <unistd.h>

t_vu	*vu_init(int n, unsigned int value)
{
	t_vu	*res;

	res = malloc(sizeof(t_vu));
	*res = (t_vu){0, 0, 0};
	vu_assign(res, n, value);
	return (res);
}

t_vu	*vu_uninit(int n)
{
	t_vu	*res;

	res = malloc(sizeof(t_vu));
	*res = (t_vu){0, 0, 0};
	vu_resize(res, n);
	return (res);
}

void	vu_free(t_vu *vec)
{
	free(vec->arr);
	free(vec);
}

t_vu	*vu_copy(t_vu *vec)
{
	t_vu	*res;

	res = safe_alloc(sizeof(t_vu));
	res->size = vec->size;
	res->buf_size = vec->buf_size;
	res->arr = safe_alloc(sizeof(unsigned int) * vec->buf_size);
	ft_memmove(res->arr, vec->arr, sizeof(unsigned int) * vec->size);
	return (res);
}

void	vu_map(t_vu *vec, void (*f)(unsigned int))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		f(vec->arr[i]);
		i++;
	}
}
