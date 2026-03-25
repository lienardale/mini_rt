/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_motion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Check if any shape in the scene has a non-zero velocity */
int ft_scene_has_motion(t_window *win)
{
	t_shape *sh;

	sh = win->beg_sh;
	while (sh)
	{
		if (sh->mat.vel.x != 0.0 || sh->mat.vel.y != 0.0
			|| sh->mat.vel.z != 0.0)
			return (1);
		sh = sh->next;
	}
	return (0);
}

/* Render a pixel with motion blur: average samples at different times */
void ft_ray_motion(double i, double j, t_window *win, t_cam *cam)
{
	t_argb color;
	t_argb sample;
	unsigned int seed;
	int s;
	int n;
	t_cam local;
	t_ray ray;

	n = win->motion_blur_samples;
	color = (t_argb){0, 0, 0, 0};
	s = 0;
	while (s < n)
	{
		seed = ft_rand_seed((int)i, (int)j, s + 1000);
		local = *cam;
		ft_ray_single_setup(i, j, win, &local);
		ft_init_ray_cam(&local, &ray);
		ray.motion_time = ft_rand_float(&seed);
		sample = ft_trace_ray_recursive(win, &ray, 0);
		color.r += sample.r;
		color.g += sample.g;
		color.b += sample.b;
		s++;
	}
	color.r /= (double)n;
	color.g /= (double)n;
	color.b /= (double)n;
	ft_pix((int)i, (int)j, win, color);
}
