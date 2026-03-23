/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_box.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_box_init(t_window *win, t_shape **cur, char *line)
{
	int check;

	(*cur)->id = 'b';
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "box center");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_1, &line)
								  : ft_error(7, win, "box dimensions");
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "box color");
	if (check == 0)
		check =
			(*line == '\0') ? ft_box_check(win, cur) : ft_error(4, win, "box");
	return (check == 0 ? 0 : ft_error(check, win, "box"));
}

int ft_box_check(t_window *win, t_shape **cur)
{
	int check;

	if (!cur || !(*cur))
		return (ft_error(17, win, ""));
	if ((*cur)->pt_1.x <= 0.0 || (*cur)->pt_1.y <= 0.0 || (*cur)->pt_1.z <= 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*cur)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*cur)->color) : check;
	return (check);
}

static double ft_dmin_local(double a, double b)
{
	if (a < b)
		return (a);
	return (b);
}

static double ft_dmax_local(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

static void ft_box_slab(double *tmin, double *tmax, double orig, double dir,
						double bmin, double bmax)
{
	double inv;
	double t0;
	double t1;

	if (fabs(dir) < 1e-8)
	{
		if (orig < bmin || orig > bmax)
		{
			*tmin = INFINITY;
			*tmax = -INFINITY;
		}
		return;
	}
	inv = 1.0 / dir;
	t0 = (bmin - orig) * inv;
	t1 = (bmax - orig) * inv;
	if (inv < 0)
	{
		*tmin = ft_dmax_local(*tmin, t1);
		*tmax = ft_dmin_local(*tmax, t0);
	}
	else
	{
		*tmin = ft_dmax_local(*tmin, t0);
		*tmax = ft_dmin_local(*tmax, t1);
	}
}

void ft_box_compute_normal(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt half;
	t_pt d;
	double bias;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	half.x = sh->pt_1.x / 2.0;
	half.y = sh->pt_1.y / 2.0;
	half.z = sh->pt_1.z / 2.0;
	bias = 1.0001;
	d.x = r.x / half.x * bias;
	d.y = r.y / half.y * bias;
	d.z = r.z / half.z * bias;
	sh->n = (t_pt){0, 0, 0};
	if (fabs(d.x) > fabs(d.y) && fabs(d.x) > fabs(d.z))
		sh->n.x = (d.x > 0) ? 1.0 : -1.0;
	else if (fabs(d.y) > fabs(d.z))
		sh->n.y = (d.y > 0) ? 1.0 : -1.0;
	else
		sh->n.z = (d.z > 0) ? 1.0 : -1.0;
	if (ft_dot_product(ray->dir, sh->n) > 0.001)
		ft_inv_norm(&sh->n);
}

void ft_intersect_ray_box(t_shape *sh, t_ray *ray)
{
	double tmin;
	double tmax;
	t_pt bmin;
	t_pt bmax;

	bmin.x = sh->pt_0.x - sh->pt_1.x / 2.0;
	bmin.y = sh->pt_0.y - sh->pt_1.y / 2.0;
	bmin.z = sh->pt_0.z - sh->pt_1.z / 2.0;
	bmax.x = sh->pt_0.x + sh->pt_1.x / 2.0;
	bmax.y = sh->pt_0.y + sh->pt_1.y / 2.0;
	bmax.z = sh->pt_0.z + sh->pt_1.z / 2.0;
	tmin = 0.0001;
	tmax = INFINITY;
	ft_box_slab(&tmin, &tmax, ray->orig.x, ray->dir.x, bmin.x, bmax.x);
	ft_box_slab(&tmin, &tmax, ray->orig.y, ray->dir.y, bmin.y, bmax.y);
	ft_box_slab(&tmin, &tmax, ray->orig.z, ray->dir.z, bmin.z, bmax.z);
	if (tmax < tmin)
	{
		ray->lenght = -1;
		return;
	}
	ray->lenght = tmin;
	ft_box_compute_normal(sh, ray);
}
