/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero_struct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 20:35:36 by alienard          #+#    #+#             */
/*   Updated: 2020/03/03 11:43:07 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Zero-initialize all fields of the main window/scene struct. */
int ft_window_init(t_window *win)
{
	win->fd = -1;
	win->line = NULL;
	win->beg_sh = NULL;
	win->beg_cam = NULL;
	win->beg_light = NULL;
	win->mlx_ptr = NULL;
	win->win_ptr = NULL;
	win->img_ptr = NULL;
	win->data = NULL;
	win->endian = -1;
	win->x = 0;
	win->y = 0;
	win->ratio = -1;
	win->resol = RESOL;
	win->bvh = NULL;
	win->num_threads = 1;
	win->keys_held = 0;
	win->needs_render = 1;
	win->verbose = 0;
	win->output_path = NULL;
	win->orig_x = 0;
	win->orig_y = 0;
	win->progressive_stage = -1;
	win->mouse_pressed = 0;
	win->mouse_last_x = 0;
	win->mouse_last_y = 0;
	win->show_stats = 0;
	win->show_help = 0;
	win->last_render_ms = 0.0;
	win->ac = 0;
	win->av = NULL;
	win->path_trace_spp = 0;
	win->path_trace_bounces = PT_DEFAULT_BOUNCES;
	win->motion_blur_samples = 0;
	return (0);
}

/* Zero-initialize an ARGB color struct. */
void ft_argb_init(t_argb *a)
{
	a->a = 0;
	a->r = 0;
	a->g = 0;
	a->b = 0;
}
