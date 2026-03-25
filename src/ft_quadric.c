/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quadric.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse hyperboloid properties (center, axis, radii, height, color) from scene line */
static int ft_hyperboloid_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "hyperboloid center");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "hyperboloid axis");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_1, &line)
								  : ft_error(7, win, "hyperboloid radii");
	if (ft_isdigit(*line) == 1)
		(*cur)->height = ft_atof(line);
	else
		check = ft_error(7, win, "hyperboloid height");
	ft_iterate_in_line(&line);
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "hyperboloid color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_hyperboloid_check(win, cur)
								: ft_error(4, win, "hyperboloid");
	return (check == 0 ? 0 : ft_error(check, win, "hyperboloid"));
}

/* Initialize hyperboloid shape and delegate to parser */
int ft_hyperboloid_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_HYPERBOLOID;
	return (ft_hyperboloid_parse(win, cur, line));
}

/* Validate hyperboloid parameters (positive radii/height, valid point, color, orientation) */
int ft_hyperboloid_check(t_window *win, t_shape **cur)
{
	int check;

	if (!cur || !(*cur))
		return (ft_error(17, win, ""));
	if ((*cur)->pt_1.x <= 0.0 || (*cur)->pt_1.y <= 0.0 || (*cur)->height <= 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*cur)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*cur)->color) : check;
	check = (check == 0) ? ft_ori_check(win, (*cur)->ori) : check;
	return (check);
}

/* Compute hyperboloid normal via gradient of x^2/a^2 + z^2/a^2 - y^2/b^2 = 1 */
void ft_hyperboloid_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt loc;
	double m;
	t_pt up;
	t_pt right;
	t_pt fwd;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	up = ft_normal_vect(sh->ori);
	if (fabs(up.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(up, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(up, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, up);
	loc = ft_subtraction(r, sh->pt_0);
	m = ft_dot_product(loc, up);
	ray->hit_n.x = 2.0 * ft_dot_product(loc, right) / ft_sqr(sh->pt_1.x);
	ray->hit_n.y = -2.0 * m / ft_sqr(sh->pt_1.y);
	ray->hit_n.z = 2.0 * ft_dot_product(loc, fwd) / ft_sqr(sh->pt_1.x);
	(void)m;
	ray->hit_n = ft_normal_vect(ray->hit_n);
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}

/* Ray-hyperboloid intersection: transform to local space and solve quadratic */
void ft_intersect_ray_hyperboloid(t_shape *sh, t_ray *ray)
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
	ft_hyperboloid_solve(sh, ray, (t_pt){ox, oy, oz}, (t_pt){dx, dy, dz});
}

/* Solve quadratic for hyperboloid intersection in local coordinates */
void ft_hyperboloid_solve(t_shape *sh, t_ray *ray, t_pt o, t_pt d)
{
	double a;
	double b;
	double c;
	double disc;
	double t0;
	double t1;
	double ax;
	double ay;

	ax = sh->pt_1.x;
	ay = sh->pt_1.y;
	a = d.x * d.x / (ax * ax) + d.z * d.z / (ax * ax) - d.y * d.y / (ay * ay);
	b = 2.0 *
		(o.x * d.x / (ax * ax) + o.z * d.z / (ax * ax) - o.y * d.y / (ay * ay));
	c = o.x * o.x / (ax * ax) + o.z * o.z / (ax * ax) - o.y * o.y / (ay * ay) -
		1.0;
	disc = b * b - 4.0 * a * c;
	if (disc < 0.0)
	{
		ray->lenght = -1;
		return;
	}
	t0 = (-b - sqrt(disc)) / (2.0 * a);
	t1 = (-b + sqrt(disc)) / (2.0 * a);
	ft_hyperboloid_pick(sh, ray, t0, t1);
}

/* Select nearest valid root within hyperboloid height bounds */
void ft_hyperboloid_pick(t_shape *sh, t_ray *ray, double t0, double t1)
{
	t_pt up;
	double m;

	up = ft_normal_vect(sh->ori);
	if (t0 > EPSILON_HIT)
	{
		m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
									   ft_multi_scal(t0, ray->dir)),
						   up);
		if (fabs(m) <= sh->height / 2.0)
		{
			ray->lenght = t0;
			ft_hyperboloid_norm(sh, ray);
			return;
		}
	}
	if (t1 > EPSILON_HIT)
	{
		m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
									   ft_multi_scal(t1, ray->dir)),
						   up);
		if (fabs(m) <= sh->height / 2.0)
		{
			ray->lenght = t1;
			ft_hyperboloid_norm(sh, ray);
			return;
		}
	}
	ray->lenght = -1;
}
