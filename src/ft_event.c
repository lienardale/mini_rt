/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 14:52:09 by alienard          #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Increase or decrease the current camera's field of view. */
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

/* Handle key press events: update held keys, FOV, resolution, and camera
 * switching. */
int ft_key_press(int keycode, t_window *param)
{
	unsigned int bit;

	if (keycode == ESC)
		ft_close(param);
	bit = ft_keycode_to_bit(keycode);
	if (bit)
	{
		param->keys_held |= bit;
		return (0);
	}
	ft_key_fov(keycode, param);
	if (keycode == CHG_RES)
	{
		if (param->resol == 4)
		{
			param->resol = 1;
			ft_aff(param);
			return (0);
		}
		param->resol = 4;
	}
	if (keycode == NXT_CAM)
	{
		if (param->cur_cam->next)
			param->cur_cam = param->cur_cam->next;
		else
			param->cur_cam = param->beg_cam;
	}
	param->needs_render = 1;
	return (0);
}

/* Handle key release events by clearing the corresponding held-key bit. */
int ft_key_release(int keycode, t_window *param)
{
	unsigned int bit;

	bit = ft_keycode_to_bit(keycode);
	if (bit)
		param->keys_held &= ~bit;
	return (0);
}

/* Per-frame callback: apply movement/look, render at low then full resolution.
 */
int ft_frame_update(t_window *win)
{
	if (win->keys_held == 0 && !win->needs_render)
		return (0);
	if (win->keys_held)
	{
		ft_cam_apply_movement(win->cur_cam, win->keys_held);
		ft_cam_apply_look(win->cur_cam, win->keys_held);
	}
	win->resol = 4;
	ft_aff(win);
	if (win->keys_held == 0)
	{
		win->resol = 1;
		ft_aff(win);
		win->needs_render = 0;
	}
	return (0);
}

/* Handle mouse button events (currently unused). */
int ft_mouse(int button, int x, int y, t_window *param)
{
	(void)button;
	(void)x;
	(void)y;
	(void)param;
	return (0);
}
