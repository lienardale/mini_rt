/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 21:56:28 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_ray ft_no_ray(void)
{
	t_ray ray;

	ray.lenght = -1;
	return (ray);
}

void ft_which_shape(t_shape *sh, t_ray *ray)
{
	if (sh->id == 's')
		ft_intersect_ray_sphere(sh, ray);
	else if (sh->id == 'p')
		ft_intersect_ray_plan(sh, ray);
	else if (sh->id == 'q')
		ft_intersect_ray_square(sh, ray);
	else if (sh->id == 'y')
		ft_intersect_ray_cylinder(sh, ray);
	else if (sh->id == 't')
		ft_intersect_ray_triangle(sh, ray);
	else if (sh->id == 'o')
		ft_intersect_ray_cone(sh, ray);
	else if (sh->id == 'k')
		ft_intersect_ray_disk(sh, ray);
	else if (sh->id == 'u')
		ft_intersect_ray_torus(sh, ray);
	else if (sh->id == 'e')
		ft_intersect_ray_ellipsoid(sh, ray);
	else if (sh->id == 'b')
		ft_intersect_ray_box(sh, ray);
	else if (sh->id == 'h')
		ft_intersect_ray_hyperboloid(sh, ray);
	else if (sh->id == 'a')
		ft_intersect_ray_paraboloid(sh, ray);
	else if (sh->id == 'g')
		ray->lenght = -1;
}

void ft_trace_shapes(t_shape *cur_shape, t_ray *ray, double *min,
					 t_shape **min_sh)
{
	t_pt best_n;

	best_n = (t_pt){0, 0, 0};
	while (cur_shape)
	{
		ft_which_shape(cur_shape, ray);
		if (ray->lenght > 0.0001 && ray->lenght < *min)
		{
			*min = ray->lenght;
			*min_sh = cur_shape;
			best_n = ray->hit_n;
		}
		cur_shape = cur_shape->next;
	}
	ray->hit_n = best_n;
}
