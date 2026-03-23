/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_material.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void ft_material_default(t_material *mat)
{
	mat->reflectivity = 0.0;
	mat->transparency = 0.0;
	mat->refr_index = 1.0;
	mat->specular = 0.0;
	mat->shininess = 32.0;
	mat->texture = NULL;
	mat->bump_map = NULL;
	mat->tex_scale_u = 1.0;
	mat->tex_scale_v = 1.0;
	mat->bump_strength = 1.0;
}

static int ft_parse_mat_prop(t_material *mat, char **line)
{
	if (ft_strncmp(*line, "refl:", 5) == 0)
	{
		*line += 5;
		mat->reflectivity = ft_atof(*line);
	}
	else if (ft_strncmp(*line, "trans:", 6) == 0)
	{
		*line += 6;
		mat->transparency = ft_atof(*line);
	}
	else if (ft_strncmp(*line, "ior:", 4) == 0)
	{
		*line += 4;
		mat->refr_index = ft_atof(*line);
	}
	else if (ft_strncmp(*line, "spec:", 5) == 0)
	{
		*line += 5;
		mat->specular = ft_atof(*line);
	}
	else
		return (0);
	ft_iterate_in_line(line);
	return (1);
}

int ft_parse_material(t_window *win, t_material *mat, char **line)
{
	(void)win;
	while ((ft_isspace(**line)) == 1)
		(*line)++;
	while (**line != '\0' && ft_parse_mat_prop(mat, line))
		;
	return (0);
}

t_argb ft_apply_specular(t_pt view, t_pt light_dir, t_pt normal,
						 t_material *mat, t_argb light_col)
{
	t_argb spec;
	t_pt reflect;
	double dot;
	double factor;

	spec = (t_argb){0, 0, 0, 0};
	if (mat->specular <= 0.0)
		return (spec);
	dot = ft_dot_product(normal, light_dir);
	reflect = ft_subtraction(ft_multi_scal(2.0 * dot, normal), light_dir);
	reflect = ft_normal_vect(reflect);
	dot = ft_dot_product(reflect, view);
	if (dot <= 0.0)
		return (spec);
	factor = mat->specular * pow(dot, mat->shininess);
	spec.r = factor * light_col.r;
	spec.g = factor * light_col.g;
	spec.b = factor * light_col.b;
	return (spec);
}

t_pt ft_reflect_ray(t_pt dir, t_pt normal)
{
	double dot;

	dot = ft_dot_product(dir, normal);
	return (ft_subtraction(dir, ft_multi_scal(2.0 * dot, normal)));
}

t_pt ft_refract_ray(t_pt dir, t_pt normal, double eta)
{
	double cos_i;
	double sin2_t;
	t_pt refracted;

	cos_i = -ft_dot_product(dir, normal);
	sin2_t = eta * eta * (1.0 - cos_i * cos_i);
	if (sin2_t > 1.0)
		return (ft_reflect_ray(dir, normal));
	refracted =
		ft_addition(ft_multi_scal(eta, dir),
					ft_multi_scal(eta * cos_i - sqrt(1.0 - sin2_t), normal));
	return (ft_normal_vect(refracted));
}
