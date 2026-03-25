/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cone.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse cone properties (center, axis, diameter, height, color) from scene line
 */
static int ft_cone_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "cone point");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "cone orientation");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "cone diameter");
	ft_iterate_in_line(&line);
	(*cur)->height =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "cone height"));
	ft_iterate_in_line(&line);
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "cone color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_cone_check(win, cur)
								: ft_error(4, win, "cone");
	return (check == 0 ? 0 : ft_error(check, win, "cone"));
}

/* Initialize cone shape and delegate to parser */
int ft_cone_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_CONE;
	return (ft_cone_parse(win, cur, line));
}

/* Validate cone parameters (non-negative height/diameter, valid point, color,
 * orientation) */
int ft_cone_check(t_window *win, t_shape **cur)
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

/* Compute cone surface normal using gradient of the implicit cone equation */
void ft_cone_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;
	t_pt v;
	double m;
	double k;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	v = ft_subtraction(r, sh->pt_0);
	m = ft_dot_product(v, sh->ori);
	k = sh->cone_half_angle_sq;
	ray->hit_n = ft_normal_vect(
		ft_subtraction(v, ft_multi_scal(m * (1.0 + k), sh->ori)));
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}

/* Ray-cone intersection using quadratic formula with tapered radius along axis
 */
void ft_intersect_ray_cone(t_shape *sh, t_ray *ray)
{
	t_pt oc;
	double a;
	double b;
	double c;
	double disc;

	oc = ft_subtraction(ray->orig, sh->pt_0);
	a = ft_dot_product(ray->dir, ray->dir) -
		(1.0 + sh->cone_half_angle_sq) *
			ft_sqr(ft_dot_product(ray->dir, sh->ori));
	b = 2.0 * (ft_dot_product(oc, ray->dir) -
			   (1.0 + sh->cone_half_angle_sq) * ft_dot_product(oc, sh->ori) *
				   ft_dot_product(ray->dir, sh->ori));
	c = ft_dot_product(oc, oc) -
		(1.0 + sh->cone_half_angle_sq) * ft_sqr(ft_dot_product(oc, sh->ori));
	disc = b * b - 4.0 * a * c;
	if (disc < 0.0)
	{
		ray->lenght = -1;
		return;
	}
	ft_cone_solve(sh, ray, a, (t_pt){b, c, disc});
}

/* Select nearest valid quadratic root within cone height bounds */
void ft_cone_solve(t_shape *sh, t_ray *ray, double a, t_pt bcd)
{
	double t0;
	double t1;
	double m;

	t0 = (-bcd.x - sqrt(bcd.z)) / (2.0 * a);
	t1 = (-bcd.x + sqrt(bcd.z)) / (2.0 * a);
	if (t0 > t1)
	{
		m = t0;
		t0 = t1;
		t1 = m;
	}
	m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
								   ft_multi_scal(t0, ray->dir)),
					   sh->ori);
	if (t0 > EPSILON_HIT && m >= 0.0 && m <= sh->height)
	{
		ray->lenght = t0;
		ft_cone_norm(sh, ray);
		return;
	}
	m = ft_dot_product(ft_addition(ft_subtraction(ray->orig, sh->pt_0),
								   ft_multi_scal(t1, ray->dir)),
					   sh->ori);
	if (t1 > EPSILON_HIT && m >= 0.0 && m <= sh->height)
	{
		ray->lenght = t1;
		ft_cone_norm(sh, ray);
		return;
	}
	ray->lenght = -1;
}
