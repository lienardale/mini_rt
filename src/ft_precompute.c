/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_precompute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Cache the squared radius for sphere intersection tests */
static void ft_precompute_sphere(t_shape *sh)
{
	double radius;

	radius = sh->diameter / 2.0;
	sh->radius_sq = radius * radius;
}

/* Precompute the plane equation constant d = -(normal . point) */
static void ft_precompute_plane(t_shape *sh)
{
	sh->plane_d = -ft_dot_product(sh->ori, sh->pt_0);
}

/* Precompute triangle normal, edge vectors, and plane constant */
static void ft_precompute_triangle(t_shape *sh)
{
	t_pt u;
	t_pt v;

	u = ft_subtraction(sh->pt_1, sh->pt_0);
	v = ft_subtraction(sh->pt_2, sh->pt_0);
	sh->tri_norm = ft_normal_vect(ft_cross_product(u, v));
	sh->tri_edge0 = ft_subtraction(sh->pt_1, sh->pt_0);
	sh->tri_edge1 = ft_subtraction(sh->pt_2, sh->pt_1);
	sh->tri_edge2 = ft_subtraction(sh->pt_0, sh->pt_2);
	sh->plane_d = -ft_dot_product(sh->tri_norm, sh->pt_0);
}

/* Precompute cylinder axis vector, top cap position, and squared radius */
static void ft_precompute_cylinder(t_shape *sh)
{
	sh->radius_sq = ft_sqr(sh->diameter / 2.0);
	sh->cyl_axis = ft_subtraction(
		ft_addition(sh->pt_0, ft_multi_scal(sh->height, sh->ori)), sh->pt_0);
	sh->cyl_top = ft_addition(sh->pt_0, ft_multi_scal(sh->height, sh->ori));
	sh->cyl_axis_len_sq = ft_dot_product(sh->cyl_axis, sh->cyl_axis);
}

/* Precompute the squared tangent of the cone half-angle */
static void ft_precompute_cone(t_shape *sh)
{
	double half_angle;

	half_angle = atan2(sh->diameter / 2.0, sh->height);
	sh->cone_half_angle_sq = tan(half_angle) * tan(half_angle);
}

/* Precompute the squared radius for disk intersection tests */
static void ft_precompute_disk(t_shape *sh)
{
	sh->radius_sq = ft_sqr(sh->diameter / 2.0);
}

/* Dispatch precomputation to the handler matching the shape type */
void ft_precompute_shape(t_shape *sh)
{
	if (sh->id == SHAPE_SPHERE)
		ft_precompute_sphere(sh);
	else if (sh->id == SHAPE_PLANE)
		ft_precompute_plane(sh);
	else if (sh->id == SHAPE_TRIANGLE)
		ft_precompute_triangle(sh);
	else if (sh->id == SHAPE_CYLINDER)
		ft_precompute_cylinder(sh);
	else if (sh->id == SHAPE_SQUARE)
		ft_precompute_plane(sh);
	else if (sh->id == SHAPE_CONE)
		ft_precompute_cone(sh);
	else if (sh->id == SHAPE_DISK)
		ft_precompute_disk(sh);
}

/* Precompute derived values for every shape in the scene */
void ft_precompute_shapes(t_window *win)
{
	t_shape *cur;

	cur = win->beg_sh;
	while (cur)
	{
		ft_precompute_shape(cur);
		cur = cur->next;
	}
}
