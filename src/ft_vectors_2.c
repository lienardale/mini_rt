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

t_pt ft_cross_product(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.y * b.z) - (a.z * b.y);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);
	return (c);
}

t_pt ft_addition(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.x + b.x);
	c.y = (a.y + b.y);
	c.z = (a.z + b.z);
	return (c);
}

t_pt ft_subtraction(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.x - b.x);
	c.y = (a.y - b.y);
	c.z = (a.z - b.z);
	return (c);
}

t_pt ft_multi_vect(t_pt a, t_pt b)
{
	t_pt c;

	c.x = (a.x * b.x);
	c.y = (a.y * b.y);
	c.z = (a.z * b.z);
	return (c);
}

t_pt ft_neg_pt(t_pt dir)
{
	t_pt tmp;

	tmp.x = -dir.x;
	tmp.y = -dir.y;
	tmp.z = -dir.z;
	return (tmp);
}
