/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vvu_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 02:39:22 by apuddu            #+#    #+#             */
/*   Updated: 2024/08/17 02:39:22 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vector.h"
#include <stdlib.h>
#include <unistd.h>

t_vvu	*vvu_init(int n, t_vu *value)
{
	t_vvu	*res;

	res = malloc(sizeof(t_vvu));
	*res = (t_vvu){0, 0, 0};
	vvu_assign(res, n, value);
	return (res);
}

t_vvu	*vvu_uninit(int n)
{
	t_vvu	*res;

	res = malloc(sizeof(t_vvu));
	*res = (t_vvu){0, 0, 0};
	vvu_resize(res, n);
	return (res);
}

void	vvu_free(t_vvu *vec)
{
	free(vec->arr);
	free(vec);
}

t_vvu	*vvu_copy(t_vvu *vec)
{
	t_vvu	*res;

	res = safe_alloc(sizeof(t_vvu));
	res->size = vec->size;
	res->buf_size = vec->buf_size;
	res->arr = safe_alloc(sizeof(t_vu *) * vec->buf_size);
	ft_memmove(res->arr, vec->arr, sizeof(t_vu *) * vec->size);
	return (res);
}

void	vvu_map(t_vvu *vec, void (*f)(t_vu *))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		f(vec->arr[i]);
		i++;
	}
}
