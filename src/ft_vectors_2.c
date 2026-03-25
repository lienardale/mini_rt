/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vectors_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 12:04:58 by alienard          #+#    #+#             */
/*   Updated: 2020/02/28 12:33:03 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#ifdef __SSE2__
#include <emmintrin.h>
#endif

/* Compute the cross product a x b, yielding a vector perpendicular to both */
t_pt ft_cross_product(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.y * b.z) - (a.z * b.y);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);
	return (c);
}

/* Component-wise addition of two vectors: c = a + b */
t_pt ft_addition(t_pt a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d va;
	__m128d vb;

	va = _mm_loadu_pd(&a.x);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_add_pd(va, vb));
#else
	c.x = (a.x + b.x);
	c.y = (a.y + b.y);
#endif
	c.z = (a.z + b.z);
	return (c);
}

/* Component-wise subtraction of two vectors: c = a - b */
t_pt ft_subtraction(t_pt a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d va;
	__m128d vb;

	va = _mm_loadu_pd(&a.x);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_sub_pd(va, vb));
#else
	c.x = (a.x - b.x);
	c.y = (a.y - b.y);
#endif
	c.z = (a.z - b.z);
	return (c);
}

/* Component-wise (Hadamard) multiplication of two vectors: c = a * b */
t_pt ft_multi_vect(t_pt a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d va;
	__m128d vb;

	va = _mm_loadu_pd(&a.x);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_mul_pd(va, vb));
#else
	c.x = (a.x * b.x);
	c.y = (a.y * b.y);
#endif
	c.z = (a.z * b.z);
	return (c);
}

/* Return the negation of a vector: (-x, -y, -z) */
t_pt ft_neg_pt(t_pt dir)
{
	t_pt tmp;

#ifdef __SSE2__
	__m128d v;
	__m128d neg;

	neg = _mm_set1_pd(-1.0);
	v = _mm_loadu_pd(&dir.x);
	_mm_storeu_pd(&tmp.x, _mm_mul_pd(v, neg));
#else
	tmp.x = -dir.x;
	tmp.y = -dir.y;
#endif
	tmp.z = -dir.z;
	return (tmp);
}
