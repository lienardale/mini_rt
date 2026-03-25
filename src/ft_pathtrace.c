/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pathtrace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Compute direct lighting at a hit point using PBR or Phong shading */
static t_argb ft_pt_direct(t_window *win, t_ray *ray, t_shape *sh, t_pt p,
						   t_argb albedo)
{
	t_light *cur;
	t_argb color;
	t_pt l_vec;
	double l_dist;
	double shadow;
	t_pt view;

	color = (t_argb){0, 0, 0, 0};
	view = ft_normal_vect(ft_neg_pt(ray->dir));
	cur = win->beg_light;
	while (cur)
	{
		l_vec = ft_subtraction(cur->coord, p);
		l_dist = ft_lenght(l_vec);
		l_vec = ft_div_scal(l_dist, l_vec);
		if (ft_dot_product(ray->hit_n, l_vec) > EPSILON_NORMAL)
		{
			shadow = ft_shadow(win, l_vec, p, l_dist);
			if (shadow > EPSILON_NORMAL)
			{
				if (sh->mat.roughness > EPSILON_ZERO
					|| sh->mat.metallic > EPSILON_ZERO)
				{
					t_argb pbr;

					pbr = ft_pbr_shade(view, l_vec, ray->hit_n, &sh->mat,
									   cur->col, albedo);
					color.r += pbr.r * cur->light_ratio;
					color.g += pbr.g * cur->light_ratio;
					color.b += pbr.b * cur->light_ratio;
				}
				else
				{
					double n_dot_l;

					n_dot_l = cur->light_ratio
						* ft_dot_product(ray->hit_n, l_vec);
					color.r += n_dot_l * cur->col.r * albedo.r / 255.0;
					color.g += n_dot_l * cur->col.g * albedo.g / 255.0;
					color.b += n_dot_l * cur->col.b * albedo.b / 255.0;
				}
			}
		}
		cur = cur->next;
	}
	color.r += win->ratio * win->col.r * albedo.r / 255.0;
	color.g += win->ratio * win->col.g * albedo.g / 255.0;
	color.b += win->ratio * win->col.b * albedo.b / 255.0;
	return (color);
}

/* Compute luminance of an ARGB color for Russian Roulette */
static double ft_luminance(t_argb c)
{
	return (0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b);
}

/* Trace a single path through the scene with Monte Carlo bounces,
 * Russian Roulette termination, emissive materials, and environment sky */
static t_argb ft_pathtrace_ray(t_window *win, t_ray *ray, int bounces,
							   unsigned int *seed)
{
	t_shape *min_sh;
	double min;
	t_argb albedo;
	t_argb color;
	t_pt p;
	double rr_prob;

	min_sh = NULL;
	min = INFINITY;
	if (win->bvh)
		ft_bvh_trace(win->bvh, ray, &min, &min_sh);
	else
		ft_trace_shapes(win->beg_sh, ray, &min, &min_sh);
	if (min_sh == NULL)
		return (ft_env_sky(ray->dir));
	albedo = min_sh->color;
	if (min_sh->mat.texture)
		albedo = ft_get_shape_color(min_sh, ray);
	if (min_sh->mat.bump_map)
		ft_apply_bump_map(min_sh, ray);
	p = ft_addition(ray->orig, ft_multi_scal(min, ray->dir));
	color = ft_pt_direct(win, ray, min_sh, p, albedo);
	color.r += min_sh->mat.emission.r;
	color.g += min_sh->mat.emission.g;
	color.b += min_sh->mat.emission.b;
	if (bounces <= 0)
		return (color);
	rr_prob = fmin(ft_luminance(albedo) / 255.0, 0.95);
	if (bounces < win->path_trace_bounces && ft_rand_float(seed) > rr_prob)
		return (color);
	if (bounces < win->path_trace_bounces)
	{
		t_ray bounce;
		t_argb indirect;
		double cosine_pdf;
		double n_dot_d;

		bounce.orig =
			ft_addition(p, ft_multi_scal(EPSILON_NORMAL, ray->hit_n));
		if (min_sh->mat.reflectivity > EPSILON_ZERO
			&& ft_rand_float(seed) < min_sh->mat.reflectivity)
			bounce.dir = ft_reflect_ray(ray->dir, ray->hit_n);
		else
			bounce.dir = ft_rand_hemisphere_cosine(ray->hit_n, seed);
		bounce.lenght = -1;
		bounce.motion_time = ray->motion_time;
		indirect = ft_pathtrace_ray(win, &bounce, bounces - 1, seed);
		n_dot_d = fmax(ft_dot_product(ray->hit_n, bounce.dir), 0.0);
		cosine_pdf = n_dot_d / M_PI;
		if (cosine_pdf > EPSILON_ZERO)
		{
			color.r += indirect.r * (albedo.r / 255.0) * n_dot_d
				/ (cosine_pdf * rr_prob);
			color.g += indirect.g * (albedo.g / 255.0) * n_dot_d
				/ (cosine_pdf * rr_prob);
			color.b += indirect.b * (albedo.b / 255.0) * n_dot_d
				/ (cosine_pdf * rr_prob);
		}
	}
	else
	{
		t_ray bounce;
		t_argb indirect;

		bounce.orig =
			ft_addition(p, ft_multi_scal(EPSILON_NORMAL, ray->hit_n));
		bounce.dir = ft_rand_hemisphere_cosine(ray->hit_n, seed);
		bounce.lenght = -1;
		bounce.motion_time = ray->motion_time;
		indirect = ft_pathtrace_ray(win, &bounce, bounces - 1, seed);
		color.r += indirect.r * (albedo.r / 255.0);
		color.g += indirect.g * (albedo.g / 255.0);
		color.b += indirect.b * (albedo.b / 255.0);
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
