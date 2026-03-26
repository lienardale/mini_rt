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
	win->img_ptr = mlx_new_image(win->mlx_ptr, (int)win->x, (int)win->y);
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
	ft_draw_hud(win);
	return (0);
}

/* Parse the scene file line by line, dispatching each element to its
 * initializer. */
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
		else if (win->line[0] == 'E' &&
				 ((ft_isspace(win->line[1])) == 1 || win->line[1] == '\0'))
			win->use_env_sky = 1;
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

/* Register all event hooks on the current window. */
void ft_register_hooks(t_window *win)
{
	mlx_do_key_autorepeatoff(win->mlx_ptr);
	mlx_hook(win->win_ptr, 2, 1L << 0, &ft_key_press, win);
	mlx_hook(win->win_ptr, 3, 1L << 1, &ft_key_release, win);
	mlx_mouse_hook(win->win_ptr, &ft_mouse, win);
	mlx_hook(win->win_ptr, 5, 1L << 3, &ft_mouse_release, win);
	mlx_hook(win->win_ptr, 6, 1L << 6, &ft_mouse_move, win);
	mlx_hook(win->win_ptr, 17, 0, &ft_close, win);
	mlx_loop_hook(win->mlx_ptr, &ft_frame_update, win);
}

/* Initialize mlx hooks, handle -save flag, and start the event loop. */
void ft_mlx_init(t_window *win, int ac, char **av)
{
	int ret;

	ft_register_hooks(win);
	if (ft_has_save_flag(ac, av))
	{
		ret = ft_save(win);
		ret == 0 ? ft_close(win) : ft_error(2, win, "save");
	}
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

/* Parse a flag with a numeric argument, return the value or 0 */
static int ft_parse_int_flag(char **av, int ac, int *idx, const char *flag,
							 size_t len)
{
	if (ft_strncmp(flag, av[*idx], len) == 0 && *idx + 1 < ac)
		return (ft_atoi(av[++(*idx)]));
	return (0);
}

/* Parse --width, --height, --output, --verbose, --pathtrace, --pt-bounces,
 * --motion-samples flags. */
static void ft_parse_flags(t_window *win, int ac, char **av)
{
	int i;
	int val;

	i = 2;
	while (i < ac)
	{
		if (ft_strncmp("--verbose", av[i], 10) == 0 ||
			ft_strncmp("-v", av[i], 3) == 0)
			win->verbose = 1;
		else if (ft_strncmp("--width", av[i], 8) == 0 && i + 1 < ac)
			win->orig_x = (unsigned int)ft_atoi(av[++i]);
		else if (ft_strncmp("--height", av[i], 9) == 0 && i + 1 < ac)
			win->orig_y = (unsigned int)ft_atoi(av[++i]);
		else if (ft_strncmp("--output", av[i], 9) == 0 && i + 1 < ac)
			win->output_path = av[++i];
		else if ((val = ft_parse_int_flag(av, ac, &i, "--pathtrace", 12)))
			win->path_trace_spp = val;
		else if ((val = ft_parse_int_flag(av, ac, &i, "--pt-bounces", 13)))
			win->path_trace_bounces = val;
		else if ((val = ft_parse_int_flag(av, ac, &i, "--motion-samples", 17)))
			win->motion_blur_samples = val;
		i++;
	}
}

/* Print scene information when --verbose is set. */
static void ft_print_verbose(t_window *win)
{
	int shapes;
	int lights;
	t_shape *sh;
	t_light *lt;

	shapes = 0;
	lights = 0;
	sh = win->beg_sh;
	while (sh)
	{
		shapes++;
		sh = sh->next;
	}
	lt = win->beg_light;
	while (lt)
	{
		lights++;
		lt = lt->next;
	}
	ft_printf("miniRT: %ux%u, %d shapes, %d lights, %d threads\n", win->x,
			  win->y, shapes, lights, win->num_threads);
}

/* Return 1 if the argument is a recognized command-line flag. */
static int ft_valid_arg(char *arg)
{
	if (ft_strncmp("-save", arg, 6) == 0)
		return (1);
	if (ft_strncmp("--threads", arg, 9) == 0)
		return (1);
	if (ft_strncmp("--verbose", arg, 10) == 0)
		return (1);
	if (ft_strncmp("-v", arg, 3) == 0)
		return (1);
	if (ft_strncmp("--width", arg, 8) == 0)
		return (1);
	if (ft_strncmp("--height", arg, 9) == 0)
		return (1);
	if (ft_strncmp("--output", arg, 9) == 0)
		return (1);
	if (ft_strncmp("--pathtrace", arg, 12) == 0)
		return (1);
	if (ft_strncmp("--pt-bounces", arg, 13) == 0)
		return (1);
	if (ft_strncmp("--motion-samples", arg, 17) == 0)
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
			if (ft_strncmp("--threads", av[arg_i], 10) == 0 ||
				ft_strncmp("--width", av[arg_i], 8) == 0 ||
				ft_strncmp("--height", av[arg_i], 9) == 0 ||
				ft_strncmp("--output", av[arg_i], 9) == 0 ||
				ft_strncmp("--pathtrace", av[arg_i], 12) == 0 ||
				ft_strncmp("--pt-bounces", av[arg_i], 13) == 0 ||
				ft_strncmp("--motion-samples", av[arg_i], 17) == 0)
				arg_i++;
			arg_i++;
		}
	}
	ft_parse_threads(&win, ac, av);
	ft_parse_flags(&win, ac, av);
	if ((win.fd = open(av[1], O_RDONLY)) < 0)
		return (ft_error(2, &win, "open"));
	ft_parse(&check, &win, win.fd);
	(check != 0) ? ft_close(&win) : check;
	if (close(win.fd) < 0)
		return (ft_error(2, &win, "close"));
	ft_check_parsing(&win);
	if (win.orig_x > 0)
		win.x = win.orig_x;
	if (win.orig_y > 0)
		win.y = win.orig_y;
	if (win.orig_x == 0)
		win.orig_x = win.x;
	if (win.orig_y == 0)
		win.orig_y = win.y;
	if (win.verbose)
		ft_print_verbose(&win);
	ft_precompute_shapes(&win);
	ft_build_scene_bvh(&win);
	if (win.motion_blur_samples == 0 && ft_scene_has_motion(&win))
		win.motion_blur_samples = MOTION_BLUR_SAMPLES;
	win.cur_cam = win.beg_cam;
	win.ac = ac;
	win.av = av;
	if (!(win.mlx_ptr = mlx_init()))
		return (check = ft_error(2, &win, "initialize mlx"));
	win.win_ptr = mlx_new_window(win.mlx_ptr, (int)win.x, (int)win.y, "miniRT");
	check = ft_aff(&win);
	if (check == -1)
		ft_close(&win);
	else
		ft_mlx_init(&win, ac, av);
	return (0);
}
