/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 14:52:09 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 19:41:09 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void ft_key_moove(int keycode, t_window *param)
{
	if (keycode == MV_R)
		param->cur_cam->coord.x -= 5;
	if (keycode == MV_L)
		param->cur_cam->coord.x += 5;
	if (keycode == MV_BKW)
		param->cur_cam->coord.z += 5;
	if (keycode == MV_FWD)
		param->cur_cam->coord.z -= 5;
	if (keycode == MV_UP)
		param->cur_cam->coord.y += 5;
	if (keycode == MV_DWN)
		param->cur_cam->coord.y -= 5;
}

void ft_key_look(int keycode, t_window *param)
{
	if (keycode == LK_LFT)
		param->cur_cam->ori.y -= 0.05;
	if (keycode == LK_RGT)
		param->cur_cam->ori.y += 0.05;
	if (keycode == LK_UP)
		param->cur_cam->ori.x -= 0.05;
	if (keycode == LK_DWN)
		param->cur_cam->ori.x += 0.05;
	if (keycode == LK_ZL)
		param->cur_cam->ori.z -= 0.05;
	if (keycode == LK_ZR)
		param->cur_cam->ori.z += 0.05;
}

void ft_key_fov(int keycode, t_window *param)
{
	if (keycode == FOV_P)
	{
		if (param->cur_cam->fov < 180)
			param->cur_cam->fov += 5;
	}
	if (keycode == FOV_N)
	{
		if (param->cur_cam->fov > 0)
			param->cur_cam->fov -= 5;
	}
}

int ft_key(int keycode, t_window *param)
{
	if (keycode == ESC)
		ft_close(param);
	ft_key_moove(keycode, param);
	ft_key_look(keycode, param);
	ft_key_fov(keycode, param);
	if (keycode == CHG_RES)
		param->resol = (param->resol == 4) ? 1 : 4;
	if (keycode == NXT_CAM)
	{
		if (param->cur_cam->next)
			param->cur_cam = param->cur_cam->next;
		else
			param->cur_cam = param->beg_cam;
	}
	ft_aff(param);
	return (0);
}

int ft_mouse(int button, int x, int y, t_window *param)
{
	(void)button;
	(void)x;
	(void)y;
	(void)param;
	return (0);
}
