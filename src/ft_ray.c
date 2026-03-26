/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:00:43 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Compute ray origin and direction for pixel (i,j) on camera (no trace) */
void ft_ray_single_setup(double i, double j, t_window *win, t_cam *cam)
{
	double size;
	t_mat unit;

	unit.x = (t_pt){1, 0, 0};
	unit.y = (t_pt){0, 1, 0};
	unit.z = (t_pt){0, 0, -1};
	size = tan(cam->fov * M_PI / 180 / 2);
	cam->pij = cam->coord;
	cam->rij.x = (i - (win->x / 2));
	cam->rij.y = (j - (win->y / 2));
	cam->rij.z = -(win->x / (2 * size));
	cam->rij = ft_normal_vect(cam->rij);
	cam->rij = ft_rotation_vect(cam->rij, cam->ori);
	cam->rij = ft_point_matrix_transl(cam->rij, unit);
	cam->pij = ft_point_matrix_transl(cam->pij, unit);
}

/* Cast a single ray through pixel (i,j) and return the resulting color */
static t_argb ft_ray_single(double i, double j, t_window *win, t_cam *cam)
{
	ft_ray_single_setup(i, j, win, cam);
	return (ft_trace_ray(win, cam));
}

/* Cast 4 sub-pixel rays for anti-aliasing and write the averaged color */
void ft_ray(double i, double j, t_window *win, t_cam *cam)
{
	t_argb color;
	t_argb sample;
	double offsets[4];
	int s;

	if (win->path_trace_spp > 0)
	{
		ft_pathtrace_pixel(i, j, win, cam);
		return;
	}
	if (cam->aperture > EPSILON_ZERO)
	{
		ft_ray_dof(i, j, win, cam);
		return;
	}
	if (win->motion_blur_samples > 1)
	{
		ft_ray_motion(i, j, win, cam);
		return;
	}
	offsets[0] = -0.25;
	offsets[1] = 0.25;
	offsets[2] = -0.25;
	offsets[3] = 0.25;
	color = (t_argb){0, 0, 0, 0};
	s = 0;
	while (s < 4)
	{
		sample =
			ft_ray_single(i + offsets[s % 2], j + offsets[s / 2], win, cam);
		color.r += sample.r;
		color.g += sample.g;
		color.b += sample.b;
		s++;
	}
	color.r /= 4.0;
	color.g /= 4.0;
	color.b /= 4.0;
	ft_pix((int)i, (int)j, win, color);
}

/* Initialize a ray from the camera's current position and direction */
void ft_init_ray_cam(t_cam *cam, t_ray *ray)
{
	ray->dir = cam->rij;
	ray->orig = cam->pij;
	ray->lenght = -1;
	ray->unit = cam->rij;
	ray->motion_time = 0.0;
}

/* Test ray against all CSG (constructive solid geometry) shapes */
static void ft_trace_csg_shapes(t_window *win, t_ray *ray, double *min,
								t_shape **min_sh)
{
	t_shape *cur;
	t_pt best_n;

	best_n = ray->hit_n;
	cur = win->beg_sh;
	while (cur)
	{
		if (cur->id == SHAPE_CSG)
		{
			ft_intersect_ray_csg(cur, ray, win);
			if (ray->lenght > EPSILON_HIT && ray->lenght < *min)
			{
				*min = ray->lenght;
				*min_sh = cur;
				best_n = ray->hit_n;
			}
		}
		cur = cur->next;
	}
	ray->hit_n = best_n;
}

/* Trace a primary ray from camera and return the shaded pixel color */
t_argb ft_trace_ray(t_window *win, t_cam *cam)
{
	t_ray ray;

	ft_init_ray_cam(cam, &ray);
	return (ft_trace_ray_recursive(win, &ray, 0));
}

/* Compute refracted ray through transparent surface and blend its color */
static void ft_apply_refraction(t_window *win, t_ray *ray, t_shape *sh,
								double t, t_argb *color, int depth)
{
	t_ray ref_ray;
	t_argb ref_color;
	double tr;
	double eta;

	tr = sh->mat.transparency;
	eta = 1.0 / sh->mat.refr_index;
	ref_ray.orig =
		ft_addition(ray->orig, ft_multi_scal(t + EPSILON_NORMAL, ray->dir));
	ref_ray.dir = ft_refract_ray(ray->dir, ray->hit_n, eta);
	ref_ray.lenght = -1;
	ref_ray.motion_time = ray->motion_time;
	ref_color = ft_trace_ray_recursive(win, &ref_ray, depth + 1);
	color->r = color->r * (1.0 - tr) + ref_color.r * tr;
	color->g = color->g * (1.0 - tr) + ref_color.g * tr;
	color->b = color->b * (1.0 - tr) + ref_color.b * tr;
}

/* Recursively trace a ray, handling intersections, lighting, reflection, and
 * refraction */
t_argb ft_trace_ray_recursive(t_window *win, t_ray *ray, int depth)
{
	t_shape *min_sh;
	double min;
	t_argb black;
	t_argb color;
	t_argb hit_color;

	min_sh = NULL;
	min = INFINITY;
	if (win->bvh)
		ft_bvh_trace(win->bvh, ray, &min, &min_sh);
	else
		ft_trace_shapes(win->beg_sh, ray, &min, &min_sh);
	ft_trace_csg_shapes(win, ray, &min, &min_sh);
	if (min_sh == NULL)
	{
		if (win->use_env_sky)
			return (ft_env_sky(ray->dir));
		return (black = (t_argb){0, 0, 0, 0});
	}
	hit_color = min_sh->color;
	if (min_sh->mat.texture)
		hit_color = ft_get_shape_color(min_sh, ray);
	if (min_sh->mat.bump_map)
		ft_apply_bump_map(min_sh, ray);
	color = ft_albedo(ft_pre_light(win, min_sh, min, ray), hit_color);
	if (depth < MAX_REFLECT_DEPTH && min_sh->mat.reflectivity > EPSILON_NORMAL)
		ft_apply_reflection(win, ray, min_sh, min, &color, depth);
	if (depth < MAX_REFLECT_DEPTH && min_sh->mat.transparency > EPSILON_NORMAL)
		ft_apply_refraction(win, ray, min_sh, min, &color, depth);
	return (color);
}

/* Compute reflected ray off a surface and blend its color by reflectivity */
void ft_apply_reflection(t_window *win, t_ray *ray, t_shape *sh, double t,
						 t_argb *color, int depth)
{
	t_ray ref_ray;
	t_argb ref_color;
	double r;

	r = sh->mat.reflectivity;
	ref_ray.orig =
		ft_addition(ray->orig, ft_multi_scal(t + EPSILON_NORMAL, ray->dir));
	ref_ray.dir = ft_reflect_ray(ray->dir, ray->hit_n);
	ref_ray.lenght = -1;
	ref_ray.motion_time = ray->motion_time;
	ref_color = ft_trace_ray_recursive(win, &ref_ray, depth + 1);
	color->r = color->r * (1.0 - r) + ref_color.r * r;
	color->g = color->g * (1.0 - r) + ref_color.g * r;
	color->b = color->b * (1.0 - r) + ref_color.b * r;
}

/* Construct a ray with origin, direction, and precomputed hit point */
t_ray ft_shoot_ray(t_pt orig, t_pt dir, double t)
{
	t_ray ray;

	ray.orig = orig;
	ray.dir = dir;
	ray.lenght = t;
	ray.unit = ft_addition(orig, ft_multi_scal(t, dir));
	ray.unit = ft_normal_vect(ray.unit);
	return (ray);
}
