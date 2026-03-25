/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pbr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Schlick's approximation for Fresnel reflectance */
double ft_fresnel_schlick(double cos_theta, double f0)
{
	double x;

	if (cos_theta < 0.0)
		cos_theta = 0.0;
	x = 1.0 - cos_theta;
	return (f0 + (1.0 - f0) * x * x * x * x * x);
}

/* GGX/Trowbridge-Reitz normal distribution function */
static double ft_ggx_ndf(double n_dot_h, double roughness)
{
	double a;
	double a2;
	double denom;

	a = roughness * roughness;
	a2 = a * a;
	denom = n_dot_h * n_dot_h * (a2 - 1.0) + 1.0;
	denom = M_PI * denom * denom;
	if (denom < EPSILON_ZERO)
		return (0.0);
	return (a2 / denom);
}

/* Schlick-GGX geometry function for a single direction */
static double ft_geom_schlick_ggx(double n_dot_v, double roughness)
{
	double k;

	k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	return (n_dot_v / (n_dot_v * (1.0 - k) + k));
}

/* Smith's geometry function combining view and light masking */
static double ft_geom_smith(double n_dot_v, double n_dot_l, double roughness)
{
	return (ft_geom_schlick_ggx(n_dot_v, roughness) *
			ft_geom_schlick_ggx(n_dot_l, roughness));
}

/* Cook-Torrance PBR BRDF: combines GGX NDF, geometry, and Fresnel */
t_argb ft_pbr_shade(t_pt view, t_pt light_dir, t_pt normal, t_material *mat,
					t_argb light_col, t_argb albedo)
{
	t_argb result;
	t_pt half_v;
	double n_dot_l;
	double n_dot_v;
	double n_dot_h;
	double h_dot_v;
	double d;
	double g;
	double f;
	double f0;
	double spec;
	double kd;

	result = (t_argb){0, 0, 0, 0};
	n_dot_l = ft_dot_product(normal, light_dir);
	if (n_dot_l <= 0.0)
		return (result);
	n_dot_v = ft_dot_product(normal, view);
	if (n_dot_v <= 0.0)
		n_dot_v = EPSILON_ZERO;
	half_v = ft_normal_vect(ft_addition(view, light_dir));
	n_dot_h = fmax(ft_dot_product(normal, half_v), 0.0);
	h_dot_v = fmax(ft_dot_product(half_v, view), 0.0);
	f0 = 0.04 * (1.0 - mat->metallic) + mat->metallic;
	d = ft_ggx_ndf(n_dot_h, mat->roughness);
	g = ft_geom_smith(n_dot_v, n_dot_l, mat->roughness);
	f = ft_fresnel_schlick(h_dot_v, f0);
	spec = (d * g * f) / (4.0 * n_dot_v * n_dot_l + EPSILON_ZERO);
	kd = (1.0 - f) * (1.0 - mat->metallic);
	result.r = (kd * albedo.r / 255.0 / M_PI + spec) * light_col.r * n_dot_l;
	result.g = (kd * albedo.g / 255.0 / M_PI + spec) * light_col.g * n_dot_l;
	result.b = (kd * albedo.b / 255.0 / M_PI + spec) * light_col.b * n_dot_l;
	return (result);
}
