/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scalar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 12:04:12 by alienard          #+#    #+#             */
/*   Updated: 2020/02/19 14:13:59 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#ifdef __SSE2__
#include <emmintrin.h>
#endif

t_pt ft_multi_scal(double a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d vs;
	__m128d vb;

	vs = _mm_set1_pd(a);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_mul_pd(vs, vb));
#else
	c.x = (a * b.x);
	c.y = (a * b.y);
#endif
	c.z = (a * b.z);
	return (c);
}

t_pt ft_div_scal(double a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d vs;
	__m128d vb;

	vs = _mm_set1_pd(a);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_div_pd(vb, vs));
#else
	c.x = (b.x / a);
	c.y = (b.y / a);
#endif
	c.z = (b.z / a);
	return (c);
}

t_pt ft_add_scal(double a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d vs;
	__m128d vb;

	vs = _mm_set1_pd(a);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_add_pd(vb, vs));
#else
	c.x = b.x + a;
	c.y = b.y + a;
#endif
	c.z = b.z + a;
	return (c);
}

t_pt ft_sub_scal(double a, t_pt b)
{
	t_pt c;

#ifdef __SSE2__
	__m128d vs;
	__m128d vb;

	vs = _mm_set1_pd(a);
	vb = _mm_loadu_pd(&b.x);
	_mm_storeu_pd(&c.x, _mm_sub_pd(vb, vs));
#else
	c.x = b.x - a;
	c.y = b.y - a;
#endif
	c.z = b.z - a;
	return (c);
}
