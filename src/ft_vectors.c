/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vectors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 18:49:45 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 18:11:09 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#ifdef __SSE2__
#include <emmintrin.h>
#endif

double ft_lenght(t_pt a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

void ft_inv_norm(t_pt *dir)
{
#ifdef __SSE2__
	__m128d v;
	__m128d neg;

	neg = _mm_set1_pd(-1.0);
	v = _mm_loadu_pd(&dir->x);
	v = _mm_mul_pd(v, neg);
	_mm_storeu_pd(&dir->x, v);
#else
	dir->x = -dir->x;
	dir->y = -dir->y;
#endif
	dir->z = -dir->z;
}

t_pt ft_normal_vect(t_pt a)
{
	double inv;

	inv = 1.0 / ft_lenght(a);
	return (ft_multi_scal(inv, a));
}

double ft_dot_product(t_pt a, t_pt b)
{
#ifdef __SSE2__
	__m128d va;
	__m128d vb;
	__m128d vm;
	double tmp[2];

	va = _mm_loadu_pd(&a.x);
	vb = _mm_loadu_pd(&b.x);
	vm = _mm_mul_pd(va, vb);
	_mm_storeu_pd(tmp, vm);
	return (tmp[0] + tmp[1] + a.z * b.z);
#else
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
#endif
}

t_pt ft_div_vect(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.x / b.x);
	c.y = (a.y / b.y);
	c.z = (a.z / b.z);
	return (c);
}
