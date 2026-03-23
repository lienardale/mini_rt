/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_light.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 13:54:44 by alienard          #+#    #+#             */
/*   Updated: 2022/06/14 15:57:55 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_pt ft_pre_light(t_window *win, t_shape *sh, double clos, t_ray *ray)
{
	t_pt p;
	t_pt i;

	p = ft_addition(ray->orig, ft_multi_scal(clos, ray->dir));
	i = ft_light(win, sh->n, p, sh);
	return (i);
}

t_pt ft_light(t_window *win, t_pt n, t_pt p, t_shape *sh)
{
	t_light *cur_light;
	t_pt i;
	t_pt l;
	double n_dot_l;
	double min;

	i = ft_add_scal(win->ratio, (t_pt){0, 0, 0});
	ft_db_mult_to_add_pt(&i, win->ratio, win->col);
	cur_light = win->beg_light;
	while (cur_light)
	{
		l = ft_normal_vect(ft_subtraction(cur_light->coord, p));
		n_dot_l = ft_dot_product(n, l);
		min = ft_shadow(win, l, p, sh);
		min = min - ft_lenght(ft_subtraction(cur_light->coord, p));
		if (n_dot_l > 0.001 && min > 0.001)
		{
			n_dot_l = cur_light->light_ratio * n_dot_l /
					  (ft_lenght(l) * ft_lenght(n));
			ft_db_mult_to_add_pt(&i, n_dot_l, cur_light->col);
		}
		cur_light = cur_light->next;
	}
	return (i);
}

int ft_lstsize_light(t_window *win)
{
	t_light *cur_light;
	int len;

	len = 0;
	cur_light = win->beg_light;
	while (cur_light)
	{
		len++;
		cur_light = cur_light->next;
	}
	return (len);
}

double ft_shadow(t_window *win, t_pt n, t_pt p, t_shape *sh)
{
	t_shape *cur_shape;
	double min;
	t_ray ray;

	ray.orig = p;
	ray.dir = n;
	ray.lenght = -1;
	cur_shape = win->beg_sh;
	min = INFINITY;
	(void)sh;
	while (cur_shape)
	{
		ft_which_shape(cur_shape, &ray);
		if (ray.lenght > 0.0001 && ray.lenght < min)
			min = ray.lenght;
		cur_shape = cur_shape->next;
	}
	return (min);
}
