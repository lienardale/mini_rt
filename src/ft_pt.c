/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 19:17:44 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 18:53:22 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void ft_swap_pt(t_pt *a, t_pt *b)
{
	t_pt c;

	c = *a;
	*a = *b;
	*b = c;
}

double ft_sqr(double d)
{
	return (d * d);
}

void ft_inv_pt(t_pt *dir)
{
	dir->x = 1 / dir->x;
	dir->y = 1 / dir->y;
	dir->z = 1 / dir->z;
}

void ft_pt_init(t_pt *a, double x, double y, double z)
{
	a->x = x;
	a->y = y;
	a->z = z;
}

t_pt ft_pt_create(double x, double y, double z)
{
	t_pt a;

	a.x = x;
	a.y = y;
	a.z = z;
	return (a);
}
