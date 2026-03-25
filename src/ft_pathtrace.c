/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pathtrace.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Compute indirect illumination color contribution from shape albedo */
static t_argb ft_indirect_color(t_argb indirect, t_argb shape_col)
{
	t_argb result;

	result.a = 0;
	result.r = indirect.r * (shape_col.r / 255.0);
	result.g = indirect.g * (shape_col.g / 255.0);
	result.b = indirect.b * (shape_col.b / 255.0);
	return (result);
}

/* Trace a single path through the scene with Monte Carlo bounces */
static t_argb ft_pathtrace_ray(t_window *win, t_ray *ray, int bounces,
							   unsigned int *seed)
{
	t_shape *min_sh;
	double min;
	t_argb color;
	t_argb hit_color;
	t_pt p;
	t_pt view_dir;

	min_sh = NULL;
	min = INFINITY;
	if (win->bvh)
		ft_bvh_trace(win->bvh, ray, &min, &min_sh);
	else
		ft_trace_shapes(win->beg_sh, ray, &min, &min_sh);
	if (min_sh == NULL)
		return ((t_argb){0, 0, 0, 0});
	hit_color = min_sh->color;
	if (min_sh->mat.texture)
		hit_color = ft_get_shape_color(min_sh, ray);
	if (min_sh->mat.bump_map)
		ft_apply_bump_map(min_sh, ray);
	p = ft_addition(ray->orig, ft_multi_scal(min, ray->dir));
	view_dir = ft_normal_vect(ft_neg_pt(ray->dir));
	color =
		ft_albedo(ft_light(win, ray->hit_n, p, min_sh, view_dir), hit_color);
	if (bounces > 0)
	{
		t_ray bounce;
		t_argb indirect;

		bounce.orig = ft_addition(p, ft_multi_scal(EPSILON_NORMAL, ray->hit_n));
		bounce.dir = ft_rand_hemisphere_cosine(ray->hit_n, seed);
		bounce.lenght = -1;
		bounce.motion_time = ray->motion_time;
		indirect = ft_pathtrace_ray(win, &bounce, bounces - 1, seed);
		indirect = ft_indirect_color(indirect, hit_color);
		color.r += indirect.r;
		color.g += indirect.g;
		color.b += indirect.b;
	}
	return (color);
}

/* Render a pixel using path tracing: average spp samples with random bounces */
void ft_pathtrace_pixel(double i, double j, t_window *win, t_cam *cam)
{
	t_argb color;
	t_argb sample;
	unsigned int seed;
	int s;
	int spp;
	t_cam local;
	t_ray ray;

	spp = win->path_trace_spp;
	color = (t_argb){0, 0, 0, 0};
	s = 0;
	while (s < spp)
	{
		seed = ft_rand_seed((int)i, (int)j, s);
		local = *cam;
		ft_ray_single_setup(i + ft_rand_float(&seed) - 0.5,
							j + ft_rand_float(&seed) - 0.5, win, &local);
		ft_init_ray_cam(&local, &ray);
		sample = ft_pathtrace_ray(win, &ray, win->path_trace_bounces, &seed);
		color.r += sample.r;
		color.g += sample.g;
		color.b += sample.b;
		s++;
	}
	color.r /= (double)spp;
	color.g /= (double)spp;
	color.b /= (double)spp;
	ft_pix((int)i, (int)j, win, color);
}
