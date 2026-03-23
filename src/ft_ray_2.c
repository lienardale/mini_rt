/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 21:56:28 by alienard          #+#    #+#             */
/*   Updated: 2020/02/27 17:46:22 by alienard         ###   ########.fr       */
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
}
