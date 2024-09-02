/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vu_2.c                                             :+:      :+:    :+:   */
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

unsigned int	vu_back(t_vu *vec)
{
	return (vec->arr[vec->size - 1]);
}

void	vu_map_sub(t_vu *vec, unsigned int (*f)(unsigned int))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		vec->arr[i] = f(vec->arr[i]);
		i++;
	}
}
