/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_torus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse torus properties (center, orientation, major/minor radii, color) from
 * scene line */
static int ft_torus_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "torus center");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "torus orientation");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "torus major radius");
	ft_iterate_in_line(&line);
	(*cur)->height =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "torus minor radius"));
	ft_iterate_in_line(&line);
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "torus color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_torus_check(win, cur)
								: ft_error(4, win, "torus");
	return (check == 0 ? 0 : ft_error(check, win, "torus"));
}

/* Initialize torus shape and delegate to parser */
int ft_torus_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_TORUS;
	return (ft_torus_parse(win, cur, line));
}

/* Validate torus parameters (positive radii, valid point, color, orientation)
 */
int ft_torus_check(t_window *win, t_shape **cur)
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

/* Transform ray into local torus coordinate space using orientation basis */
static void ft_torus_transform_ray(t_shape *sh, t_ray *ray, t_pt *lo, t_pt *ld)
{
	t_pt up;
	t_pt right;
	t_pt fwd;
	t_pt oc;

	up = ft_normal_vect(sh->ori);
	if (fabs(up.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(up, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(up, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, up);
	oc = ft_subtraction(ray->orig, sh->pt_0);
	lo->x = ft_dot_product(oc, right);
	lo->y = ft_dot_product(oc, up);
	lo->z = ft_dot_product(oc, fwd);
	ld->x = ft_dot_product(ray->dir, right);
	ld->y = ft_dot_product(ray->dir, up);
	ld->z = ft_dot_product(ray->dir, fwd);
}

/* Build quartic polynomial coefficients for torus intersection */
static void ft_torus_build_quartic(double *c, t_pt lo, t_pt ld, double big_r,
								   double small_r)
{
	double dd;
	double oo;
	double od;
	double r2;
	double big_r2;

	dd = ft_dot_product(ld, ld);
	oo = ft_dot_product(lo, lo);
	od = ft_dot_product(lo, ld);
	r2 = small_r * small_r;
	big_r2 = big_r * big_r;
	c[0] = dd * dd;
	c[1] = 4.0 * dd * od;
	c[2] = 2.0 * dd * (oo - r2 - big_r2) + 4.0 * od * od +
		   4.0 * big_r2 * ld.y * ld.y;
	c[3] = 4.0 * od * (oo - r2 - big_r2) + 8.0 * big_r2 * lo.y * ld.y;
	c[4] = (oo - r2 - big_r2) * (oo - r2 - big_r2) -
		   4.0 * big_r2 * (r2 - lo.y * lo.y);
}

/* Solve quadratic equation ax^2 + bx + c = 0, return number of real roots */
static int ft_solve_quadratic_local(double a, double b, double c, double *r)
{
	double disc;

	disc = b * b - 4.0 * a * c;
	if (disc < 0.0)
		return (0);
	disc = sqrt(disc);
	r[0] = (-b - disc) / (2.0 * a);
	r[1] = (-b + disc) / (2.0 * a);
	return (2);
}

/* Compute cube root supporting negative values */
static double ft_cbrt(double x)
{
	if (x >= 0.0)
		return (pow(x, 1.0 / 3.0));
	return (-pow(-x, 1.0 / 3.0));
}

/* Solve depressed cubic using Cardano's formula, return number of real roots */
static int ft_solve_cubic(double a, double b, double c, double *r)
{
	double p;
	double q;
	double disc;
	double u;
	double v;

	p = b - a * a / 3.0;
	q = 2.0 * a * a * a / 27.0 - a * b / 3.0 + c;
	disc = q * q / 4.0 + p * p * p / 27.0;
	if (disc >= 0.0)
	{
		u = ft_cbrt(-q / 2.0 + sqrt(disc));
		v = ft_cbrt(-q / 2.0 - sqrt(disc));
		r[0] = u + v - a / 3.0;
		return (1);
	}
	disc = acos(-q / 2.0 * sqrt(-27.0 / (p * p * p)));
	u = 2.0 * sqrt(-p / 3.0);
	r[0] = u * cos(disc / 3.0) - a / 3.0;
	r[1] = u * cos((disc + 2.0 * M_PI) / 3.0) - a / 3.0;
	r[2] = u * cos((disc + 4.0 * M_PI) / 3.0) - a / 3.0;
	return (3);
}

/* Attempt to factor quartic into two quadratics using a cubic resolvent root */
static int ft_try_cubic_root(double *coef, double y, double *roots)
{
	double p;
	double q;
	double r1[2];
	double r2[2];
	int n;
	int count;

	p = coef[0] * coef[0] / 4.0 - coef[1] + y;
	q = y * y / 4.0 - coef[3];
	if (p < -1e-10 || q < -1e-10)
		return (0);
	p = (p < 0) ? 0 : sqrt(p);
	q = (q < 0) ? 0 : sqrt(q);
	if (coef[0] * y / 2.0 - coef[2] < 0)
		q = -q;
	count = 0;
	n = ft_solve_quadratic_local(1.0, coef[0] / 2.0 + p, y / 2.0 + q, r1);
	if (n > 0 && r1[0] > EPSILON_HIT)
		roots[count++] = r1[0];
	if (n > 1 && r1[1] > EPSILON_HIT)
		roots[count++] = r1[1];
	n = ft_solve_quadratic_local(1.0, coef[0] / 2.0 - p, y / 2.0 - q, r2);
	if (n > 0 && r2[0] > EPSILON_HIT)
		roots[count++] = r2[0];
	if (n > 1 && r2[1] > EPSILON_HIT)
		roots[count++] = r2[1];
	return (count);
}

/* Solve quartic equation using Ferrari's method via cubic resolvent */
static int ft_solve_quartic(double *c, double *roots)
{
	double coef[4];
	double cubic_roots[3];
	int nc;
	int i;
	int count;

	coef[0] = c[1] / c[0];
	coef[1] = c[2] / c[0];
	coef[2] = c[3] / c[0];
	coef[3] = c[4] / c[0];
	nc = ft_solve_cubic(-coef[1], coef[0] * coef[2] - 4.0 * coef[3],
						-coef[0] * coef[0] * coef[3] + 4.0 * coef[1] * coef[3] -
							coef[2] * coef[2],
						cubic_roots);
	i = -1;
	while (++i < nc)
	{
		count = ft_try_cubic_root(coef, cubic_roots[i], roots);
		if (count > 0)
			return (count);
	}
	return (0);
}

/* Public wrapper to solve quartic x^4 + ax^3 + bx^2 + cx + d = 0 */
int ft_quartic_inner(double a, double b, double cc, double d,
					 double *cubic_roots, double *roots)
{
	double c[5];

	(void)cubic_roots;
	c[0] = 1;
	c[1] = a;
	c[2] = b;
	c[3] = cc;
	c[4] = d;
	return (ft_solve_quartic(c, roots));
}

/* Compute torus surface normal in local space, then convert to world space */
void ft_torus_norm(t_shape *sh, t_ray *ray)
{
	t_pt lo;
	t_pt ld;
	t_pt p;
	double param;
	t_pt proj;

	ft_torus_transform_ray(sh, ray, &lo, &ld);
	p = ft_addition(lo, ft_multi_scal(ray->lenght, ld));
	param = sqrt(p.x * p.x + p.z * p.z);
	if (param < EPSILON_ZERO)
		param = EPSILON_ZERO;
	proj.x = sh->diameter * p.x / param;
	proj.y = 0;
	proj.z = sh->diameter * p.z / param;
	ray->hit_n = ft_normal_vect(ft_subtraction(p, proj));
	ft_torus_world_normal(sh, ray);
}

/* Transform local-space normal back to world coordinates using orientation
 * basis */
void ft_torus_world_normal(t_shape *sh, t_ray *ray)
{
	t_pt up;
	t_pt right;
	t_pt fwd;
	t_pt n;

	up = ft_normal_vect(sh->ori);
	if (fabs(up.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(up, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(up, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, up);
	n.x = ray->hit_n.x * right.x + ray->hit_n.y * up.x + ray->hit_n.z * fwd.x;
	n.y = ray->hit_n.x * right.y + ray->hit_n.y * up.y + ray->hit_n.z * fwd.y;
	n.z = ray->hit_n.x * right.z + ray->hit_n.y * up.z + ray->hit_n.z * fwd.z;
	ray->hit_n = ft_normal_vect(n);
}

/* Ray-torus intersection by solving quartic equation in local space */
void ft_intersect_ray_torus(t_shape *sh, t_ray *ray)
{
	t_pt lo;
	t_pt ld;
	double c[5];
	double roots[4];
	int n;
	int i;
	double best;

	ft_torus_transform_ray(sh, ray, &lo, &ld);
	ft_torus_build_quartic(c, lo, ld, sh->diameter, sh->height);
	n = ft_solve_quartic(c, roots);
	if (n == 0)
	{
		ray->lenght = -1;
		return;
	}
	best = INFINITY;
	i = -1;
	while (++i < n)
	{
		if (roots[i] > EPSILON_HIT && roots[i] < best)
			best = roots[i];
	}
	if (best >= INFINITY)
	{
		ray->lenght = -1;
		return;
	}
	ray->lenght = best;
	ft_torus_norm(sh, ray);
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}
