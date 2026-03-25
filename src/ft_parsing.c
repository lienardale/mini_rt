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

/* Parse an R,G,B color triplet from the line and validate it. */
int ft_color_init(t_window *win, t_argb *col, char **l)
{
	col->a = -1;
	col->r = -1;
	col->g = -1;
	col->b = -1;
	col->r = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	if (**l == ',')
		(*l)++;
	col->g = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	if (**l == ',')
		(*l)++;
	col->b = (ft_isdigit(**l) == 1) ? ft_atoi(*l) : -1;
	while ((ft_isdigit(**l)) == 1 && **l)
		(*l)++;
	while ((ft_isspace(**l)) == 1)
		(*l)++;
	return (ft_color_check(win, *col));
}

/* Parse a 3D point (x,y,z) from the line and validate it. */
int ft_point_init(t_window *win, t_pt *pt, char **l)
{
	pt->x = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	if (**l == ',')
		(*l)++;
	pt->y = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	if (**l == ',')
		(*l)++;
	pt->z = ((ft_isnum(*l) == 1) ? ft_atof(*l) : ft_error(7, win, "point"));
	while (((ft_isdigit(**l)) == 1 || **l == '.' || **l == '-') && **l)
		(*l)++;
	while ((ft_isspace(**l)) == 1)
		(*l)++;
	return (ft_pt_check(win, *pt));
}

/* Identify the shape type and dispatch to its specific initializer. */
int ft_shape_init(t_window *win, t_shape **begin, char *line)
{
	int i;
	t_shape *current;

	i = 0;
	if ((i = ft_which_id(line)) == -1)
		return (ft_error(5, win, "shape"));
	if (i == SHAPE_MESH)
		return (ft_mesh_init(win, begin, line));
	if (!(current = ft_calloc(1, sizeof(t_shape))))
		return (ft_error(6, win, "shape"));
	current->next = *begin;
	*begin = current;
	ft_material_default(&current->mat);
	if (i == SHAPE_SPHERE)
		ft_sphere_init(win, &current, line);
	else if (i == SHAPE_PLANE)
		ft_plane_init(win, &current, line);
	else if (i == SHAPE_SQUARE)
		ft_square_init(win, &current, line);
	else if (i == SHAPE_CYLINDER)
		ft_cylinder_init(win, &current, line);
	else if (i == SHAPE_TRIANGLE)
		ft_triangle_init(win, &current, line);
	else if (i == SHAPE_CONE)
		ft_cone_init(win, &current, line);
	else if (i == SHAPE_DISK)
		ft_disk_init(win, &current, line);
	else if (i == SHAPE_TORUS)
		ft_torus_init(win, &current, line);
	else if (i == SHAPE_ELLIPSOID)
		ft_ellipsoid_init(win, &current, line);
	else if (i == SHAPE_BOX)
		ft_box_init(win, &current, line);
	else if (i == SHAPE_HYPERBOLOID)
		ft_hyperboloid_init(win, &current, line);
	else if (i == SHAPE_PARABOLOID)
		ft_paraboloid_init(win, &current, line);
	else if (i == SHAPE_CSG)
		ft_csg_init(win, &current, line);
	return (0);
}

/* Parse ambient light ratio and color from the scene line. */
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
	if ((ft_isdigit(*line)) == 1)
		ft_color_init(amb, &amb->col, &line);
	else
		ft_error(3, amb, "amb_light");
	return ((*line == '\0') ? ft_check_amb_light(amb)
							: ft_error(4, amb, "amb_light"));
}

/* Parse window resolution (width and height) from the scene line. */
int ft_resol_init(t_window *res, char *line)
{
	while (*line && ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1))
		line++;
	res->x = (unsigned int)((ft_isdigit(*line) == 1)
								? ft_atoi(line)
								: ft_error(3, res, "resolution"));
	while ((ft_isdigit(*line)) == 1 && *line)
		line++;
	while (*line && (ft_isspace(*line)) == 1)
		line++;
	res->y = (unsigned int)((ft_isdigit(*line) == 1)
								? ft_atoi(line)
								: ft_error(3, res, "resolution"));
	while (((ft_isdigit(*line)) == 1) && *line)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	return ((*line == '\0') ? ft_check_resol(res)
							: ft_error(4, res, "resolution"));
}
