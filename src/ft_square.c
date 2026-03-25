/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_square.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 11:15:20 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:23:35 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_square_init(t_window *win, t_shape **cur, char *line)
{
	int check;

	(*cur)->id = 'q';
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "square point");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "square orientation");
	if (ft_isdigit(*line) == 1)
		(*cur)->height = ft_atof(line);
	else
		check = ft_error(7, win, "square height");
	while ((ft_isdigit(*line)) == 1 || *line == '.')
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "square color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_square_check(win, cur)
								: ft_error(4, win, "square");
	return (check == 0 ? 0 : ft_error(check, win, "square"));
}

int ft_square_check(t_window *win, t_shape **current)
{
	int check;

	check = 0;
	if (!current || !(*current))
		return (ft_error(17, win, ""));
	if ((*current)->height < 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*current)->pt_0);
	check = (check == 0) ? ft_ori_check(win, (*current)->ori) : check;
	check = (check == 0) ? ft_color_check(win, (*current)->color) : check;
	return (check);
}

void ft_square_norm(t_shape *sh, t_ray *ray)
{
	ray->hit_n = sh->ori;
}

void ft_intersect_ray_square(t_shape *sh, t_ray *ray)
{
	double d;
	double t;
	t_pt r;

	ray->hit_n = sh->ori;
	t = -(ft_dot_product(ray->hit_n, ray->orig) + sh->plane_d) /
		ft_dot_product(ray->hit_n, ray->dir);
	if (t < 0.0001)
	{
		ray->lenght = -1;
		return;
	}
	r = ft_addition(ray->orig, ft_multi_scal(t, ray->dir));
	if ((d = ft_is_in_square(ray, sh, r)) == 0)
	{
		ray->lenght = t;
		ft_square_norm(sh, ray);
		if (ft_dot_product(ft_subtraction(sh->pt_0, ray->orig), ray->hit_n) >
			0.001)
			ft_inv_norm(&ray->hit_n);
	}
	else
		ray->lenght = -1;
}

double ft_is_in_square(t_ray *ray, t_shape *sh, t_pt r)
{
	t_pt tmp;
	double h;

	(void)ray;
	h = sh->height / 2;
	tmp.x = fabs(r.x - sh->pt_0.x);
	tmp.y = fabs(r.y - sh->pt_0.y);
	tmp.z = fabs(r.z - sh->pt_0.z);
	if (tmp.x > h || tmp.y > h || tmp.z > h)
		return (-1);
	return (0);
}
