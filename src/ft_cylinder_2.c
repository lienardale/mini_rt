/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cylinder_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:36:14 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 18:57:43 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void ft_cylinder_calc_one(t_shape *sh, t_ray *ray, t_mat *tmp, t_argb *dot)
{
	sh->in = 0;
	tmp->x = sh->pt_0;
	tmp->y = sh->cyl_top;
	dot->a = sh->diameter / 2;
	tmp->z = sh->cyl_axis;
	tmp->a = ft_subtraction(ray->orig, tmp->x);
	dot->r = sh->cyl_axis_len_sq;
	dot->g = ft_dot_product(tmp->z, ray->dir);
	dot->b = ft_dot_product(tmp->z, tmp->a);
}

void ft_cylinder_calc_two(t_pt *calc, t_ray *ray, t_mat *tmp, t_argb *dot)
{
	calc->x = dot->r - ft_sqr(dot->g);
	calc->y = dot->r * ft_dot_product(tmp->a, ray->dir) - dot->b * dot->g;
	calc->z = dot->r * ft_dot_product(tmp->a, tmp->a) - ft_sqr(dot->b) -
			  ft_sqr(dot->a) * dot->r;
}

double ft_cylinder_calc_three(t_pt *calc, t_argb *dist, t_argb *dot, t_ray *ray)
{
	dist->a = calc->y * calc->y - calc->x * calc->z;
	if (dist->a < 0.001 && (ray->lenght = -1))
		return (-1);
	ft_cylinder_calc_four(calc, dist, dot);
	return (0);
}

void ft_cylinder_calc_four(t_pt *calc, t_argb *dist, t_argb *dot)
{
	dist->a = sqrt(dist->a);
	dist->r = (-calc->y - dist->a) / calc->x;
	dist->g = (-calc->y + dist->a) / calc->x;
	if ((dist->g > 0.0001 && dist->g < dist->r) ||
		(dist->g > 0.0001 && dist->r < 0.0001))
		dist->r = dist->g;
	dist->b = dot->b + dist->r * dot->g;
}

double ft_cylinder_calc_five(t_shape *sh, t_argb *dist, t_argb *dot, t_ray *ray)
{
	if (dist->b > 0.0001 && dist->b < dot->r)
	{
		ray->lenght = dist->r;
		ft_cylinder_norm(sh, ray);
		return (-1);
	}
	dist->b = (dist->b < 0.0001) ? 0 : dot->r;
	dist->r = (dist->b - dot->b) / dot->g;
	ray->hit_n = sh->ori;
	return (0);
}
