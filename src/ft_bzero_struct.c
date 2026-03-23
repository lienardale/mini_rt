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
	return (0);
}

void ft_argb_init(t_argb *a)
{
	a->a = 0;
	a->r = 0;
	a->g = 0;
	a->b = 0;
}
