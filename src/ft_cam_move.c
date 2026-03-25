/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cam_move.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/24 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Map a keycode to its corresponding bitmask for held-key tracking. */
unsigned int ft_keycode_to_bit(int keycode)
{
	if (keycode == MV_FWD)
		return (KEY_BIT_FWD);
	if (keycode == MV_BKW)
		return (KEY_BIT_BKW);
	if (keycode == MV_L)
		return (KEY_BIT_LEFT);
	if (keycode == MV_R)
		return (KEY_BIT_RIGHT);
	if (keycode == MV_UP)
		return (KEY_BIT_UP);
	if (keycode == MV_DWN)
		return (KEY_BIT_DOWN);
	if (keycode == LK_LFT)
		return (KEY_BIT_LK_L);
	if (keycode == LK_RGT)
		return (KEY_BIT_LK_R);
	if (keycode == LK_UP)
		return (KEY_BIT_LK_U);
	if (keycode == LK_DWN)
		return (KEY_BIT_LK_D);
	if (keycode == LK_ZL)
		return (KEY_BIT_LK_ZL);
	if (keycode == LK_ZR)
		return (KEY_BIT_LK_ZR);
	return (0);
}

/* Compute the camera's forward direction projected onto the XZ plane. */
t_pt ft_cam_forward_xz(t_cam *cam)
{
	double yaw;
	t_pt fwd;

	yaw = cam->ori.y * M_PI;
	fwd.x = -sin(yaw);
	fwd.y = 0;
	fwd.z = -cos(yaw);
	return (fwd);
}

/* Compute the camera's right direction projected onto the XZ plane. */
t_pt ft_cam_right_xz(t_cam *cam)
{
	double yaw;
	t_pt right;

	yaw = cam->ori.y * M_PI;
	right.x = cos(yaw);
	right.y = 0;
	right.z = -sin(yaw);
	return (right);
}

/* Translate the camera position based on currently held movement keys. */
void ft_cam_apply_movement(t_cam *cam, unsigned int keys_held)
{
	t_pt fwd;
	t_pt right;

	fwd = ft_cam_forward_xz(cam);
	right = ft_cam_right_xz(cam);
	if (keys_held & KEY_BIT_FWD)
		cam->coord = ft_addition(cam->coord, ft_multi_scal(MOVE_STEP, fwd));
	if (keys_held & KEY_BIT_BKW)
		cam->coord = ft_subtraction(cam->coord, ft_multi_scal(MOVE_STEP, fwd));
	if (keys_held & KEY_BIT_RIGHT)
		cam->coord = ft_addition(cam->coord, ft_multi_scal(MOVE_STEP, right));
	if (keys_held & KEY_BIT_LEFT)
		cam->coord =
			ft_subtraction(cam->coord, ft_multi_scal(MOVE_STEP, right));
	if (keys_held & KEY_BIT_UP)
		cam->coord.y += MOVE_STEP;
	if (keys_held & KEY_BIT_DOWN)
		cam->coord.y -= MOVE_STEP;
}

/* Adjust the camera orientation angles based on currently held look keys. */
void ft_cam_apply_look(t_cam *cam, unsigned int keys_held)
{
	if (keys_held & KEY_BIT_LK_L)
		cam->ori.y -= LOOK_STEP;
	if (keys_held & KEY_BIT_LK_R)
		cam->ori.y += LOOK_STEP;
	if (keys_held & KEY_BIT_LK_U)
		cam->ori.x -= LOOK_STEP;
	if (keys_held & KEY_BIT_LK_D)
		cam->ori.x += LOOK_STEP;
	if (keys_held & KEY_BIT_LK_ZL)
		cam->ori.z -= LOOK_STEP;
	if (keys_held & KEY_BIT_LK_ZR)
		cam->ori.z += LOOK_STEP;
}
