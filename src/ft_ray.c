/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:00:43 by alienard          #+#    #+#             */
/*   Updated: 2020/07/29 14:36:08 by alienard         ###   ########.fr       */
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

t_argb ft_trace_ray(t_window *win, t_cam *cam)
{
	t_shape *min_sh;
	double min;
	t_ray ray;
	t_argb black;

	min_sh = NULL;
	min = INFINITY;
	ft_init_ray_cam(cam, &ray);
	if (win->bvh)
		ft_bvh_trace(win->bvh, &ray, &min, &min_sh);
	else
		ft_trace_shapes(win->beg_sh, &ray, &min, &min_sh);
	if (min_sh == NULL)
		return (black = (t_argb){0, 0, 0, 0});
	min_sh->color.a = ft_lstsize_light(win);
	black = ft_albedo(ft_pre_light(win, min_sh, min, &ray), min_sh->color);
	return (black);
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
