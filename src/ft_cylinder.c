/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cylinder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 13:58:00 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:21:57 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_cylinder_init(t_window *win, t_shape **cur, char *line)
{
	int check;

	(*cur)->id = 'y';
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "cylinder point");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "cylinder orientation");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "cylinder diameter");
	ft_iterate_in_line(&line);
	(*cur)->height =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "cylinder height"));
	ft_iterate_in_line(&line);
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "cylinder color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_cylinder_check(win, cur)
								: ft_error(4, win, "cylinder");
	return (check == 0 ? 0 : ft_error(check, win, "cylinder"));
}

int ft_cylinder_check(t_window *win, t_shape **cur)
{
	int check;

	if (!cur || !(*cur))
		return (ft_error(17, win, ""));
	if ((*cur)->height < 0.0 || (*cur)->diameter < 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*cur)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*cur)->color) : check;
	check = (check == 0) ? ft_ori_check(win, (*cur)->ori) : check;
	return (check);
}

void ft_cylinder_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt n;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	n = ft_subtraction(r, sh->pt_0);
	n = ft_normal_vect(ft_cross_product(n, sh->ori));
	n = ft_cross_product(n, sh->ori);
	ft_inv_norm(&n);
	sh->n = n;
	if (ft_dot_product(ray->dir, sh->n) > 0.001)
		ft_inv_norm(&sh->n);
}

void ft_cylinder_cap_check(t_shape *sh, t_ray *ray, double *best_t)
{
	double denom;
	double t;
	t_pt p;
	t_pt cap_center;
	int i;

	i = -1;
	while (++i < 2)
	{
		cap_center = (i == 0) ? sh->pt_0 : sh->cyl_top;
		denom = ft_dot_product(sh->ori, ray->dir);
		if (fabs(denom) < 1e-8)
			continue;
		t = ft_dot_product(ft_subtraction(cap_center, ray->orig), sh->ori) /
			denom;
		if (t < 0.0001 || t >= *best_t)
			continue;
		p = ft_addition(ray->orig, ft_multi_scal(t, ray->dir));
		p = ft_subtraction(p, cap_center);
		if (ft_dot_product(p, p) <= sh->radius_sq)
		{
			*best_t = t;
			ray->lenght = t;
			sh->n = sh->ori;
			if (ft_dot_product(ray->dir, sh->n) > 0.001)
				ft_inv_norm(&sh->n);
		}
	}
}

void ft_intersect_ray_cylinder(t_shape *sh, t_ray *ray)
{
	t_mat tmp;
	t_argb dot;
	t_pt calc;
	t_argb dist;
	int ret;
	double best_t;

	ft_cylinder_calc_one(sh, ray, &tmp, &dot);
	ft_cylinder_calc_two(&calc, ray, &tmp, &dot);
	if ((ret = ft_cylinder_calc_three(&calc, &dist, &dot, ray)) != 0)
	{
		ray->lenght = -1;
		best_t = INFINITY;
		ft_cylinder_cap_check(sh, ray, &best_t);
		return;
	}
	if ((ret = ft_cylinder_calc_five(sh, &dist, &dot, ray)) != 0)
	{
		best_t = ray->lenght;
		ft_cylinder_cap_check(sh, ray, &best_t);
		return;
	}
	if ((fabs(calc.y + calc.x * dist.r)) < dist.a &&
		(ray->lenght = dist.r + 0.001))
	{
		best_t = ray->lenght;
		ft_cylinder_cap_check(sh, ray, &best_t);
		if (ft_dot_product(ray->dir, sh->n) > 0)
			ft_inv_norm(&sh->n);
		return;
	}
	ray->lenght = -1;
	best_t = INFINITY;
	ft_cylinder_cap_check(sh, ray, &best_t);
}
