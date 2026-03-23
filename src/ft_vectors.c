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

double ft_lenght(t_pt a)
{
	double len;

	len = sqrt(ft_sqr(a.x) + ft_sqr(a.y) + ft_sqr(a.z));
	return (len);
}

void ft_inv_norm(t_pt *dir)
{
	dir->x = -dir->x;
	dir->y = -dir->y;
	dir->z = -dir->z;
}

t_pt ft_normal_vect(t_pt a)
{
	double len;
	t_pt b;

	len = ft_lenght(a);
	b = ft_div_scal(len, a);
	return (b);
}

double ft_dot_product(t_pt a, t_pt b)
{
	double c;

	c = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return (c);
}

t_pt ft_div_vect(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.x / b.x);
	c.y = (a.y / b.y);
	c.z = (a.z / b.z);
	return (c);
}
