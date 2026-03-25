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

/* Apply gamma correction (gamma=2.2) to a color channel, clamped to [0, 255] */
static double ft_gamma_correct(double c)
{
	if (c <= 0.0)
		return (0.0);
	if (c >= 255.0)
		return (255.0);
	return (255.0 * pow(c / 255.0, 1.0 / 2.2));
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
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 0] = b;
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 1] = g;
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 2] = r;
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
