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

int ft_which_id(char *line)
{
	if (line[0] == 's' && (ft_isalpha(line[1])) == 1)
	{
		if (line[1] == 'q' && (ft_isspace(line[2])) == 1)
			return ('q');
		if (line[1] == 'p' && (ft_isspace(line[2])) == 1)
			return ('s');
	}
	if (line[0] == 'c' && line[1] == 'y' && (ft_isspace(line[2])) == 1)
		return ('y');
	if (line[0] == 'c' && line[1] == 'o' && (ft_isspace(line[2])) == 1)
		return ('o');
	if (line[0] == 'c' && line[1] == 's' && line[2] == 'g'
		&& (ft_isspace(line[3])) == 1)
		return ('g');
	if (line[0] == 't' && line[1] == 'r' && (ft_isspace(line[2])) == 1)
		return ('t');
	if (line[0] == 't' && line[1] == 'o' && (ft_isspace(line[2])) == 1)
		return ('u');
	if (line[0] == 'p' && line[1] == 'l' && (ft_isspace(line[2])) == 1)
		return ('p');
	if (line[0] == 'p' && line[1] == 'a' && (ft_isspace(line[2])) == 1)
		return ('a');
	if (line[0] == 'd' && line[1] == 'k' && (ft_isspace(line[2])) == 1)
		return ('k');
	if (line[0] == 'e' && line[1] == 'l' && (ft_isspace(line[2])) == 1)
		return ('e');
	if (line[0] == 'b' && line[1] == 'x' && (ft_isspace(line[2])) == 1)
		return ('b');
	if (line[0] == 'h' && line[1] == 'y' && (ft_isspace(line[2])) == 1)
		return ('h');
	if (line[0] == 'm' && line[1] == 'e' && (ft_isspace(line[2])) == 1)
		return ('m');
	return (-1);
}

void ft_iterate_in_line(char **line)
{
	while ((ft_isdigit(**line)) == 1 || **line == '.' || **line == ',')
		(*line)++;
	while ((ft_isspace(**line)) == 1)
		(*line)++;
}

int ft_cam_init(t_window *win, t_cam **begin, char *line)
{
	t_cam *cur;

	if (!(cur = ft_calloc(1, sizeof(t_cam))))
		return (ft_error(6, win, "camera"));
	cur->next = *begin;
	*begin = cur;
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	(ft_isnum(line) == 1) ? ft_point_init(win, &(*cur).coord, &line)
						  : ft_error(3, win, "camera");
	(ft_isnum(line) == 1) ? ft_point_init(win, &(*cur).ori, &line)
						  : ft_error(7, win, "camera orientation");
	cur->fov = (ft_isdigit(*line) == 1) ? ft_atoi(line)
										: ft_error(7, win, "camera fov");
	while ((ft_isdigit(*line)) == 1)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	return (*line == '\0' ? ft_check_cam_parsing(win, cur)
						  : ft_error(4, win, "camera"));
}

int ft_light_init(t_window *win, t_light **begin, char *line)
{
	t_light *cur;

	if (!(cur = ft_calloc(1, sizeof(t_light))))
		return (ft_error(6, win, "light"));
	cur->next = *begin;
	*begin = cur;
	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	(ft_isnum(line) == 1) ? ft_point_init(win, &(*cur).coord, &line)
						  : ft_error(3, win, "light");
	(*cur).light_ratio =
		((ft_isdigit(*line) == 1) ? ft_atof(line)
								  : ft_error(7, win, "light ratio"));
	while ((ft_isdigit(*line)) == 1 || *line == '.')
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	(ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur).col, &line)
							 : ft_error(7, win, "light color");
	return ((*line == '\0') ? ft_check_light_parsing(win, cur)
							: ft_error(4, win, "light"));
}
