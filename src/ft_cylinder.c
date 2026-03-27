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

static void ft_cylinder_calc_four(t_pt *calc, t_argb *dist, t_argb *dot);
void ft_cylinder_norm(t_shape *sh, t_ray *ray);

/* Prepare cylinder intersection data: endpoints, axis, and dot products */
static void ft_cylinder_calc_one(t_shape *sh, t_ray *ray, t_mat *tmp,
								 t_argb *dot)
{
	tmp->x = sh->pt_0;
	tmp->y = sh->cyl_top;
	dot->a = sh->diameter / 2;
	tmp->z = sh->cyl_axis;
	tmp->a = ft_subtraction(ray->orig, tmp->x);
	dot->r = sh->cyl_axis_len_sq;
	dot->g = ft_dot_product(tmp->z, ray->dir);
	dot->b = ft_dot_product(tmp->z, tmp->a);
}

/* Compute quadratic coefficients (a, b, c) for cylinder intersection */
static void ft_cylinder_calc_two(t_pt *calc, t_ray *ray, t_mat *tmp,
								 t_argb *dot)
{
	calc->x = dot->r - ft_sqr(dot->g);
	calc->y = dot->r * ft_dot_product(tmp->a, ray->dir) - dot->b * dot->g;
	calc->z = dot->r * ft_dot_product(tmp->a, tmp->a) - ft_sqr(dot->b) -
			  ft_sqr(dot->a) * dot->r;
}

/* Compute discriminant and solve for intersection distances */
static double ft_cylinder_calc_three(t_pt *calc, t_argb *dist, t_argb *dot,
									 t_ray *ray)
{
	dist->a = calc->y * calc->y - calc->x * calc->z;
	if (dist->a < EPSILON_NORMAL)
	{
		ray->lenght = -1;
		return (-1);
	}
	ft_cylinder_calc_four(calc, dist, dot);
	return (0);
}

/* Select nearest valid root from the quadratic and project onto axis */
static void ft_cylinder_calc_four(t_pt *calc, t_argb *dist, t_argb *dot)
{
	dist->a = sqrt(dist->a);
	dist->r = (-calc->y - dist->a) / calc->x;
	dist->g = (-calc->y + dist->a) / calc->x;
	if ((dist->g > EPSILON_HIT && dist->g < dist->r) ||
		(dist->g > EPSILON_HIT && dist->r < EPSILON_HIT))
		dist->r = dist->g;
	dist->b = dot->b + dist->r * dot->g;
}

/* Check if hit is within cylinder height bounds; handle cap fallback */
static double ft_cylinder_calc_five(t_shape *sh, t_argb *dist, t_argb *dot,
									t_ray *ray)
{
	if (dist->b > EPSILON_HIT && dist->b < dot->r)
	{
		ray->lenght = dist->r;
		ft_cylinder_norm(sh, ray);
		return (-1);
	}
	dist->b = (dist->b < EPSILON_HIT) ? 0 : dot->r;
	dist->r = (dist->b - dot->b) / dot->g;
	ray->hit_n = sh->ori;
	return (0);
}

/* Parse cylinder properties (center, axis, diameter, height, color) from scene
 * line */
static int ft_cylinder_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

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

/* Initialize cylinder shape and delegate to parser */
int ft_cylinder_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_CYLINDER;
	return (ft_cylinder_parse(win, cur, line));
}

/* Validate cylinder parameters (non-negative height/diameter, valid point,
 * color, orientation) */
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

/* Compute cylinder surface normal using double cross product with axis */
void ft_cylinder_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt n;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	n = ft_subtraction(r, sh->pt_0);
	n = ft_normal_vect(ft_cross_product(n, sh->ori));
	n = ft_cross_product(n, sh->ori);
	ft_inv_norm(&n);
	ray->hit_n = n;
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}

/* Test ray intersection with top and bottom disk caps of the cylinder */
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
		if (fabs(denom) < EPSILON_ZERO)
			continue;
		t = ft_dot_product(ft_subtraction(cap_center, ray->orig), sh->ori) /
			denom;
		if (t < EPSILON_HIT || t >= *best_t)
			continue;
		p = ft_addition(ray->orig, ft_multi_scal(t, ray->dir));
		p = ft_subtraction(p, cap_center);
		if (ft_dot_product(p, p) <= sh->radius_sq)
		{
			*best_t = t;
			ray->lenght = t;
			ray->hit_n = sh->ori;
			if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
				ft_inv_norm(&ray->hit_n);
		}
	}
}

/* Ray-cylinder intersection using quadratic formula projected off axis, with
 * cap checks */
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
	if ((ret = (int)ft_cylinder_calc_three(&calc, &dist, &dot, ray)) != 0)
	{
		ray->lenght = -1;
		best_t = INFINITY;
		ft_cylinder_cap_check(sh, ray, &best_t);
		return;
	}
	if ((ret = (int)ft_cylinder_calc_five(sh, &dist, &dot, ray)) != 0)
	{
		best_t = ray->lenght;
		ft_cylinder_cap_check(sh, ray, &best_t);
		return;
	}
	if ((fabs(calc.y + calc.x * dist.r)) < dist.a)
	{
		ray->lenght = dist.r + EPSILON_NORMAL;
		best_t = ray->lenght;
		ft_cylinder_cap_check(sh, ray, &best_t);
		if (ft_dot_product(ray->dir, ray->hit_n) > 0)
			ft_inv_norm(&ray->hit_n);
		return;
	}
	ray->lenght = -1;
	best_t = INFINITY;
	ft_cylinder_cap_check(sh, ray, &best_t);
}
