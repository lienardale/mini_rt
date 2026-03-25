/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 12:06:42 by alienard          #+#    #+#             */
/*   Updated: 2020/07/20 14:10:39 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Multiply point p by 3x3 matrix m: applies rotation/transform */
t_pt ft_point_matrix_transl(t_pt p, t_mat m)
{
	t_pt n;

	n.x = p.x * m.x.x + p.y * m.y.x + p.z * m.z.x;
	n.y = p.x * m.x.y + p.y * m.y.y + p.z * m.z.y;
	n.z = p.x * m.x.z + p.y * m.y.z + p.z * m.z.z;
	return (n);
}

/* Translate point m1 by offset coord (adds non-zero components) */
t_pt ft_translation(t_pt coord, t_pt m1)
{
	t_pt m2;

	m2 = m1;
	if (coord.x != 0)
		m2.x += coord.x;
	if (coord.y != 0)
		m2.y += coord.y;
	if (coord.x != 0)
		m2.z += coord.z;
	return (m2);
}
