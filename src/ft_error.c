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

/* Report an invalid identifier error for the current line. */
void ft_error_id(t_window *win)
{
	if (win->line)
	{
		free(win->line);
		win->line = NULL;
	}
	ft_error(5, win, "line");
}

/* Verify that exactly one resolution and one ambient light line were parsed. */
void ft_error_param(int amb, int res, t_window *win)
{
	if (res != 1)
		ft_error(res, win, "resolution lines, should be one");
	if (amb != 1)
		ft_error(amb, win, "ambient light lines, should be one");
}

static const char *g_error_msgs[] = {
	"invalid number of %s",
	"av[1] not a %s file",
	"failed to %s",
	"non-digit parameters in %s line",
	"too many parameters in %s line",
	"invalid %s identifier",
	"memory allocation failed during %s initialization",
	"invalid %s parameters",
	"invalid orientation vector in %s line",
	"invalid color values in %s line"
};

/* Print a formatted error message, clean up resources, and exit. */
int ft_error(int error, t_window *win, const char *str)
{
	ft_printf("Error\n  [%d] ", error);
	if (error >= 0 && error <= 9)
		ft_printf(g_error_msgs[error], str);
	else
		ft_printf("unknown error in %s", str);
	ft_printf(".\n");
	ft_cleanup(win);
	exit(1);
	return (-1);
}
