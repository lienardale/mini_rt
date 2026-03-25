/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 21:04:12 by alienard          #+#    #+#             */
/*   Updated: 2020/03/05 16:16:01 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Identify the shape type from the line prefix and return its ID constant. */
int ft_which_id(char *line)
{
	if (line[0] == 's' && (ft_isalpha(line[1])) == 1)
	{
		if (line[1] == 'q' && (ft_isspace(line[2])) == 1)
			return (SHAPE_SQUARE);
		if (line[1] == 'p' && (ft_isspace(line[2])) == 1)
			return (SHAPE_SPHERE);
	}
	if (line[0] == 'c' && line[1] == 'y' && (ft_isspace(line[2])) == 1)
		return (SHAPE_CYLINDER);
	if (line[0] == 'c' && line[1] == 'o' && (ft_isspace(line[2])) == 1)
		return (SHAPE_CONE);
	if (line[0] == 'c' && line[1] == 's' && line[2] == 'g' &&
		(ft_isspace(line[3])) == 1)
		return (SHAPE_CSG);
	if (line[0] == 't' && line[1] == 'r' && (ft_isspace(line[2])) == 1)
		return (SHAPE_TRIANGLE);
	if (line[0] == 't' && line[1] == 'o' && (ft_isspace(line[2])) == 1)
		return (SHAPE_TORUS);
	if (line[0] == 'p' && line[1] == 'l' && (ft_isspace(line[2])) == 1)
		return (SHAPE_PLANE);
	if (line[0] == 'p' && line[1] == 'a' && (ft_isspace(line[2])) == 1)
		return (SHAPE_PARABOLOID);
	if (line[0] == 'd' && line[1] == 'k' && (ft_isspace(line[2])) == 1)
		return (SHAPE_DISK);
	if (line[0] == 'e' && line[1] == 'l' && (ft_isspace(line[2])) == 1)
		return (SHAPE_ELLIPSOID);
	if (line[0] == 'b' && line[1] == 'x' && (ft_isspace(line[2])) == 1)
		return (SHAPE_BOX);
	if (line[0] == 'h' && line[1] == 'y' && (ft_isspace(line[2])) == 1)
		return (SHAPE_HYPERBOLOID);
	if (line[0] == 'm' && line[1] == 'e' && (ft_isspace(line[2])) == 1)
		return (SHAPE_MESH);
	return (-1);
}

/* Advance the line pointer past digits, dots, commas, and trailing whitespace. */
void ft_iterate_in_line(char **line)
{
	while ((ft_isdigit(**line)) == 1 || **line == '.' || **line == ',')
		(*line)++;
	while ((ft_isspace(**line)) == 1)
		(*line)++;
}

/* Parse camera position, orientation, and FOV from the scene line. */
int ft_cam_init(t_window *win, t_cam **begin, char *line)
{
	t_cam *cur;

	if (!(cur = ft_calloc(1, sizeof(t_cam))))
		return (ft_error(6, win, "camera"));
	cur->next = *begin;
	*begin = cur;
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	if (ft_isnum(line) == 1)
		ft_point_init(win, &(*cur).coord, &line);
	else
		ft_error(3, win, "camera");
	if (ft_isnum(line) == 1)
		ft_point_init(win, &(*cur).ori, &line);
	else
		ft_error(7, win, "camera orientation");
	cur->fov = (ft_isdigit(*line) == 1) ? ft_atoi(line)
										: ft_error(7, win, "camera fov");
	while ((ft_isdigit(*line)) == 1)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	return (*line == '\0' ? ft_check_cam_parsing(win, cur)
						  : ft_error(4, win, "camera"));
}

/* Parse light position, intensity ratio, and color from the scene line. */
int ft_light_init(t_window *win, t_light **begin, char *line)
{
	t_light *cur;

	if (!(cur = ft_calloc(1, sizeof(t_light))))
		return (ft_error(6, win, "light"));
	cur->next = *begin;
	*begin = cur;
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	if (ft_isnum(line) == 1)
		ft_point_init(win, &(*cur).coord, &line);
	else
		ft_error(3, win, "light");
	(*cur).light_ratio =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "light ratio"));
	while ((ft_isdigit(*line)) == 1 || *line == '.')
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	if (ft_isdigit(*line) == 1)
		ft_color_init(win, &(*cur).col, &line);
	else
		ft_error(7, win, "light color");
	return ((*line == '\0') ? ft_check_light_parsing(win, cur)
							: ft_error(4, win, "light"));
}
