#include "mini_rt.h"

int	ft_error(int error, t_window *win, const char *str)
{
	(void)win;
	(void)str;
	return (error);
}

void	ft_cleanup(t_window *win)
{
	(void)win;
}

int	ft_close(t_window *win)
{
	(void)win;
	return (0);
}

int	ft_free_lst_cam(t_window *win)
{
	(void)win;
	return (0);
}

int	ft_free_lst_light(t_window *win)
{
	(void)win;
	return (0);
}

int	ft_free_lst_sh(t_window *win)
{
	(void)win;
	return (0);
}

void	*mlx_xpm_file_to_image(void *mlx, char *f, int *w, int *h)
{
	(void)mlx;
	(void)f;
	(void)w;
	(void)h;
	return (NULL);
}

char	*mlx_get_data_addr(void *img, int *bpp, int *sl, int *e)
{
	(void)img;
	(void)bpp;
	(void)sl;
	(void)e;
	return (NULL);
}

int	mlx_destroy_image(void *mlx, void *img)
{
	(void)mlx;
	(void)img;
	return (0);
}
