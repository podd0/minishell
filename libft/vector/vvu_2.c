/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vvu_2.c                                            :+:      :+:    :+:   */
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

t_vu	*vvu_back(t_vvu *vec)
{
	return (vec->arr[vec->size - 1]);
}

void	vvu_map_sub(t_vvu *vec, t_vu *(*f)(t_vu *))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		vec->arr[i] = f(vec->arr[i]);
		i++;
	}
}
