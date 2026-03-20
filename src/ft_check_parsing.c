/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 11:14:29 by alienard          #+#    #+#             */
/*   Updated: 2020/03/07 10:42:14 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int	ft_check_resol(t_window *win)
{
	if (!win || !win->x || !win->y || win->x <= 0 || win->y <= 0)
	{
		return (ft_error(7, win, "resolution"));
	}
	if (win->x > 2560)
		win->x = 2560;
	if (win->y > 1440)
		win->y = 1440;
	return (0);
}

int	ft_check_amb_light(t_window *win)
{
	int check;

	if (!win)
		return (ft_error(7, win, "amb_light params, no amb_light"));
	if (!win->ratio)
		return (ft_error(7, win, "amb_light params, no ratio"));
	if (win->ratio < 0.0 || 1.0 < win->ratio)
		return (ft_error(9, win, "amb_light ratio"));
	check = ft_color_check(win, win->col);
	return ((check == 0) ? check : ft_error(check, win, "amb_light"));
}

int	ft_check_cam_parsing(t_window *win, t_cam *current)
{
	int check;

	if (!current)
		return (ft_error(7, win, "camera params, no camera"));
	if (current->fov < 0 || current->fov > 180)
		return (ft_error(7, win, "camera params, no fov"));
	check = ft_pt_check(win, current->coord);
	check = (check == 0) ? ft_ori_check(win, current->ori) : check;
	return ((check == 0) ? check : ft_error(check, win, "camera"));
}

int	ft_check_light_parsing(t_window *win, t_light *current)
{
	int check;

	if (!current)
		return (ft_error(7, win, "light params, no light"));
	if (current->light_ratio < 0.0 || current->light_ratio > 1.0)
		return (ft_error(7, win, "light params, no ratio"));
	check = ft_pt_check(win, current->coord);
	check = (check == 0) ? ft_color_check(win, current->col) : check;
	return ((check == 0) ? check : ft_error(check, win, "light"));
}

void	ft_check_parsing(t_window *win)
{
	ft_check_resol(win);
	ft_check_amb_light(win);
	ft_check_cam_parsing(win, win->beg_cam);
}
