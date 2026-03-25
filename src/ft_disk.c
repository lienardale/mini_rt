/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_disk.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse disk properties (center, orientation, diameter, color) from scene line
 */
static int ft_disk_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "disk point");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "disk orientation");
	if (ft_isdigit(*line) == 1)
		(*cur)->diameter = ft_atof(line);
	else
		check = ft_error(7, win, "disk diameter");
	while ((ft_isdigit(*line)) == 1 || *line == '.')
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "disk color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_disk_check(win, cur)
								: ft_error(4, win, "disk");
	return (check == 0 ? 0 : ft_error(check, win, "disk"));
}

/* Initialize disk shape and delegate to parser */
int ft_disk_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_DISK;
	return (ft_disk_parse(win, cur, line));
}

/* Validate disk parameters (non-negative diameter, valid point, color,
 * orientation) */
int ft_disk_check(t_window *win, t_shape **cur)
{
	int check;

	if (!cur || !(*cur))
		return (ft_error(17, win, ""));
	if ((*cur)->diameter < 0.0)
		return (ft_error(20, win, ""));
	check = ft_pt_check(win, (*cur)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*cur)->color) : check;
	check = (check == 0) ? ft_ori_check(win, (*cur)->ori) : check;
	return (check);
}

/* Ray-disk intersection: plane intersection then radius distance check */
void ft_intersect_ray_disk(t_shape *sh, t_ray *ray)
{
	double denom;
	double t;
	t_pt p;
	t_pt v;

	denom = ft_dot_product(sh->ori, ray->dir);
	if (fabs(denom) < EPSILON_ZERO)
	{
		ray->lenght = -1;
		return;
	}
	t = ft_dot_product(ft_subtraction(sh->pt_0, ray->orig), sh->ori) / denom;
	if (t < EPSILON_HIT)
	{
		ray->lenght = -1;
		return;
	}
	p = ft_addition(ray->orig, ft_multi_scal(t, ray->dir));
	v = ft_subtraction(p, sh->pt_0);
	if (ft_dot_product(v, v) > sh->radius_sq)
	{
		ray->lenght = -1;
		return;
	}
	ray->lenght = t;
	ray->hit_n = sh->ori;
	if (ft_dot_product(ray->dir, ray->hit_n) > EPSILON_NORMAL)
		ft_inv_norm(&ray->hit_n);
}
