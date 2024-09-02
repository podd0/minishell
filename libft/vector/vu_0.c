/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vu_0.c                                             :+:      :+:    :+:   */
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

void	vu_push_back(t_vu *vec, unsigned int elem)
{
	int	new_buf_size;
	int	c_size;

	if (vec->size >= vec->buf_size)
	{
		c_size = vec->size;
		new_buf_size = vec->buf_size;
		if (new_buf_size < 16)
			new_buf_size = 16;
		vu_resize(vec, new_buf_size * 2);
		vec->size = c_size;
	}
	vec->arr[vec->size] = elem;
	vec->size++;
}

unsigned int	vu_pop_back(t_vu *vec)
{
	if (vec->size == 0)
	{
		write(2, "Error: pop empty vector\n", 24);
		exit(1);
	}
	vec->size--;
	return (vec->arr[vec->size]);
}

void	vu_assign(t_vu *vec, int n, unsigned int value)
{
	int	i;

	i = 0;
	vec->size = n;
	vec->buf_size = n;
	free(vec->arr);
	vec->arr = safe_alloc(n * sizeof(unsigned int));
	while (i < n)
	{
		vec->arr[i] = value;
		i++;
	}
}

void	vu_resize(t_vu *vec, int n)
{
	t_vu	old;

	old = *vec;
	vec->size = n;
	vec->buf_size = n;
	vec->arr = safe_alloc(n * sizeof(unsigned int));
	ft_memcpy(vec->arr, old.arr, old.size * sizeof(unsigned int));
	free(old.arr);
}
