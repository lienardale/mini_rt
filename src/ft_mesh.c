/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mesh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Count vertex and face lines in an OBJ file for memory allocation */
static int ft_count_obj_elements(int fd, int *vcount, int *fcount)
{
	char *line;
	int ret;

	*vcount = 0;
	*fcount = 0;
	ret = get_next_line(fd, &line, 0);
	while (ret > 0)
	{
		if (line[0] == 'v' && line[1] == ' ')
			(*vcount)++;
		else if (line[0] == 'f' && line[1] == ' ')
			(*fcount)++;
		free(line);
		ret = get_next_line(fd, &line, 0);
	}
	if (ret == 0 && line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			(*vcount)++;
		else if (line[0] == 'f' && line[1] == ' ')
			(*fcount)++;
		free(line);
	}
	return (0);
}

/* Parse a vertex line ("v x y z") into a 3D point */
static t_pt ft_parse_vertex(char *line)
{
	t_pt v;

	line += 2;
	while (*line == ' ')
		line++;
	v.x = ft_atof(line);
	while (*line && *line != ' ')
		line++;
	while (*line == ' ')
		line++;
	v.y = ft_atof(line);
	while (*line && *line != ' ')
		line++;
	while (*line == ' ')
		line++;
	v.z = ft_atof(line);
	return (v);
}

/* Parse a face line ("f v1 v2 v3") into three vertex indices (1-based to 0-based) */
static void ft_parse_face(char *line, int *indices)
{
	int i;

	line += 2;
	i = 0;
	while (i < 3)
	{
		while (*line == ' ')
			line++;
		indices[i] = ft_atoi(line) - 1;
		while (*line && *line != ' ' && *line != '/')
			line++;
		if (*line == '/')
		{
			while (*line && *line != ' ')
				line++;
		}
		i++;
	}
}

/* Second pass: read OBJ file and populate vertex and face arrays */
static int ft_load_obj_data(int fd, t_pt *verts, int *faces)
{
	char *line;
	int ret;
	int vi;
	int fi;

	vi = 0;
	fi = 0;
	ret = get_next_line(fd, &line, 0);
	while (ret > 0)
	{
		if (line[0] == 'v' && line[1] == ' ')
			verts[vi++] = ft_parse_vertex(line);
		else if (line[0] == 'f' && line[1] == ' ')
		{
			ft_parse_face(line, &faces[fi * 3]);
			fi++;
		}
		free(line);
		ret = get_next_line(fd, &line, 0);
	}
	if (ret == 0 && line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			verts[vi++] = ft_parse_vertex(line);
		else if (line[0] == 'f' && line[1] == ' ')
		{
			ft_parse_face(line, &faces[fi * 3]);
			fi++;
		}
		free(line);
	}
	return (0);
}

/* Parse mesh scene line (OBJ path, optional offset, optional color) and load mesh */
int ft_mesh_init(t_window *win, t_shape **begin, char *line)
{
	char path[256];
	t_argb color;
	t_pt offset;
	int i;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	i = 0;
	while (*line && *line != ' ' && *line != '\t' && i < 255)
		path[i++] = *line++;
	path[i] = '\0';
	while ((ft_isspace(*line)) == 1)
		line++;
	if (ft_isnum(line) == 1)
		ft_point_init(win, &offset, &line);
	else
		offset = (t_pt){0, 0, 0};
	if (ft_isdigit(*line) == 1)
		ft_color_init(win, &color, &line);
	else
		color = (t_argb){0, 200, 200, 200};
	return (ft_load_obj_mesh(win, begin, path, offset, color));
}

int ft_load_obj_mesh(t_window *win, t_shape **begin, char *path, t_pt offset,
					 t_argb color)
{
	int fd;
	int vcount;
	int fcount;
	t_pt *verts;
	int *faces;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_error(7, win, "mesh file open"));
	ft_count_obj_elements(fd, &vcount, &fcount);
	close(fd);
	if (vcount < 3 || fcount < 1)
		return (ft_error(7, win, "mesh too few vertices/faces"));
	verts = malloc(sizeof(t_pt) * vcount);
	faces = malloc(sizeof(int) * fcount * 3);
	if (!verts || !faces)
	{
		free(verts);
		free(faces);
		return (ft_error(6, win, "mesh"));
	}
	fd = open(path, O_RDONLY);
	ft_load_obj_data(fd, verts, faces);
	close(fd);
	ft_mesh_create_triangles(win, begin, verts, faces, fcount, offset, color);
	free(verts);
	free(faces);
	return (0);
}

void ft_mesh_create_triangles(t_window *win, t_shape **begin, t_pt *verts,
							  int *faces, int fcount, t_pt offset, t_argb color)
{
	int i;
	t_shape *cur;

	i = 0;
	while (i < fcount)
	{
		cur = ft_calloc(1, sizeof(t_shape));
		if (!cur)
		{
			ft_error(6, win, "mesh triangle");
			return;
		}
		cur->id = SHAPE_TRIANGLE;
		cur->pt_0 = ft_addition(verts[faces[i * 3]], offset);
		cur->pt_1 = ft_addition(verts[faces[i * 3 + 1]], offset);
		cur->pt_2 = ft_addition(verts[faces[i * 3 + 2]], offset);
		cur->color = color;
		cur->next = *begin;
		*begin = cur;
		i++;
	}
}
