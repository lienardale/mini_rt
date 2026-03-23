/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 09:33:32 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 15:09:15 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

int ft_color_init(t_window *win, t_argb *col, char **l)
{
	col->a = -1;
	col->r = -1;
	col->g = -1;
	col->b = -1;
	col->r = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	(**l == ',') ? (*l)++ : 0;
	col->g = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	(**l == ',') ? (*l)++ : 0;
	col->b = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	while ((ft_isspace(**l)) == 1)
		(*l)++;
	return (ft_color_check(win, *col));
}

int ft_point_init(t_window *win, t_pt *pt, char **l)
{
	pt->x = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	(**l == ',') ? (*l)++ : 0;
	pt->y = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	(**l == ',') ? (*l)++ : 0;
	pt->z = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	while ((ft_isspace(**l)) == 1)
		(*l)++;
	return (ft_pt_check(win, *pt));
}

int ft_shape_init(t_window *win, t_shape **begin, char *line)
{
	int i;
	t_shape *current;

	i = 0;
	if ((i = ft_which_id(line)) == -1)
		return (ft_error(5, win, "shape"));
	if (!(current = ft_calloc(1, sizeof(t_shape))))
		return (ft_error(6, win, "shape"));
	current->next = *begin;
	*begin = current;
	(i == 's') ? ft_sphere_init(win, &current, line) : 0;
	(i == 'p') ? ft_plane_init(win, &current, line) : 0;
	(i == 'q') ? ft_square_init(win, &current, line) : 0;
	(i == 'y') ? ft_cylinder_init(win, &current, line) : 0;
	(i == 't') ? ft_triangle_init(win, &current, line) : 0;
	return (0);
}

int ft_amb_light_init(t_window *amb, char *line)
{
	while (*line && ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1))
		line++;
	amb->ratio = (((ft_isdigit(*line)) == 1) ? ft_atof(line)
											 : ft_error(3, amb, "amb_light"));
	while (((ft_isdigit(*line)) == 1 || *line == '.') && *line)
		line++;
	while (*line && (ft_isspace(*line)) == 1)
		line++;
	((ft_isdigit(*line)) == 1) ? ft_color_init(amb, &amb->col, &line)
							   : ft_error(3, amb, "amb_light");
	return ((*line == '\0') ? ft_check_amb_light(amb)
							: ft_error(4, amb, "amb_light"));
}

int ft_resol_init(t_window *res, char *line)
{
	while (*line && ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1))
		line++;
	res->x = ((ft_isdigit(*line) == 1) ? ft_atoi(line)
									   : ft_error(3, res, "resolution"));
	while ((ft_isdigit(*line)) == 1 && *line)
		line++;
	while (*line && (ft_isspace(*line)) == 1)
		line++;
	res->y = ((ft_isdigit(*line) == 1) ? ft_atoi(line)
									   : ft_error(3, res, "resolution"));
	while (((ft_isdigit(*line)) == 1) && *line)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	return ((*line == '\0') ? ft_check_resol(res)
							: ft_error(4, res, "resolution"));
}
