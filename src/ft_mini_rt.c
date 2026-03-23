/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_rt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/20 19:17:00 by alienard          #+#    #+#             */
/*   Updated: 2020/07/29 14:36:15 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_aff(t_window *win)
{
	double i;
	double j;
	t_cam *cam;

	if (win->img_ptr)
		mlx_destroy_image(win->mlx_ptr, win->img_ptr);
	win->img_ptr = mlx_new_image(win->mlx_ptr, win->x, win->y);
	win->data = mlx_get_data_addr(win->img_ptr, &(win->depth),
								  &(win->size_line), &(win->endian));
	cam = win->cur_cam;
	j = 0;
	while (j != win->y)
	{
		i = 0;
		while (i != win->x)
		{
			ft_ray(i, j, win, cam);
			i += win->resol;
		}
		j += win->resol;
	}
	mlx_put_image_to_window(win->mlx_ptr, win->win_ptr, win->img_ptr, 0, 0);
	// return (ft_error(2, win, "put img to window"));
	return (0);
}

void ft_parse(int *check, t_window *win, int fd)
{
	int res;
	int amb;

	res = 0;
	amb = 0;
	while ((*check = get_next_line(fd, &win->line, 0)) >= 0)
	{
		if (win->line[0] == 'R' && (ft_isspace(win->line[1])) == 1)
			ft_parse_resol(&res, win, win->line);
		else if (win->line[0] == 'A' && (ft_isspace(win->line[1])) == 1)
			ft_parse_amb(&amb, win, win->line);
		else if (win->line[0] == 'c' && (ft_isspace(win->line[1])) == 1)
			ft_cam_init(win, &win->beg_cam, win->line);
		else if (win->line[0] == 'l' && (ft_isspace(win->line[1])) == 1)
			ft_light_init(win, &win->beg_light, win->line);
		else if ((ft_isalpha(win->line[0])) == 1)
			ft_shape_init(win, &win->beg_sh, win->line);
		else if (win->line[0] != '\0')
			ft_error_id(win);
		if (win->line)
		{
			free(win->line);
			win->line = NULL;
		}
		if (*check == 0)
			break;
	}
	ft_error_param(amb, res, win);
}

void ft_mlx_init(t_window *win, int ac, char **av)
{
	int ret;

	mlx_key_hook(win->win_ptr, &ft_key, win);
	mlx_mouse_hook(win->win_ptr, &ft_mouse, win);
	if (ac == 3 && ft_strncmp("-save", av[2], ft_strlen(av[2])) == 0)
	{
		ret = ft_save(win);
		ret == 0 ? ft_close(win) : ft_error(2, win, "save");
	}
	mlx_hook(win->win_ptr, 17, 0, &ft_close, win);
	mlx_loop(win->mlx_ptr);
}

int main(int ac, char **av)
{
	t_window win;
	int check;

	ft_window_init(&win);
	if (ac > 3 || !av[1] || (ft_strrchr(av[1], '.')) == NULL)
		return (ft_error(0, &win, "arguments"));
	if (ft_strncmp("rt\0", ft_strrchr(av[1], '.') + 1, 3) != 0)
		ft_error(1, &win, ".rt");
	if (ac == 3 && ft_strncmp("-save", av[2], ft_strlen(av[2])) != 0)
		ft_error(0, &win, "arguments");
	if ((win.fd = open(av[1], O_RDONLY)) < 0)
		return (ft_error(2, &win, "open"));
	ft_parse(&check, &win, win.fd);
	(check != 0) ? ft_close(&win) : check;
	if (close(win.fd) < 0)
		return (ft_error(2, &win, "close"));
	ft_check_parsing(&win);
	ft_precompute_shapes(&win);
	ft_build_scene_bvh(&win);
	win.cur_cam = win.beg_cam;
	if (!(win.mlx_ptr = mlx_init()))
		return (check = ft_error(2, &win, "initialize mlx"));
	win.win_ptr = mlx_new_window(win.mlx_ptr, win.x, win.y, "miniRT");
	check = ft_aff(&win);
	(check == -1) ? ft_close(&win) : ft_mlx_init(&win, ac, av);
	return (0);
}
