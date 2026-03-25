/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 19:25:37 by alienard          #+#    #+#             */
/*   Updated: 2020/07/17 14:59:39 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Free all allocated resources: line buffer, linked lists, BVH, and mlx
 * objects. */
void ft_cleanup(t_window *win)
{
	if (win->line)
	{
		free(win->line);
		win->line = NULL;
	}
	get_next_line(win->fd, &win->line, 1);
	if (win->line)
	{
		free(win->line);
		win->line = NULL;
	}
	ft_free_lst_cam(win);
	ft_free_lst_light(win);
	if (win->bvh)
	{
		ft_bvh_free(win->bvh);
		win->bvh = NULL;
	}
	ft_free_lst_sh(win);
	if (win->mlx_ptr)
		mlx_do_key_autorepeaton(win->mlx_ptr);
	if (win->mlx_ptr && win->img_ptr)
		mlx_destroy_image(win->mlx_ptr, win->img_ptr);
	if (win->mlx_ptr && win->win_ptr)
		mlx_destroy_window(win->mlx_ptr, win->win_ptr);
#ifdef Linux
	if (win->mlx_ptr)
	{
		mlx_destroy_display(win->mlx_ptr);
		free(win->mlx_ptr);
	}
#endif
	if (win->fd >= 0)
		close(win->fd);
}

/* Clean up all resources and exit the program. */
int ft_close(t_window *win)
{
	ft_cleanup(win);
	exit(0);
	return (0);
}

/* Free the linked list of cameras. */
int ft_free_lst_cam(t_window *win)
{
	t_cam *tmp_c;

	while (win->beg_cam)
	{
		tmp_c = win->beg_cam->next;
		free(win->beg_cam);
		win->beg_cam = tmp_c;
	}
	win->beg_cam = NULL;
	return (0);
}

/* Free the linked list of lights. */
int ft_free_lst_light(t_window *win)
{
	t_light *tmp_l;

	while (win->beg_light)
	{
		tmp_l = win->beg_light->next;
		free(win->beg_light);
		win->beg_light = tmp_l;
	}
	win->beg_light = NULL;
	return (0);
}

/* Free the linked list of shapes, including their textures and bump maps. */
int ft_free_lst_sh(t_window *win)
{
	t_shape *tmp_s;

	while (win->beg_sh)
	{
		tmp_s = win->beg_sh->next;
		if (win->beg_sh->mat.texture)
			ft_texture_free(win->beg_sh->mat.texture, win->mlx_ptr);
		if (win->beg_sh->mat.bump_map)
			ft_texture_free(win->beg_sh->mat.bump_map, win->mlx_ptr);
		free(win->beg_sh);
		win->beg_sh = tmp_s;
	}
	win->beg_sh = NULL;
	return (0);
}
