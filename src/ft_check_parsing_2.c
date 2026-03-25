/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_parsing_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 21:38:06 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 16:25:27 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Validate a 3D point (currently accepts all values). */
int ft_pt_check(t_window *win, t_pt pt)
{
	pt.x = pt.x;
	(void)win;
	return (0);
}

/* Validate that orientation vector components are within [-1, 1]. */
int ft_ori_check(t_window *win, t_pt ori)
{
	(void)win;
	if (-1 > ori.x || ori.x > 1)
		return (8);
	if (ori.y < -1 || ori.y > 1)
		return (8);
	if (ori.z < -1 || ori.z > 1)
		return (8);
	return (0);
}

/* Validate that RGB color channels are within [0, 255]. */
int ft_color_check(t_window *win, t_argb color)
{
	(void)win;
	if (0 > color.r || color.r > 255)
		return (9);
	if (0 > color.g || color.g > 255)
		return (9);
	if (0 > color.b || color.b > 255)
		return (9);
	return (0);
}

/* Parse the resolution line and increment the resolution counter. */
void ft_parse_resol(int *res, t_window *win, char *line)
{
	ft_resol_init(win, line);
	(*res)++;
}

/* Parse the ambient light line and increment the ambient counter. */
void ft_parse_amb(int *amb, t_window *win, char *line)
{
	ft_amb_light_init(win, line);
	(*amb)++;
}
