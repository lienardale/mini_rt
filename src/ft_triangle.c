/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_triangle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 14:00:15 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:24:15 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse triangle properties (three vertices, color) from scene line */
static int ft_triangle_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "triangle point 1");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_1, &line)
								  : ft_error(7, win, "triangle point 2");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_2, &line)
								  : ft_error(7, win, "triangle point 3");
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "triangle color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_triangle_check(win, cur)
								: ft_error(4, win, "triangle");
	return (check == 0 ? 0 : ft_error(check, win, "triangle"));
}

/* Initialize triangle shape and delegate to parser */
int ft_triangle_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_TRIANGLE;
	return (ft_triangle_parse(win, cur, line));
}

/* Validate triangle parameters (three vertices, color) */
int ft_triangle_check(t_window *win, t_shape **current)
{
	int check;

	if (!current || !(*current))
		return (ft_error(17, win, ""));
	check = ft_pt_check(win, (*current)->pt_0);
	check = (check == 0) ? ft_pt_check(win, (*current)->pt_1) : check;
	check = (check == 0) ? ft_pt_check(win, (*current)->pt_2) : check;
	check = (check == 0) ? ft_color_check(win, (*current)->color) : check;
	return (check);
}

/* Ray-triangle intersection: plane test followed by edge containment check */
void ft_intersect_ray_triangle(t_shape *sh, t_ray *ray)
{
	double d;
	double t;
	t_pt r;

	ray->hit_n = sh->tri_norm;
	t = -(ft_dot_product(ray->hit_n, ray->orig) + sh->plane_d) /
		ft_dot_product(ray->hit_n, ray->dir);
	if (t < EPSILON_HIT)
	{
		ray->lenght = -1;
		return;
	}
	r = ft_addition(ray->orig, ft_multi_scal(t, ray->dir));
	if ((d = ft_is_in_triangle(r, sh, ray)) == 0)
		ray->lenght = t;
	else
		ray->lenght = -1;
}

/* Compute triangle normal via cross product of two edges */
void ft_triangle_norm(t_shape *sh, t_ray *ray)
{
	t_pt u;
	t_pt v;

	u = ft_subtraction(sh->pt_1, sh->pt_0);
	v = ft_subtraction(sh->pt_2, sh->pt_0);
	ray->hit_n = ft_cross_product(u, v);
	ray->hit_n = ft_normal_vect(ray->hit_n);
}

/* Edge-based containment test: point is inside if all cross products agree with
 * normal */
double ft_is_in_triangle(t_pt r, t_shape *sh, t_ray *ray)
{
	t_mat c;

	c.x = ft_subtraction(r, sh->pt_0);
	c.y = ft_subtraction(r, sh->pt_1);
	c.z = ft_subtraction(r, sh->pt_2);
	if (ft_dot_product(ray->hit_n, ft_cross_product(sh->tri_edge0, c.x)) < 0 ||
		ft_dot_product(ray->hit_n, ft_cross_product(sh->tri_edge1, c.y)) < 0 ||
		ft_dot_product(ray->hit_n, ft_cross_product(sh->tri_edge2, c.z)) < 0)
		return (-1);
	ray->hit_n = sh->tri_norm;
	if (ft_dot_product(ft_subtraction(sh->pt_0, ray->orig), ray->hit_n) >
		EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
	return (0);
}
