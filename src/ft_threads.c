/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void *ft_render_band(void *arg)
{
	t_thread_data *td;
	t_cam local_cam;
	double i;
	double j;

	td = (t_thread_data *)arg;
	local_cam = *(td->cam);
	j = td->y_start;
	while (j < td->y_end)
	{
		i = 0;
		while (i < td->win->x)
		{
			ft_ray(i, j, td->win, &local_cam);
			i += td->win->resol;
		}
		j += td->win->resol;
	}
	return (NULL);
}

int ft_aff_threaded(t_window *win)
{
	pthread_t threads[MAX_THREADS];
	t_thread_data td[MAX_THREADS];
	int n;
	int band_height;
	int y_pos;

	if (win->img_ptr)
		mlx_destroy_image(win->mlx_ptr, win->img_ptr);
	win->img_ptr = mlx_new_image(win->mlx_ptr, win->x, win->y);
	win->data = mlx_get_data_addr(win->img_ptr, &(win->depth),
								  &(win->size_line), &(win->endian));
	band_height = (win->y + win->num_threads - 1) / win->num_threads;
	band_height = ((band_height + win->resol - 1) / win->resol) * win->resol;
	y_pos = 0;
	n = 0;
	while (n < win->num_threads && y_pos < (int)win->y)
	{
		td[n].win = win;
		td[n].cam = win->cur_cam;
		td[n].y_start = y_pos;
		td[n].y_end = y_pos + band_height;
		if (td[n].y_end > (int)win->y)
			td[n].y_end = (int)win->y;
		pthread_create(&threads[n], NULL, ft_render_band, &td[n]);
		y_pos += band_height;
		n++;
	}
	while (--n >= 0)
		pthread_join(threads[n], NULL);
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, win->img_ptr, 0, 0);
	return (0);
}

int ft_get_num_cores(void)
{
	long n;

	n = sysconf(_SC_NPROCESSORS_ONLN);
	if (n < 1)
		n = 1;
	if (n > MAX_THREADS)
		n = MAX_THREADS;
	return ((int)n);
}
