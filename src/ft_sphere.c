/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sphere.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 13:56:45 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:22:44 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_sphere_init(t_window *win, t_shape **cur, char *line)
{
	int check;

	(*cur)->id = 's';
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "sphere point");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "sphere diameter");
	while ((ft_isdigit(*line)) == 1 || *line == '.')
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "sphere color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_sphere_check(win, cur)
								: ft_error(4, win, "sphere");
	return (check == 0 ? 0 : ft_error(check, win, "sphere"));
}

int ft_sphere_check(t_window *win, t_shape **current)
{
	int check;

	if (!current || !(*current))
		return (ft_error(17, win, ""));
	if ((*current)->diameter < 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*current)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*current)->color) : check;
	return (check);
}

void ft_sphere_norm(t_shape *sh, t_ray *ray)
{
	t_pt r;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	r = ft_normal_vect(r);
	ray->hit_n = r;
}

void ft_intersect_ray_sphere(t_shape *sh, t_ray *ray)
{
	double b;
	double c;
	t_pt calc;
	t_pt oc;

	sh->in = 0;
	oc = ft_subtraction(ray->orig, sh->pt_0);
	if (ft_dot_product(oc, oc) < sh->radius_sq)
		ft_multi_scal(-1, oc);
	b = 2 * ft_dot_product(oc, ray->dir);
	c = ft_dot_product(oc, oc) - sh->radius_sq;
	calc.x = (b * b) - (4 * c);
	if (calc.x < 0 && (ray->lenght = -1))
		return;
	calc.y = (-b - sqrt(calc.x)) / 2;
	calc.z = (-b + sqrt(calc.x)) / 2;
	if (calc.z > 0.0001 && calc.z < calc.y)
		calc.y = calc.z;
	else if (calc.z > 0.0001 && calc.y < 0.0001)
		calc.y = calc.z;
	ray->lenght = calc.y;
	ft_sphere_norm(sh, ray);
	if (ft_dot_product(ray->dir, ray->hit_n) > 0.001)
		ft_inv_norm(&ray->hit_n);
}
