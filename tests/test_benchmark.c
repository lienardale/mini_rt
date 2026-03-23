/* ************************************************************************** */
/*                                                                            */
/*   test_benchmark.c - Multithreading performance benchmark                  */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include <sys/time.h>

static int get_num_cores(void)
{
	long n;

	n = sysconf(_SC_NPROCESSORS_ONLN);
	if (n < 1)
		n = 1;
	if (n > MAX_THREADS)
		n = MAX_THREADS;
	return ((int)n);
}

static long get_time_us(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000L + tv.tv_usec);
}

static t_shape *mk_sphere(double x, double y, double z, double d, int r, int g,
						  int b)
{
	t_shape *sh;

	sh = ft_calloc(1, sizeof(t_shape));
	sh->id = 's';
	sh->pt_0 = (t_pt){x, y, z};
	sh->diameter = d;
	sh->color = (t_argb){0, r, g, b};
	sh->next = NULL;
	ft_precompute_shape(sh);
	return (sh);
}

static t_light *mk_light(double x, double y, double z, double ratio)
{
	t_light *l;

	l = ft_calloc(1, sizeof(t_light));
	l->coord = (t_pt){x, y, z};
	l->light_ratio = ratio;
	l->col = (t_argb){0, 255, 255, 255};
	l->next = NULL;
	return (l);
}

static void build_scene(t_window *win)
{
	t_shape *sh;
	t_shape *prev;
	t_light *l;
	t_cam *cam;
	int i;

	win->x = 1920;
	win->y = 1080;
	win->resol = 1;
	win->ratio = 0.1;
	win->col = (t_argb){0, 255, 255, 255};
	cam = ft_calloc(1, sizeof(t_cam));
	cam->coord = (t_pt){0, 20, -60};
	cam->ori = (t_pt){0, -0.3, 1};
	cam->fov = 70;
	cam->next = NULL;
	win->beg_cam = cam;
	win->cur_cam = cam;
	l = mk_light(-30, 40, -20, 0.8);
	win->beg_light = l;
	l->next = mk_light(30, 40, -20, 0.6);
	l->next->next = mk_light(0, 50, 0, 0.5);
	prev = NULL;
	i = 0;
	while (i < 25)
	{
		sh = mk_sphere(-30 + (i % 5) * 15, 3 + (i / 5) * 2, -20 + (i / 5) * 10,
					   6 + (i % 3) * 2, 50 + i * 8, 255 - i * 8, 100 + i * 5);
		if (prev)
			prev->next = sh;
		else
			win->beg_sh = sh;
		prev = sh;
		i++;
	}
	ft_build_scene_bvh(win);
}

static void free_scene(t_window *win)
{
	t_shape *sh;
	t_shape *tmp_sh;
	t_light *l;
	t_light *tmp_l;

	sh = win->beg_sh;
	while (sh)
	{
		tmp_sh = sh->next;
		free(sh);
		sh = tmp_sh;
	}
	l = win->beg_light;
	while (l)
	{
		tmp_l = l->next;
		free(l);
		l = tmp_l;
	}
	if (win->bvh)
		ft_bvh_free(win->bvh);
	free(win->beg_cam);
	free(win->data);
}

static void render_single(t_window *win)
{
	double i;
	double j;
	t_cam cam_copy;

	cam_copy = *(win->cur_cam);
	j = 0;
	while (j < win->y)
	{
		i = 0;
		while (i < win->x)
		{
			ft_ray(i, j, win, &cam_copy);
			i += win->resol;
		}
		j += win->resol;
	}
}

static void *thread_worker(void *arg)
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

static void render_mt(t_window *win, int nthreads)
{
	pthread_t threads[MAX_THREADS];
	t_thread_data td[MAX_THREADS];
	int n;
	int band_height;
	int y_pos;

	band_height = (win->y + nthreads - 1) / nthreads;
	band_height = ((band_height + win->resol - 1) / win->resol) * win->resol;
	y_pos = 0;
	n = 0;
	while (n < nthreads && y_pos < (int)win->y)
	{
		td[n].win = win;
		td[n].cam = win->cur_cam;
		td[n].y_start = y_pos;
		td[n].y_end = y_pos + band_height;
		if (td[n].y_end > (int)win->y)
			td[n].y_end = (int)win->y;
		pthread_create(&threads[n], NULL, thread_worker, &td[n]);
		y_pos += band_height;
		n++;
	}
	while (--n >= 0)
		pthread_join(threads[n], NULL);
}

static long bench(t_window *win, int nthreads, int runs)
{
	long total;
	long start;
	int r;

	total = 0;
	r = 0;
	while (r < runs)
	{
		start = get_time_us();
		if (nthreads <= 1)
			render_single(win);
		else
			render_mt(win, nthreads);
		total += get_time_us() - start;
		r++;
	}
	return (total / runs);
}

int main(void)
{
	t_window win;
	int runs;
	long t1;
	long t2;
	long t4;
	int cores;

	runs = 3;
	cores = get_num_cores();
	ft_window_init(&win);
	build_scene(&win);
	win.data = malloc(4 * win.x * win.y);
	if (!win.data)
		return (1);
	memset(win.data, 0, 4 * win.x * win.y);
	printf("=== miniRT Multithreading Benchmark ===\n");
	printf("Resolution: %ux%u | 25 spheres, 3 lights\n", win.x, win.y);
	printf("Runs: %d | CPU cores: %d\n\n", runs, cores);
	render_single(&win);
	t1 = bench(&win, 1, runs);
	t2 = bench(&win, 2, runs);
	t4 = bench(&win, 4, runs);
	printf("%-10s %-15s %-10s\n", "Threads", "Avg Time", "Speedup");
	printf("%-10s %-15s %-10s\n", "-------", "--------", "-------");
	printf("%-10d %ld.%03ldms         1.00x\n", 1, t1 / 1000, t1 % 1000);
	if (t2 > 0)
		printf("%-10d %ld.%03ldms         %.2fx\n", 2, t2 / 1000, t2 % 1000,
			   (double)t1 / t2);
	if (t4 > 0)
		printf("%-10d %ld.%03ldms         %.2fx\n", 4, t4 / 1000, t4 % 1000,
			   (double)t1 / t4);
	printf("\n");
	free_scene(&win);
	return (0);
}
