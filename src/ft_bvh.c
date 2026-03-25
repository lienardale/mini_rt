/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bvh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Return the smaller of two doubles */
static double ft_dmin(double a, double b)
{
	if (a < b)
		return (a);
	return (b);
}

/* Return the larger of two doubles */
static double ft_dmax(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

/* Compute axis-aligned bounding box for sphere, cone, or disk shapes */
static t_aabb ft_aabb_sphere_cone_disk(t_shape *sh)
{
	t_aabb box;
	double r;

	if (sh->id == SHAPE_SPHERE || sh->id == SHAPE_DISK)
	{
		r = sh->diameter / 2.0;
		box.min = ft_sub_scal(r, sh->pt_0);
		box.max = ft_add_scal(r, sh->pt_0);
	}
	else
	{
		r = sh->diameter / 2.0;
		box.min = ft_sub_scal(r, sh->pt_0);
		box.max = ft_add_scal(
			r, ft_addition(sh->pt_0, ft_multi_scal(sh->height, sh->ori)));
		box.min.x = ft_dmin(box.min.x, sh->pt_0.x - r);
		box.min.y = ft_dmin(box.min.y, sh->pt_0.y - r);
		box.min.z = ft_dmin(box.min.z, sh->pt_0.z - r);
	}
	return (box);
}

/* Compute axis-aligned bounding box for a triangle with epsilon padding */
static t_aabb ft_aabb_triangle(t_shape *sh)
{
	t_aabb box;

	box.min.x =
		ft_dmin(sh->pt_0.x, ft_dmin(sh->pt_1.x, sh->pt_2.x)) - EPSILON_AABB;
	box.min.y =
		ft_dmin(sh->pt_0.y, ft_dmin(sh->pt_1.y, sh->pt_2.y)) - EPSILON_AABB;
	box.min.z =
		ft_dmin(sh->pt_0.z, ft_dmin(sh->pt_1.z, sh->pt_2.z)) - EPSILON_AABB;
	box.max.x =
		ft_dmax(sh->pt_0.x, ft_dmax(sh->pt_1.x, sh->pt_2.x)) + EPSILON_AABB;
	box.max.y =
		ft_dmax(sh->pt_0.y, ft_dmax(sh->pt_1.y, sh->pt_2.y)) + EPSILON_AABB;
	box.max.z =
		ft_dmax(sh->pt_0.z, ft_dmax(sh->pt_1.z, sh->pt_2.z)) + EPSILON_AABB;
	return (box);
}

/* Compute axis-aligned bounding box for ellipsoid or box shapes */
static t_aabb ft_aabb_ellipsoid_box(t_shape *sh)
{
	t_aabb box;

	if (sh->id == SHAPE_ELLIPSOID)
	{
		box.min.x = sh->pt_0.x - sh->pt_1.x;
		box.min.y = sh->pt_0.y - sh->pt_1.y;
		box.min.z = sh->pt_0.z - sh->pt_1.z;
		box.max.x = sh->pt_0.x + sh->pt_1.x;
		box.max.y = sh->pt_0.y + sh->pt_1.y;
		box.max.z = sh->pt_0.z + sh->pt_1.z;
	}
	else
	{
		box.min.x = sh->pt_0.x - sh->pt_1.x / 2.0;
		box.min.y = sh->pt_0.y - sh->pt_1.y / 2.0;
		box.min.z = sh->pt_0.z - sh->pt_1.z / 2.0;
		box.max.x = sh->pt_0.x + sh->pt_1.x / 2.0;
		box.max.y = sh->pt_0.y + sh->pt_1.y / 2.0;
		box.max.z = sh->pt_0.z + sh->pt_1.z / 2.0;
	}
	return (box);
}

/* Return the AABB for any shape, dispatching by shape type */
t_aabb ft_shape_aabb(t_shape *sh)
{
	t_aabb box;
	double r;

	if (sh->id == SHAPE_SPHERE || sh->id == SHAPE_DISK || sh->id == SHAPE_CONE)
		return (ft_aabb_sphere_cone_disk(sh));
	else if (sh->id == SHAPE_TRIANGLE)
		return (ft_aabb_triangle(sh));
	else if (sh->id == SHAPE_ELLIPSOID || sh->id == SHAPE_BOX)
		return (ft_aabb_ellipsoid_box(sh));
	else if (sh->id == SHAPE_TORUS)
	{
		r = sh->diameter + sh->height;
		box.min = ft_sub_scal(r, sh->pt_0);
		box.max = ft_add_scal(r, sh->pt_0);
		return (box);
	}
	else
	{
		box.min = (t_pt){-1e6, -1e6, -1e6};
		box.max = (t_pt){1e6, 1e6, 1e6};
	}
	return (box);
}

/* Update ray interval for one axis using the slab intersection method */
static void ft_slab_test(double *tmin, double *tmax, double orig, double dir,
						 double bmin, double bmax)
{
	double inv;
	double t0;
	double t1;

	if (fabs(dir) < EPSILON_ZERO)
	{
		if (orig < bmin || orig > bmax)
		{
			*tmin = INFINITY;
			*tmax = -INFINITY;
		}
		return;
	}
	inv = 1.0 / dir;
	t0 = (bmin - orig) * inv;
	t1 = (bmax - orig) * inv;
	if (inv < 0)
	{
		*tmin = ft_dmax(*tmin, t1);
		*tmax = ft_dmin(*tmax, t0);
	}
	else
	{
		*tmin = ft_dmax(*tmin, t0);
		*tmax = ft_dmin(*tmax, t1);
	}
}

/* Test whether a ray intersects an AABB within [epsilon, t_max] */
int ft_aabb_hit(t_aabb *box, t_ray *ray, double t_max)
{
	double tmin;
	double tmax;

	tmin = EPSILON_HIT;
	tmax = t_max;
	ft_slab_test(&tmin, &tmax, ray->orig.x, ray->dir.x, box->min.x, box->max.x);
	if (tmax < tmin)
		return (0);
	ft_slab_test(&tmin, &tmax, ray->orig.y, ray->dir.y, box->min.y, box->max.y);
	if (tmax < tmin)
		return (0);
	ft_slab_test(&tmin, &tmax, ray->orig.z, ray->dir.z, box->min.z, box->max.z);
	return (tmax >= tmin);
}
