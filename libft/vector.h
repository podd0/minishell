/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 02:39:20 by apuddu            #+#    #+#             */
/*   Updated: 2024/08/17 02:39:20 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H 1

struct					s_vi
{
	int					*arr;
	int					size;
	int					buf_size;
};

typedef struct s_vi		t_vi;

void					vi_push_back(t_vi *vec, int elem);
int						vi_pop_back(t_vi *vec);
void					vi_assign(t_vi *vec, int n, int value);
void					vi_resize(t_vi *vec, int n);
t_vi					*vi_init(int n, int value);
t_vi					*vi_uninit(int n);
void					vi_free(t_vi *vec);
t_vi					*vi_copy(t_vi *vec);
void					vi_map(t_vi *vec, void (*f)(int));
void					vi_map_sub(t_vi *vec, int (*f)(int));
int						vi_back(t_vi *vec);

struct					s_vl
{
	long				*arr;
	int					size;
	int					buf_size;
};

typedef struct s_vl		t_vl;

void					vl_push_back(t_vl *vec, long elem);
long					vl_pop_back(t_vl *vec);
void					vl_assign(t_vl *vec, int n, long value);
void					vl_resize(t_vl *vec, int n);
t_vl					*vl_init(int n, long value);
t_vl					*vl_uninit(int n);
void					vl_free(t_vl *vec);
t_vl					*vl_copy(t_vl *vec);
void					vl_map(t_vl *vec, void (*f)(long));
void					vl_map_sub(t_vl *vec, long (*f)(long));
long					vl_back(t_vl *vec);

struct					s_vlu
{
	long unsigned int	*arr;
	int					size;
	int					buf_size;
};

typedef struct s_vlu	t_vlu;

void					vlu_push_back(t_vlu *vec, long unsigned int elem);
long unsigned int		vlu_pop_back(t_vlu *vec);
void					vlu_assign(t_vlu *vec, int n, long unsigned int value);
void					vlu_resize(t_vlu *vec, int n);
t_vlu					*vlu_init(int n, long unsigned int value);
t_vlu					*vlu_uninit(int n);
void					vlu_free(t_vlu *vec);
t_vlu					*vlu_copy(t_vlu *vec);
void					vlu_map(t_vlu *vec, void (*f)(long unsigned int));
void					vlu_map_sub(t_vlu *vec,
							long unsigned int (*f)(long unsigned int));
long unsigned int		vlu_back(t_vlu *vec);

struct					s_vstr
{
	char				**arr;
	int					size;
	int					buf_size;
};

typedef struct s_vstr	t_vstr;

void					vstr_push_back(t_vstr *vec, char *elem);
char					*vstr_pop_back(t_vstr *vec);
void					vstr_assign(t_vstr *vec, int n, char *value);
void					vstr_resize(t_vstr *vec, int n);
t_vstr					*vstr_init(int n, char *value);
t_vstr					*vstr_uninit(int n);
void					vstr_free(t_vstr *vec);
t_vstr					*vstr_copy(t_vstr *vec);
void					vstr_map(t_vstr *vec, void (*f)(char *));
void					vstr_map_sub(t_vstr *vec, char *(*f)(char *));
char					*vstr_back(t_vstr *vec);

struct					s_vu
{
	unsigned int		*arr;
	int					size;
	int					buf_size;
};

typedef struct s_vu		t_vu;

void					vu_push_back(t_vu *vec, unsigned int elem);
unsigned int			vu_pop_back(t_vu *vec);
void					vu_assign(t_vu *vec, int n, unsigned int value);
void					vu_resize(t_vu *vec, int n);
t_vu					*vu_init(int n, unsigned int value);
t_vu					*vu_uninit(int n);
void					vu_free(t_vu *vec);
t_vu					*vu_copy(t_vu *vec);
void					vu_map(t_vu *vec, void (*f)(unsigned int));
void					vu_map_sub(t_vu *vec, unsigned int (*f)(unsigned int));
unsigned int			vu_back(t_vu *vec);

struct					s_vvu
{
	t_vu				**arr;
	int					size;
	int					buf_size;
};

typedef struct s_vvu	t_vvu;

void					vvu_push_back(t_vvu *vec, t_vu *elem);
t_vu					*vvu_pop_back(t_vvu *vec);
void					vvu_assign(t_vvu *vec, int n, t_vu *value);
void					vvu_resize(t_vvu *vec, int n);
t_vvu					*vvu_init(int n, t_vu *value);
t_vvu					*vvu_uninit(int n);
void					vvu_free(t_vvu *vec);
t_vvu					*vvu_copy(t_vvu *vec);
void					vvu_map(t_vvu *vec, void (*f)(t_vu *));
void					vvu_map_sub(t_vvu *vec, t_vu *(*f)(t_vu *));
t_vu					*vvu_back(t_vvu *vec);

#endif
