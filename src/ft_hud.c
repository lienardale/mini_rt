/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hud.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Count the number of shapes in the scene linked list. */
int ft_count_shapes(t_window *win)
{
	int count;
	t_shape *sh;

	count = 0;
	sh = win->beg_sh;
	while (sh)
	{
		count++;
		sh = sh->next;
	}
	return (count);
}

/* Count the number of lights in the scene linked list. */
int ft_count_lights(t_window *win)
{
	int count;
	t_light *lt;

	count = 0;
	lt = win->beg_light;
	while (lt)
	{
		count++;
		lt = lt->next;
	}
	return (count);
}

/* Format an integer into a string buffer (simple itoa). */
static int ft_hud_itoa(char *buf, int n)
{
	int len;
	int tmp;
	int neg;

	neg = 0;
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	tmp = n;
	len = (n == 0) ? 1 : 0;
	while (tmp > 0)
	{
		len++;
		tmp /= 10;
	}
	if (neg)
		buf[0] = '-';
	buf[len + neg] = '\0';
	while (len > 0)
	{
		buf[len - 1 + neg] = (char)('0' + n % 10);
		n /= 10;
		len--;
	}
	return ((int)ft_strlen(buf));
}

/* Build and display the scene statistics overlay. */
void ft_draw_stats(t_window *win)
{
	char buf[128];
	int y;
	int len;

	y = 20;
	ft_strlcpy(buf, "Res: ", 128);
	len = (int)ft_strlen(buf);
	len += ft_hud_itoa(buf + len, (int)win->x);
	buf[len++] = 'x';
	buf[len] = '\0';
	len += ft_hud_itoa(buf + len, (int)win->y);
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	ft_strlcpy(buf, "Objects: ", 128);
	len = (int)ft_strlen(buf);
	ft_hud_itoa(buf + len, ft_count_shapes(win));
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	ft_strlcpy(buf, "Lights: ", 128);
	len = (int)ft_strlen(buf);
	ft_hud_itoa(buf + len, ft_count_lights(win));
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	ft_strlcpy(buf, "Render: ", 128);
	len = (int)ft_strlen(buf);
	len += ft_hud_itoa(buf + len, (int)win->last_render_ms);
	ft_strlcpy(buf + len, " ms", 128 - (size_t)len);
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	ft_strlcpy(buf, "Threads: ", 128);
	len = (int)ft_strlen(buf);
	ft_hud_itoa(buf + len, win->num_threads);
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	ft_strlcpy(buf, "Cam: (", 128);
	len = (int)ft_strlen(buf);
	len += ft_hud_itoa(buf + len, (int)win->cur_cam->coord.x);
	ft_strlcpy(buf + len, ", ", 128 - (size_t)len);
	len += 2;
	len += ft_hud_itoa(buf + len, (int)win->cur_cam->coord.y);
	ft_strlcpy(buf + len, ", ", 128 - (size_t)len);
	len += 2;
	len += ft_hud_itoa(buf + len, (int)win->cur_cam->coord.z);
	ft_strlcpy(buf + len, ")", 128 - (size_t)len);
	mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0xFFFFFF, buf);
	y += 18;
	if (win->cur_cam->aperture > EPSILON_ZERO)
		mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y, 0x00FFFF,
					   "DOF: ON");
	if (win->path_trace_spp > 0)
	{
		ft_strlcpy(buf, "PathTrace SPP: ", 128);
		len = (int)ft_strlen(buf);
		ft_hud_itoa(buf + len, win->path_trace_spp);
		mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y + 18, 0x00FFFF,
					   buf);
	}
	if (win->motion_blur_samples > 1)
	{
		ft_strlcpy(buf, "MotionBlur: ", 128);
		len = (int)ft_strlen(buf);
		ft_hud_itoa(buf + len, win->motion_blur_samples);
		mlx_string_put(win->mlx_ptr, win->win_ptr, 10, y + 36, 0x00FFFF,
					   buf);
	}
}

/* Display the help overlay showing all available controls. */
void ft_draw_help(t_window *win)
{
	int y;
	int x;

	x = 10;
	y = 20;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0x00FF00,
				   "=== miniRT Controls ===");
	y += 22;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "W/A/S/D        Move camera");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "Arrow keys     Look around");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "PgUp/PgDn      Move up/down");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   ", / .          Roll camera");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "Numpad +/-     Change FOV");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "Mouse drag     Rotate view");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "Scroll         Zoom in/out");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "R              Toggle resolution");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "Space          Next camera");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "1/2/3/4        Resolution presets");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "I              Toggle stats");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "H              Toggle this help");
	y += 18;
	mlx_string_put(win->mlx_ptr, win->win_ptr, x, y, 0xFFFFFF,
				   "ESC            Exit");
}

/* Destroy the current window/image and recreate at a new resolution. */
void ft_change_resolution(t_window *win, unsigned int w, unsigned int h)
{
	if (w == win->x && h == win->y)
		return;
	if (win->img_ptr)
	{
		mlx_destroy_image(win->mlx_ptr, win->img_ptr);
		win->img_ptr = NULL;
	}
	if (win->win_ptr)
	{
		mlx_destroy_window(win->mlx_ptr, win->win_ptr);
		win->win_ptr = NULL;
	}
	win->x = w;
	win->y = h;
	win->win_ptr = mlx_new_window(win->mlx_ptr, (int)w, (int)h, "miniRT");
	ft_register_hooks(win);
	win->needs_render = 1;
	win->progressive_stage = 0;
}

/* Draw HUD overlays (stats and/or help) if enabled. */
void ft_draw_hud(t_window *win)
{
	if (!win->win_ptr)
		return;
	if (win->show_stats)
		ft_draw_stats(win);
	if (win->show_help)
		ft_draw_help(win);
}
