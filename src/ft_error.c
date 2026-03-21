/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 11:14:41 by alienard          #+#    #+#             */
/*   Updated: 2020/03/06 08:33:36 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	ft_error_id(t_window *win)
{
	if (win->line){
		free(win->line);
		win->line = NULL;
	}
	ft_error(5, win, "line");
}

void	ft_error_param(int amb, int res, t_window *win)
{
	res != 1 ? ft_error(res, win, "resolution lines, should be one") : 0;
	amb != 1 ? ft_error(amb, win, "ambient light lines, should be one") : 0;
}

int		ft_error(int error, t_window *win, const char *str)
{
	ft_printf("Error \n%d : ", error);
	if (error == 0)
		ft_printf("invalid number of %s.\n", str);
	if (error == 1)
		ft_printf("av[1] not a %s file.\n", str);
	if (error == 2)
		ft_printf("failed to %s.\n", str);
	if (error == 3)
		ft_printf("non digit parameters in %s line.\n", str);
	if (error == 4)
		ft_printf("too much parameters in %s line.\n", str);
	if (error == 5)
		ft_printf("invalid %s id.\n", str);
	if (error == 6)
		ft_printf("malloc error during %s init.\n", str);
	if (error == 7)
		ft_printf("invalid %s parameters.\n", str);
	if (error == 8)
		ft_printf("invalid orientation in %s line.\n", str);
	if (error == 9)
		ft_printf("invalid colors in %s line.\n", str);
	ft_cleanup(win);
	exit(1);
	return (-1);
}
