/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 04:03:39 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/20 19:32:45 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	varname_compare(char *a, char *b)
{
	if (!b)
		return (1);
	if (!a)
		return (0);
	
	while(*a != '=' && *b != '=' && *a == *b)
	{
		a++;
		b++;
	}
	if(*a == '=' || *b == '=')
	{
		return (*a == '=');
	}
	return (*a < *b);
}

void	merge(t_vstr *dst, t_vstr *src, int l, int w)
{
	int	i;
	int	r;
	int	mid;
	int	r1;

	mid = l + w;
	r = mid + w;
	i = l;
	r1 = mid;
	if (r > dst->size)
		r = dst->size;
	while (i < r)
	{
		if (r1 >= r || (l < mid && varname_compare(src->arr[l], src->arr[r1])))
		{
			dst->arr[i] = src->arr[l];
			l++;
		}
		else
		{
			dst->arr[i] = src->arr[r1];
			r1++;
		}
		i++;
	}
}

void	swap(t_vstr **a, t_vstr **b)
{
	t_vstr	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	end_sort(t_vstr *vec, t_vstr *orig_vec, t_vstr *temp)
{
	if (orig_vec != vec)
	{
		free(orig_vec->arr);
		*orig_vec = *vec;
		free(vec);
	}
	else
		vstr_free(temp);
}

void	merge_sort(t_vstr *vec)
{
	t_vstr	*temp;
	int		w;
	int		i;
	t_vstr	*orig_vec;

	orig_vec = vec;
	temp = vstr_uninit(vec->size);
	w = 1;
	while (w < vec->size)
	{
		i = 0;
		while (i < vec->size)
		{
			merge(temp, vec, i, w);
			i += w * 2;
		}
		w *= 2;
		swap(&temp, &vec);
	}
	end_sort(vec, orig_vec, temp);
}
