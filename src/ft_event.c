/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 14:52:09 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
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

/* Handle resolution preset keys (1-4). */
static void ft_key_resolution(int keycode, t_window *param)
{
	if (keycode == KEY_RES_1)
		ft_change_resolution(param, 320, 240);
	else if (keycode == KEY_RES_2)
		ft_change_resolution(param, 640, 480);
	else if (keycode == KEY_RES_3)
		ft_change_resolution(param, 1280, 720);
	else if (keycode == KEY_RES_4)
		ft_change_resolution(param, param->orig_x, param->orig_y);
}

/* Handle toggle keys (help, stats) and resolution presets. */
static void ft_key_toggles(int keycode, t_window *param)
{
	if (keycode == KEY_HELP)
	{
		param->show_help = !param->show_help;
		param->needs_render = 1;
	}
	else if (keycode == KEY_INFO)
	{
		param->show_stats = !param->show_stats;
		param->needs_render = 1;
	}
	else if (keycode == KEY_RES_1 || keycode == KEY_RES_2 ||
			 keycode == KEY_RES_3 || keycode == KEY_RES_4)
		ft_key_resolution(keycode, param);
}

/* Handle key press events: update held keys, FOV, resolution, camera
 * switching, and toggles. */
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
	ft_key_toggles(keycode, param);
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

/* Return elapsed milliseconds between two timevals. */
static double ft_elapsed_ms(struct timeval *start, struct timeval *end)
{
	return ((double)(end->tv_sec - start->tv_sec) * 1000.0 +
			(double)(end->tv_usec - start->tv_usec) / 1000.0);
}

/* Render at the given resolution and measure time. */
static void ft_timed_render(t_window *win, int resol)
{
	struct timeval t0;
	struct timeval t1;

	win->resol = resol;
	gettimeofday(&t0, NULL);
	ft_aff(win);
	gettimeofday(&t1, NULL);
	win->last_render_ms = ft_elapsed_ms(&t0, &t1);
}

/* Per-frame callback: apply movement/look, render with progressive
 * refinement (8x -> 4x -> 2x -> 1x). */
int ft_frame_update(t_window *win)
{
	if (win->keys_held == 0 && !win->needs_render && win->progressive_stage < 0)
		return (0);
	if (win->keys_held)
	{
		ft_cam_apply_movement(win->cur_cam, win->keys_held);
		ft_cam_apply_look(win->cur_cam, win->keys_held);
		ft_timed_render(win, 8);
		win->progressive_stage = 0;
		return (0);
	}
	if (win->progressive_stage == 0)
		ft_timed_render(win, 4);
	else if (win->progressive_stage == 1)
		ft_timed_render(win, 2);
	else if (win->progressive_stage == 2)
	{
		ft_timed_render(win, 1);
		win->progressive_stage = -1;
		win->needs_render = 0;
		return (0);
	}
	else
	{
		ft_timed_render(win, 1);
		win->needs_render = 0;
		win->progressive_stage = -1;
		return (0);
	}
	win->progressive_stage++;
	return (0);
}

/* Handle mouse button events: left-click for drag, scroll for zoom. */
int ft_mouse(int button, int x, int y, t_window *param)
{
	t_pt fwd;

	if (button == 1)
	{
		param->mouse_pressed = 1;
		param->mouse_last_x = x;
		param->mouse_last_y = y;
	}
	else if (button == 4 || button == 5)
	{
		fwd = ft_cam_forward_xz(param->cur_cam);
		if (button == 4)
			param->cur_cam->coord = ft_addition(param->cur_cam->coord,
												ft_multi_scal(MOVE_STEP, fwd));
		else
			param->cur_cam->coord = ft_subtraction(
				param->cur_cam->coord, ft_multi_scal(MOVE_STEP, fwd));
		param->needs_render = 1;
		param->progressive_stage = 0;
	}
	return (0);
}

/* Handle mouse button release events. */
int ft_mouse_release(int button, int x, int y, t_window *win)
{
	(void)x;
	(void)y;
	if (button == 1)
		win->mouse_pressed = 0;
	return (0);
}

/* Handle mouse motion events for click-drag camera rotation. */
int ft_mouse_move(int x, int y, t_window *win)
{
	int dx;
	int dy;

	if (!win->mouse_pressed)
		return (0);
	dx = x - win->mouse_last_x;
	dy = y - win->mouse_last_y;
	win->mouse_last_x = x;
	win->mouse_last_y = y;
	win->cur_cam->ori.y += (double)dx * MOUSE_SENSITIVITY;
	win->cur_cam->ori.x += (double)dy * MOUSE_SENSITIVITY;
	win->needs_render = 1;
	win->progressive_stage = 0;
	return (0);
}
