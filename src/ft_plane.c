/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 13:57:37 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:21:55 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Parse plane properties (point, orientation, color) from scene line */
static int ft_plane_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->pt_0, &line)
								  : ft_error(7, win, "plane point");
	check = (ft_isnum(line) == 1) ? ft_point_init(win, &(*cur)->ori, &line)
								  : ft_error(7, win, "plane orientation");
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "plane color");
	ft_parse_material(win, &(*cur)->mat, &line);
	if (check == 0)
		check = (*line == '\0') ? ft_plane_check(win, cur)
								: ft_error(4, win, "plane");
	return (check == 0 ? 0 : ft_error(check, win, "plane"));
}

/* Initialize plane shape and delegate to parser */
int ft_plane_init(t_window *win, t_shape **cur, char *line)
{
	(*cur)->id = SHAPE_PLANE;
	return (ft_plane_parse(win, cur, line));
}

/* Validate plane parameters (point, color, orientation) */
int ft_plane_check(t_window *win, t_shape **current)
{
	int check;

	if (!current || !(*current))
		return (ft_error(17, win, ""));
	check = ft_pt_check(win, (*current)->pt_0);
	check = (check == 0) ? ft_color_check(win, (*current)->color) : check;
	check = (check == 0) ? ft_ori_check(win, (*current)->ori) : check;
	return (check);
}

/* Set plane surface normal from the shape orientation, rotated by PI/2 */
void ft_plane_norm(t_shape *sh, t_ray *ray)
{
	ray->hit_n = sh->ori;
	ray->hit_n = ft_rot_angle(ray->hit_n, M_PI_2);
}

/* Ray-plane intersection using dot product: t = -(N.O + d) / (N.D) */
void ft_intersect_ray_plan(t_shape *sh, t_ray *ray)
{
	double t;

	t = -(ft_dot_product(sh->ori, ray->orig) + sh->plane_d) /
		ft_dot_product(sh->ori, ray->dir);
	if (t > EPSILON_HIT)
	{
		ray->lenght = t;
		ft_plane_norm(sh, ray);
		if (ft_dot_product(ft_subtraction(sh->pt_0, ray->orig), ray->hit_n) >
			EPSILON_NORMAL)
			ft_inv_norm(&ray->hit_n);
	}
	else
		ray->lenght = -1;
}
