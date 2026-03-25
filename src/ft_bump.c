/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static double ft_bump_height(t_texture *bump, double u, double v)
{
	t_argb c;

	c = ft_texture_sample(bump, u, v);
	return ((c.r + c.g + c.b) / (3.0 * 255.0));
}

static void ft_get_tangent_bitangent(t_pt normal, t_pt *tangent,
									 t_pt *bitangent)
{
	if (fabs(normal.x) < 0.9)
		*tangent = ft_normal_vect(ft_cross_product(normal, (t_pt){1, 0, 0}));
	else
		*tangent = ft_normal_vect(ft_cross_product(normal, (t_pt){0, 1, 0}));
	*bitangent = ft_cross_product(normal, *tangent);
}

void ft_apply_bump_map(t_shape *sh, t_ray *ray)
{
	double u;
	double v;
	double du;
	double dv;
	double h_center;
	t_pt tangent;
	t_pt bitangent;
	t_pt perturbed;
	double step;

	if (!sh->mat.bump_map)
		return;
	if (sh->id == 's')
		ft_uv_sphere(sh, ray, &u, &v);
	else if (sh->id == 'p' || sh->id == 'q')
		ft_uv_plane(sh, ray, &u, &v);
	else if (sh->id == 'y')
		ft_uv_cylinder(sh, ray, &u, &v);
	else
		ft_uv_generic(sh, ray, &u, &v);
	step = 0.001;
	h_center = ft_bump_height(sh->mat.bump_map, u, v);
	du = ft_bump_height(sh->mat.bump_map, u + step, v) - h_center;
	dv = ft_bump_height(sh->mat.bump_map, u, v + step) - h_center;
	ft_get_tangent_bitangent(ray->hit_n, &tangent, &bitangent);
	perturbed = ft_addition(
		ray->hit_n,
		ft_addition(ft_multi_scal(-du * sh->mat.bump_strength, tangent),
					ft_multi_scal(-dv * sh->mat.bump_strength, bitangent)));
	ray->hit_n = ft_normal_vect(perturbed);
}
