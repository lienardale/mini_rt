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

void ft_ray(double i, double j, t_window *win, t_cam *cam)
{
	t_argb color;
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
	color = ft_trace_ray(win, cam);
	ft_pix(i, j, win, color);
}

void ft_init_ray_cam(t_cam *cam, t_ray *ray)
{
	ray->dir = cam->rij;
	ray->orig = cam->pij;
	ray->lenght = -1;
	ray->unit = cam->rij;
}

static void ft_trace_csg_shapes(t_window *win, t_ray *ray, double *min,
								t_shape **min_sh)
{
	t_shape *cur;

	cur = win->beg_sh;
	while (cur)
	{
		if (cur->id == 'g')
		{
			ft_intersect_ray_csg(cur, ray, win);
			if (ray->lenght > 0.0001 && ray->lenght < *min)
			{
				*min = ray->lenght;
				*min_sh = cur;
			}
		}
		cur = cur->next;
	}
}

t_argb ft_trace_ray(t_window *win, t_cam *cam)
{
	t_ray ray;

	ft_init_ray_cam(cam, &ray);
	return (ft_trace_ray_recursive(win, &ray, 0));
}

t_argb ft_trace_ray_recursive(t_window *win, t_ray *ray, int depth)
{
	t_shape *min_sh;
	double min;
	t_argb black;
	t_argb color;

	min_sh = NULL;
	min = INFINITY;
	if (win->bvh)
		ft_bvh_trace(win->bvh, ray, &min, &min_sh);
	else
		ft_trace_shapes(win->beg_sh, ray, &min, &min_sh);
	ft_trace_csg_shapes(win, ray, &min, &min_sh);
	if (min_sh == NULL)
		return (black = (t_argb){0, 0, 0, 0});
	if (min_sh->mat.texture)
		min_sh->color = ft_get_shape_color(min_sh, ray);
	if (min_sh->mat.bump_map)
		ft_apply_bump_map(min_sh, ray);
	color = ft_albedo(ft_pre_light(win, min_sh, min, ray), min_sh->color);
	if (depth < MAX_REFLECT_DEPTH && min_sh->mat.reflectivity > 0.001)
		ft_apply_reflection(win, ray, min_sh, min, &color, depth);
	return (color);
}

void ft_apply_reflection(t_window *win, t_ray *ray, t_shape *sh,
						 double t, t_argb *color, int depth)
{
	t_ray ref_ray;
	t_argb ref_color;
	double r;

	r = sh->mat.reflectivity;
	ref_ray.orig = ft_addition(ray->orig, ft_multi_scal(t + 0.001, ray->dir));
	ref_ray.dir = ft_reflect_ray(ray->dir, sh->n);
	ref_ray.lenght = -1;
	ref_color = ft_trace_ray_recursive(win, &ref_ray, depth + 1);
	color->r = color->r * (1.0 - r) + ref_color.r * r;
	color->g = color->g * (1.0 - r) + ref_color.g * r;
	color->b = color->b * (1.0 - r) + ref_color.b * r;
}

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
