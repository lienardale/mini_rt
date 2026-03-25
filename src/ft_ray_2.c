/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 21:56:28 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Return an invalid ray with length -1 (no intersection) */
t_ray ft_no_ray(void)
{
	t_ray ray;

	ray.lenght = -1;
	return (ray);
}

/* Dispatch ray-shape intersection test based on shape type ID */
void ft_which_shape(t_shape *sh, t_ray *ray)
{
	if (sh->id == SHAPE_SPHERE)
		ft_intersect_ray_sphere(sh, ray);
	else if (sh->id == SHAPE_PLANE)
		ft_intersect_ray_plan(sh, ray);
	else if (sh->id == SHAPE_SQUARE)
		ft_intersect_ray_square(sh, ray);
	else if (sh->id == SHAPE_CYLINDER)
		ft_intersect_ray_cylinder(sh, ray);
	else if (sh->id == SHAPE_TRIANGLE)
		ft_intersect_ray_triangle(sh, ray);
	else if (sh->id == SHAPE_CONE)
		ft_intersect_ray_cone(sh, ray);
	else if (sh->id == SHAPE_DISK)
		ft_intersect_ray_disk(sh, ray);
	else if (sh->id == SHAPE_TORUS)
		ft_intersect_ray_torus(sh, ray);
	else if (sh->id == SHAPE_ELLIPSOID)
		ft_intersect_ray_ellipsoid(sh, ray);
	else if (sh->id == SHAPE_BOX)
		ft_intersect_ray_box(sh, ray);
	else if (sh->id == SHAPE_HYPERBOLOID)
		ft_intersect_ray_hyperboloid(sh, ray);
	else if (sh->id == SHAPE_PARABOLOID)
		ft_intersect_ray_paraboloid(sh, ray);
	else if (sh->id == SHAPE_CSG)
		ray->lenght = -1;
}

/* Test a single shape, applying motion blur offset if the shape has velocity */
static void ft_trace_one_shape(t_shape *sh, t_ray *ray, double motion_time)
{
	t_ray moved;
	t_pt vel;

	vel = sh->mat.vel;
	if (motion_time > EPSILON_ZERO
		&& (vel.x != 0.0 || vel.y != 0.0 || vel.z != 0.0))
	{
		moved = *ray;
		moved.orig = ft_subtraction(ray->orig,
									ft_multi_scal(motion_time, vel));
		ft_which_shape(sh, &moved);
		ray->lenght = moved.lenght;
		ray->hit_n = moved.hit_n;
	}
	else
		ft_which_shape(sh, ray);
}

/* Iterate all shapes to find the closest ray intersection */
void ft_trace_shapes(t_shape *cur_shape, t_ray *ray, double *min,
					 t_shape **min_sh)
{
	t_pt best_n;

	best_n = (t_pt){0, 0, 0};
	while (cur_shape)
	{
		ft_trace_one_shape(cur_shape, ray, ray->motion_time);
		if (ray->lenght > EPSILON_HIT && ray->lenght < *min)
		{
			*min = ray->lenght;
			*min_sh = cur_shape;
			best_n = ray->hit_n;
		}
		cur_shape = cur_shape->next;
	}
	ray->hit_n = best_n;
}
