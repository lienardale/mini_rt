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

/* Render the scene by casting a ray for each pixel and displaying the image. */
int ft_aff(t_window *win)
{
	double i;
	double j;
	t_cam *cam;

	if (win->num_threads > 1)
		return (ft_aff_threaded(win));
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
	return (0);
}

/* Parse the scene file line by line, dispatching each element to its initializer. */
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

/* Check if the -save flag is present among command-line arguments. */
static int ft_has_save_flag(int ac, char **av)
{
	int i;

	i = 2;
	while (i < ac)
	{
		if (ft_strncmp("-save", av[i], 6) == 0)
			return (1);
		i++;
	}
	return (0);
}

/* Initialize mlx hooks, handle -save flag, and start the event loop. */
void ft_mlx_init(t_window *win, int ac, char **av)
{
	int ret;

	mlx_do_key_autorepeatoff(win->mlx_ptr);
	mlx_hook(win->win_ptr, 2, 1L << 0, &ft_key_press, win);
	mlx_hook(win->win_ptr, 3, 1L << 1, &ft_key_release, win);
	mlx_mouse_hook(win->win_ptr, &ft_mouse, win);
	if (ft_has_save_flag(ac, av))
	{
		ret = ft_save(win);
		ret == 0 ? ft_close(win) : ft_error(2, win, "save");
	}
	mlx_hook(win->win_ptr, 17, 0, &ft_close, win);
	mlx_loop_hook(win->mlx_ptr, &ft_frame_update, win);
	mlx_loop(win->mlx_ptr);
}

/* Parse the --threads argument or default to auto-detected core count. */
static void ft_parse_threads(t_window *win, int ac, char **av)
{
	int i;

	i = 2;
	while (i < ac)
	{
		if (ft_strncmp("--threads", av[i], 10) == 0 && i + 1 < ac)
		{
			win->num_threads = ft_atoi(av[i + 1]);
			if (win->num_threads < 1)
				win->num_threads = 1;
			if (win->num_threads > MAX_THREADS)
				win->num_threads = MAX_THREADS;
			return;
		}
		else if (ft_strncmp("--threads=auto", av[i], 15) == 0)
		{
			win->num_threads = ft_get_num_cores();
			return;
		}
		i++;
	}
	win->num_threads = ft_get_num_cores();
}

/* Return 1 if the argument is a recognized command-line flag. */
static int ft_valid_arg(char *arg)
{
	if (ft_strncmp("-save", arg, 6) == 0)
		return (1);
	if (ft_strncmp("--threads", arg, 9) == 0)
		return (1);
	return (0);
}

/* Entry point: validate args, parse scene file, init mlx, and render. */
int main(int ac, char **av)
{
	t_window win;
	int check;

	ft_window_init(&win);
	if (ac < 2 || !av[1] || (ft_strrchr(av[1], '.')) == NULL)
		return (ft_error(0, &win, "arguments"));
	if (ft_strncmp("rt\0", ft_strrchr(av[1], '.') + 1, 3) != 0)
		ft_error(1, &win, ".rt");
	{
		int arg_i;

		arg_i = 2;
		while (arg_i < ac)
		{
			if (!ft_valid_arg(av[arg_i]))
				ft_error(0, &win, "arguments");
			if (ft_strncmp("--threads", av[arg_i], 10) == 0)
				arg_i++;
			arg_i++;
		}
	}
	ft_parse_threads(&win, ac, av);
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
