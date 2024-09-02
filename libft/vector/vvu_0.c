/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vvu_0.c                                            :+:      :+:    :+:   */
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

void	vvu_push_back(t_vvu *vec, t_vu *elem)
{
	int	new_buf_size;
	int	c_size;

	if (vec->size >= vec->buf_size)
	{
		c_size = vec->size;
		new_buf_size = vec->buf_size;
		if (new_buf_size < 16)
			new_buf_size = 16;
		vvu_resize(vec, new_buf_size * 2);
		vec->size = c_size;
	}
	vec->arr[vec->size] = elem;
	vec->size++;
}

t_vu	*vvu_pop_back(t_vvu *vec)
{
	if (vec->size == 0)
	{
		write(2, "Error: pop empty vector\n", 24);
		exit(1);
	}
	vec->size--;
	return (vec->arr[vec->size]);
}

void	vvu_assign(t_vvu *vec, int n, t_vu *value)
{
	int	i;

	i = 0;
	vec->size = n;
	vec->buf_size = n;
	free(vec->arr);
	vec->arr = safe_alloc(n * sizeof(t_vu *));
	while (i < n)
	{
		vec->arr[i] = value;
		i++;
	}
}

void	vvu_resize(t_vvu *vec, int n)
{
	t_vvu	old;

	old = *vec;
	vec->size = n;
	vec->buf_size = n;
	vec->arr = safe_alloc(n * sizeof(t_vu *));
	ft_memcpy(vec->arr, old.arr, old.size * sizeof(t_vu *));
	free(old.arr);
}
