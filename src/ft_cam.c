/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cam.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 13:54:29 by alienard          #+#    #+#             */
/*   Updated: 2020/07/20 14:11:16 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_pt ft_rotation_vect(t_pt p, t_pt ori)
{
	t_pt tmp;

	tmp = ft_rotate_arr_x(p, ori.x * M_PI);
	tmp = ft_rotate_arr_y(tmp, ori.y * M_PI);
	tmp = ft_rotate_arr_z(tmp, ori.z * M_PI);
	return (tmp);
}

t_pt ft_rot_angle(t_pt p, double a)
{
	t_pt tmp;

	tmp = ft_rotate_arr_x(p, a);
	tmp = ft_rotate_arr_y(tmp, a);
	tmp = ft_rotate_arr_z(tmp, a);
	return (tmp);
}

t_pt ft_rotate_arr_x(t_pt p, double a)
{
	t_pt tmp;

	tmp.x = p.x;
	tmp.y = cos(a) * p.y + sin(a) * p.z;
	tmp.z = -sin(a) * p.y + cos(a) * p.z;
	return (tmp);
}

t_pt ft_rotate_arr_y(t_pt p, double a)
{
	t_pt tmp;

	tmp.x = cos(a) * p.x - sin(a) * p.z;
	tmp.y = p.y;
	tmp.z = sin(a) * p.x + cos(a) * p.z;
	return (tmp);
}

t_pt ft_rotate_arr_z(t_pt p, double a)
{
	t_pt tmp;

	tmp.x = cos(a) * p.x + sin(a) * p.y;
	tmp.y = -sin(a) * p.x + cos(a) * p.y;
	tmp.z = p.z;
	return (tmp);
}
