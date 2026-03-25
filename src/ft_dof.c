/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dof.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Compute the focal point where this ray intersects the focal plane */
static t_pt ft_dof_focal_point(t_cam *cam, double focal_dist)
{
	return (ft_addition(cam->pij, ft_multi_scal(focal_dist, cam->rij)));
}

/* Cast a single DOF-jittered ray and return its color */
static t_argb ft_dof_single(t_window *win, t_cam *cam, t_pt focal_pt,
							unsigned int *seed)
{
	t_pt lens_pt;
	t_pt lens_offset;
	t_pt right;
	t_pt up;
	t_cam dof_cam;

	lens_pt = ft_rand_disk(seed);
	if (fabs(cam->rij.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(cam->rij, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(cam->rij, (t_pt){0, 1, 0}));
	up = ft_cross_product(cam->rij, right);
	lens_offset = ft_addition(ft_multi_scal(lens_pt.x * cam->aperture, right),
							  ft_multi_scal(lens_pt.y * cam->aperture, up));
	dof_cam = *cam;
	dof_cam.pij = ft_addition(cam->pij, lens_offset);
	dof_cam.rij = ft_normal_vect(ft_subtraction(focal_pt, dof_cam.pij));
	return (ft_trace_ray(win, &dof_cam));
}

/* Render a pixel with depth of field: average DOF_SAMPLES jittered rays */
void ft_ray_dof(double i, double j, t_window *win, t_cam *cam)
{
	t_argb color;
	t_argb sample;
	unsigned int seed;
	t_pt focal_pt;
	int s;

	ft_ray_single_setup(i, j, win, cam);
	focal_pt = ft_dof_focal_point(cam, cam->focal_dist);
	color = (t_argb){0, 0, 0, 0};
	s = 0;
	while (s < DOF_SAMPLES)
	{
		seed = ft_rand_seed((int)i, (int)j, s);
		sample = ft_dof_single(win, cam, focal_pt, &seed);
		color.r += sample.r;
		color.g += sample.g;
		color.b += sample.b;
		s++;
	}
	color.r /= (double)DOF_SAMPLES;
	color.g /= (double)DOF_SAMPLES;
	color.b /= (double)DOF_SAMPLES;
	ft_pix((int)i, (int)j, win, color);
}
