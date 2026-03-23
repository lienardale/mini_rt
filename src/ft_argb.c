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

void ft_pix(int x, int y, t_window *w, t_argb color)
{
	int i;
	int j;

	i = 0;
	while (i < w->resol)
	{
		j = 0;
		while (j < w->resol)
		{
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 0] = color.b;
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 1] = color.g;
			w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + 2] = color.r;
			j++;
		}
		i++;
	}
}

t_argb ft_multi_argb(t_argb a, t_argb col)
{
	t_argb c;

	c.r = (a.r * col.r);
	c.g = (a.g * col.g);
	c.b = (a.b * col.b);
	return (c);
}

t_argb ft_albedo(t_pt a, t_argb col)
{
	t_argb c;

	c.r = (a.x * col.r / 255);
	c.g = (a.y * col.g / 255);
	c.b = (a.z * col.b / 255);
	return (c);
}

void ft_db_mult_to_add_pt(t_pt *a, double b, t_argb c)
{
	a->x += b * (c.r);
	a->y += b * (c.g);
	a->z += b * (c.b);
}
