/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ellipsoid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse ellipsoid properties (center, radii per axis, color) from scene line */
static int ft_ellipsoid_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "ellipsoid center");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_1, &line)
								  : ft_error(7, win, "ellipsoid radii");
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "ellipsoid color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_ellipsoid_check(win, cur)
								: ft_error(4, win, "ellipsoid");
	return (check == 0 ? 0 : ft_error(check, win, "ellipsoid"));
}

/* Initialize ellipsoid shape and delegate to parser */
int ft_ellipsoid_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_ELLIPSOID;
	return (ft_ellipsoid_parse(win, cur, line));
}

/* Validate ellipsoid parameters (positive radii, valid center and color) */
int ft_ellipsoid_check(t_window *win, t_shape **cur)
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

/* Compute ellipsoid normal via gradient of implicit surface
 * (x/a)^2+(y/b)^2+(z/c)^2=1 */
void ft_ellipsoid_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	ray->hit_n.x = 2.0 * r.x / (sh->pt_1.x * sh->pt_1.x);
	ray->hit_n.y = 2.0 * r.y / (sh->pt_1.y * sh->pt_1.y);
	ray->hit_n.z = 2.0 * r.z / (sh->pt_1.z * sh->pt_1.z);
	ray->hit_n = ft_normal_vect(ray->hit_n);
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}

/* Ray-ellipsoid intersection by scaling ray into unit-sphere space */
void ft_intersect_ray_ellipsoid(t_shape *sh, t_ray *ray)
{
	t_pt oc;
	t_pt d_s;
	t_pt oc_s;
	double a;
	double b;

	oc = ft_subtraction(ray->orig, sh->pt_0);
	d_s.x = ray->dir.x / sh->pt_1.x;
	d_s.y = ray->dir.y / sh->pt_1.y;
	d_s.z = ray->dir.z / sh->pt_1.z;
	oc_s.x = oc.x / sh->pt_1.x;
	oc_s.y = oc.y / sh->pt_1.y;
	oc_s.z = oc.z / sh->pt_1.z;
	a = ft_dot_product(d_s, d_s);
	b = 2.0 * ft_dot_product(oc_s, d_s);
	ft_ellipsoid_solve(sh, ray, a,
					   (t_pt){b, ft_dot_product(oc_s, oc_s) - 1.0, 0});
}

/* Solve quadratic and select nearest positive root for ellipsoid hit */
void ft_ellipsoid_solve(t_shape *sh, t_ray *ray, double a, t_pt bc)
{
	double disc;
	double t0;
	double t1;

	disc = bc.x * bc.x - 4.0 * a * bc.y;
	if (disc < 0.0)
	{
		ray->lenght = -1;
		return;
	}
	t0 = (-bc.x - sqrt(disc)) / (2.0 * a);
	t1 = (-bc.x + sqrt(disc)) / (2.0 * a);
	if (t0 > EPSILON_HIT)
		ray->lenght = t0;
	else if (t1 > EPSILON_HIT)
		ray->lenght = t1;
	else
	{
		ray->lenght = -1;
		return;
	}
	ft_ellipsoid_norm(sh, ray);
}
