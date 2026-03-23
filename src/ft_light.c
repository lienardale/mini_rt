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
	t_pt view_dir;

	p = ft_addition(ray->orig, ft_multi_scal(clos, ray->dir));
	view_dir = ft_normal_vect(ft_neg_pt(ray->dir));
	i = ft_light(win, sh->n, p, sh, view_dir);
	return (i);
}

static void ft_add_specular(t_pt *i, t_pt l_vec, t_pt n, t_pt view_dir,
							t_shape *sh, t_argb l_col)
{
	t_argb spec;

	if (sh->mat.specular <= 0.0)
		return;
	spec = ft_apply_specular(view_dir, l_vec, n, &sh->mat, l_col);
	i->x += spec.r / 255.0;
	i->y += spec.g / 255.0;
	i->z += spec.b / 255.0;
}

static double ft_soft_shadow_sample(t_window *win, t_pt l_vec, t_pt p,
									double light_dist)
{
	t_pt perp1;
	t_pt perp2;
	t_pt offset;
	double total;
	int s;

	if (fabs(l_vec.x) < 0.9)
		perp1 = ft_normal_vect(ft_cross_product(l_vec, (t_pt){1, 0, 0}));
	else
		perp1 = ft_normal_vect(ft_cross_product(l_vec, (t_pt){0, 1, 0}));
	perp2 = ft_cross_product(l_vec, perp1);
	total = 0.0;
	s = 0;
	while (s < 16)
	{
		offset = ft_addition(
			l_vec, ft_addition(ft_multi_scal(((s % 4) - 1.5) * 0.02, perp1),
							   ft_multi_scal(((s / 4) - 1.5) * 0.02, perp2)));
		offset = ft_normal_vect(offset);
		if (ft_shadow(win, offset, p, light_dist) > 0.001)
			total += 1.0;
		s++;
	}
	return (total / 16.0);
}

t_pt ft_light(t_window *win, t_pt n, t_pt p, t_shape *sh, t_pt view_dir)
{
	t_light *cur_light;
	t_pt i;
	t_pt l_vec;
	double n_dot_l;
	double light_dist;
	double shadow_factor;

	i = ft_add_scal(win->ratio, (t_pt){0, 0, 0});
	ft_db_mult_to_add_pt(&i, win->ratio, win->col);
	cur_light = win->beg_light;
	while (cur_light)
	{
		l_vec = ft_subtraction(cur_light->coord, p);
		light_dist = ft_lenght(l_vec);
		l_vec = ft_div_scal(light_dist, l_vec);
		n_dot_l = ft_dot_product(n, l_vec);
		if (n_dot_l > 0.001)
		{
			shadow_factor = ft_soft_shadow_sample(win, l_vec, p, light_dist);
			if (shadow_factor > 0.001)
			{
				n_dot_l = cur_light->light_ratio * n_dot_l / ft_lenght(n);
				ft_db_mult_to_add_pt(&i, n_dot_l * shadow_factor,
									 cur_light->col);
				ft_add_specular(&i, l_vec, n, view_dir, sh, cur_light->col);
			}
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

double ft_shadow(t_window *win, t_pt n, t_pt p, double light_dist)
{
	t_shape *cur_shape;
	t_shape *min_sh;
	double min;
	t_ray ray;

	ray.orig = p;
	ray.dir = n;
	ray.lenght = -1;
	min_sh = NULL;
	if (win->bvh)
	{
		min = light_dist;
		ft_bvh_trace(win->bvh, &ray, &min, &min_sh);
		if (min_sh == NULL)
			return (1.0);
		return (min - light_dist);
	}
	min = INFINITY;
	cur_shape = win->beg_sh;
	while (cur_shape)
	{
		ft_which_shape(cur_shape, &ray);
		if (ray.lenght > 0.0001 && ray.lenght < min)
		{
			min = ray.lenght;
			if (min < light_dist)
				return (min - light_dist);
		}
		cur_shape = cur_shape->next;
	}
	return (min - light_dist);
}
