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
