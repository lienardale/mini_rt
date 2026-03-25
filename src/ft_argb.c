/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 19:46:46 by alienard          #+#    #+#             */
/*   Updated: 2020/07/10 16:25:45 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* ACES filmic tone mapping: maps HDR [0,inf) to [0,1] with filmic curve */
static double ft_aces_tonemap(double x)
{
	double a;
	double b;

	a = x * (2.51 * x + 0.03);
	b = x * (2.43 * x + 0.59) + 0.14;
	if (b < EPSILON_ZERO)
		return (0.0);
	x = a / b;
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}

/* Apply ACES tone mapping and sRGB gamma to a color channel */
static double ft_gamma_correct(double c)
{
	double linear;

	if (c <= 0.0)
		return (0.0);
	linear = c / 255.0;
	linear = ft_aces_tonemap(linear);
	return (255.0 * pow(linear, 1.0 / 2.2));
}

/* Write a gamma-corrected pixel (scaled by resolution factor) into the image
 * buffer */
void ft_pix(int x, int y, t_window *w, t_argb color)
{
	int i;
	int j;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	r = (unsigned char)ft_gamma_correct(color.r);
	g = (unsigned char)ft_gamma_correct(color.g);
	b = (unsigned char)ft_gamma_correct(color.b);
	i = 0;
	while (i < w->resol)
	{
		j = 0;
		while (j < w->resol)
		{
			w->data[((int)(w->y - (unsigned int)y - (unsigned int)j - 1) *
						 (int)w->x +
					 x + i) *
						4 +
					0] = (char)b;
			w->data[((int)(w->y - (unsigned int)y - (unsigned int)j - 1) *
						 (int)w->x +
					 x + i) *
						4 +
					1] = (char)g;
			w->data[((int)(w->y - (unsigned int)y - (unsigned int)j - 1) *
						 (int)w->x +
					 x + i) *
						4 +
					2] = (char)r;
			j++;
		}
		i++;
	}
}

/* Component-wise multiplication of two ARGB colors */
t_argb ft_multi_argb(t_argb a, t_argb col)
{
	t_argb c;

	c.r = (a.r * col.r);
	c.g = (a.g * col.g);
	c.b = (a.b * col.b);
	return (c);
}

/* Scale a color by an albedo vector (per-channel intensity), normalized to
 * [0,255] */
t_argb ft_albedo(t_pt a, t_argb col)
{
	t_argb c;

	c.r = (a.x * col.r / 255);
	c.g = (a.y * col.g / 255);
	c.b = (a.z * col.b / 255);
	return (c);
}

/* Accumulate scaled color into a point: a += b * c (for light contribution) */
void ft_db_mult_to_add_pt(t_pt *a, double b, t_argb c)
{
	a->x += b * (c.r);
	a->y += b * (c.g);
	a->z += b * (c.b);
}
