/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paraboloid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_paraboloid_init(t_window *win, t_shape **cur, char *line)
{
	int check;

	(*cur)->id = 'a';
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "paraboloid center");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "paraboloid axis");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "paraboloid focal_len");
	ft_iterate_in_line(&line);
	(*cur)->height =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "paraboloid height"));
	ft_iterate_in_line(&line);
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "paraboloid color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_paraboloid_check(win, cur)
								: ft_error(4, win, "paraboloid");
	return (check == 0 ? 0 : ft_error(check, win, "paraboloid"));
}

int ft_paraboloid_check(t_window *win, t_shape **cur)
{
	int check;

	if (!cur || !(*cur))
		return (ft_error(17, win, ""));
	if ((*cur)->diameter <= 0.0 || (*cur)->height <= 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*cur)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*cur)->color) : check;
	check = (check == 0) ? ft_ori_check(win, (*cur)->ori) : check;
	return (check);
}

void ft_paraboloid_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt oc;
	t_pt up;
	t_pt right;
	t_pt fwd;
	double lx;
	double lz;
	double ly;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	up = ft_normal_vect(sh->ori);
	if (fabs(up.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(up, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(up, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, up);
	oc = ft_subtraction(r, sh->pt_0);
	lx = ft_dot_product(oc, right);
	lz = ft_dot_product(oc, fwd);
	ly = -1.0;
	(void)ly;
	ray->hit_n.x =
		2.0 * lx * right.x + 2.0 * lz * fwd.x - 4.0 * sh->diameter * up.x;
	ray->hit_n.y =
		2.0 * lx * right.y + 2.0 * lz * fwd.y - 4.0 * sh->diameter * up.y;
	ray->hit_n.z =
		2.0 * lx * right.z + 2.0 * lz * fwd.z - 4.0 * sh->diameter * up.z;
	ray->hit_n = ft_normal_vect(ray->hit_n);
	if (ft_dot_product(ray->dir, ray->hit_n) > 0.001)
		ft_inv_norm(&ray->hit_n);
}

void ft_intersect_ray_paraboloid(t_shape *sh, t_ray *ray)
{
	t_pt oc;
	t_pt up;
	t_pt right;
	t_pt fwd;
	double ox;
	double oy;
	double oz;
	double dx;
	double dy;
	double dz;

	up = ft_normal_vect(sh->ori);
	if (fabs(up.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(up, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(up, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, up);
	oc = ft_subtraction(ray->orig, sh->pt_0);
	ox = ft_dot_product(oc, right);
	oy = ft_dot_product(oc, up);
	oz = ft_dot_product(oc, fwd);
	dx = ft_dot_product(ray->dir, right);
	dy = ft_dot_product(ray->dir, up);
	dz = ft_dot_product(ray->dir, fwd);
	ft_paraboloid_solve(sh, ray, (t_pt){ox, oy, oz}, (t_pt){dx, dy, dz});
}

void ft_paraboloid_solve(t_shape *sh, t_ray *ray, t_pt o, t_pt d)
{
	double f;
	double a;
	double b;
	double c;
	double disc;
	double t0;
	double t1;

	f = sh->diameter;
	a = d.x * d.x + d.z * d.z;
	b = 2.0 * (o.x * d.x + o.z * d.z) - 4.0 * f * d.y;
	c = o.x * o.x + o.z * o.z - 4.0 * f * o.y;
	disc = b * b - 4.0 * a * c;
	if (disc < 0.0)
	{
		ray->lenght = -1;
		return;
	}
	t0 = (-b - sqrt(disc)) / (2.0 * a);
	t1 = (-b + sqrt(disc)) / (2.0 * a);
	ft_paraboloid_pick(sh, ray, t0, t1);
}

void ft_paraboloid_pick(t_shape *sh, t_ray *ray, double t0, double t1)
{
	t_pt up;
	double m;

	up = ft_normal_vect(sh->ori);
	if (t0 > 0.0001)
	{
		m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
									   ft_multi_scal(t0, ray->dir)),
						   up);
		if (m >= 0.0 && m <= sh->height)
		{
			ray->lenght = t0;
			ft_paraboloid_norm(sh, ray);
			return;
		}
	}
	if (t1 > 0.0001)
	{
		m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
									   ft_multi_scal(t1, ray->dir)),
						   up);
		if (m >= 0.0 && m <= sh->height)
		{
			ray->lenght = t1;
			ft_paraboloid_norm(sh, ray);
			return;
		}
	}
	ray->lenght = -1;
}
