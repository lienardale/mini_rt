/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_save.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 22:05:51 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 19:51:44 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int		ft_save(t_window *rt)
{
	int				fd;
	int				ret;

	ret = 0;
	if ((fd = open("miniRT.bmp", O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
		return (-5);
	if (ft_file_header(fd, rt) == -1 || ft_info_header(fd, rt) == -1
		|| ft_pixel_data(fd, rt) == -1)
		ret = -1;
	close(fd);
	if (ret == -1)
		return (ret);
	ft_close(rt);
	return (0);
}

int		ft_file_header(int fd, t_window *rt)
{
	t_fileheader	fh;

	ft_bzero(&fh, sizeof(t_fileheader));
	fh = (t_fileheader){ 19778, 54 + rt->x * rt->y * 3, 0, 54};
	if (write(fd, &fh.id, 2) == -1 || write(fd, &fh.size, 4) == -1 ||
		write(fd, &fh.reserved, 4) == -1 || write(fd, &fh.offset, 4) == -1)
		return (-1);
	return (0);
}

int		ft_info_header(int fd, t_window *rt)
{
	t_infoheader	dib;

	ft_bzero(&dib, sizeof(t_infoheader));
	dib.headersize = sizeof(t_infoheader);
	dib.width = rt->x;
	dib.height = rt->y;
	dib.pane = 1;
	dib.bitcount = 24;
	if (
		write(fd, &dib.headersize, 4) == -1 ||
		write(fd, &dib.width, 4) == -1 ||
		write(fd, &dib.height, 4) == -1 ||
		write(fd, &dib.pane, 2) == -1 ||
		write(fd, &dib.bitcount, 2) == -1 ||
		write(fd, &dib.offset, 24) == -1)
		return (-1);
	return (0);
}

int		ft_pixel_data(int fd, t_window *win)
{
	unsigned int	i;
	unsigned int	x;
	unsigned int	y;
	unsigned char	*tab;

	tab = malloc(3 * win->x * win->y);
	if (!tab)
		return (-1);
	i = 0;
	y = 0;
	while (y < win->y)
	{
		x = 0;
		while (x < win->x)
		{
			tab[i * 3] = win->data[((win->y - y - 1) * win->x + x) * 4 + 0];
			tab[i * 3 + 1] = win->data[((win->y - y - 1) * win->x + x) * 4 + 1];
			tab[i * 3 + 2] = win->data[((win->y - y - 1) * win->x + x) * 4 + 2];
			i++;
			x++;
		}
		y++;
	}
	if (write(fd, tab, 3 * win->x * win->y) == -1)
	{
		free(tab);
		return (-1);
	}
	free(tab);
	return (0);
}
