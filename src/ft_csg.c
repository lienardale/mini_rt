/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_csg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int ft_csg_parse(t_window *win, t_shape **cur, char *line)
{
	int check;

	while ((ft_isspace(*line)) == 1 || (ft_isalpha(*line)) == 1)
		line++;
	if (*line == 'U' || *line == 'u')
		(*cur)->csg_op = CSG_UNION;
	else if (*line == 'I' || *line == 'i')
		(*cur)->csg_op = CSG_INTERSECT;
	else if (*line == 'D' || *line == 'd')
		(*cur)->csg_op = CSG_DIFFERENCE;
	else
		return (ft_error(7, win, "csg operation"));
	(*cur)->id = SHAPE_CSG;
	while (*line && (ft_isalpha(*line)) == 1)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	if (ft_isdigit(*line) == 1)
		(*cur)->csg_idx_a = ft_atoi(line);
	else
		return (ft_error(7, win, "csg index_a"));
	while ((ft_isdigit(*line)) == 1)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	check = 0;
	if (ft_isdigit(*line) == 1)
		(*cur)->csg_idx_b = ft_atoi(line);
	else
		return (ft_error(7, win, "csg index_b"));
	while ((ft_isdigit(*line)) == 1)
		line++;
	while ((ft_isspace(*line)) == 1)
		line++;
	check = (ft_isdigit(*line) == 1) ? ft_color_init(win, &(*cur)->color, &line)
									 : ft_error(7, win, "csg color");
	if (check == 0 && *line != '\0')
		check = ft_error(4, win, "csg");
	return (check);
}

int ft_csg_init(t_window *win, t_shape **cur, char *line)
{
	return (ft_csg_parse(win, cur, line));
}

static int ft_csg_test_hit(t_shape *sh, t_ray *ray, double *t_out)
{
	ft_which_shape_base(sh, ray);
	if (ray->lenght > EPSILON_HIT)
	{
		*t_out = ray->lenght;
		return (1);
	}
	*t_out = -1;
	return (0);
}

static int ft_point_inside_shape(t_shape *sh, t_pt p)
{
	t_ray test;
	int hits;
	double t;
	t_pt start;

	start = ft_addition(p, (t_pt){EPSILON_HIT, EPSILON_HIT, EPSILON_HIT});
	test.orig = start;
	test.dir = (t_pt){1, 0, 0};
	test.lenght = -1;
	hits = 0;
	ft_which_shape_base(sh, &test);
	if (test.lenght > EPSILON_HIT)
		hits++;
	(void)t;
	return (hits % 2);
}

void ft_intersect_ray_csg(t_shape *sh, t_ray *ray, t_window *win)
{
	t_shape *shape_a;
	t_shape *shape_b;
	double t_a;
	double t_b;
	t_ray ray_a;
	t_ray ray_b;

	shape_a = ft_get_shape_by_index(win, sh->csg_idx_a);
	shape_b = ft_get_shape_by_index(win, sh->csg_idx_b);
	if (!shape_a || !shape_b)
	{
		ray->lenght = -1;
		return;
	}
	ray_a = *ray;
	ray_b = *ray;
	ft_csg_test_hit(shape_a, &ray_a, &t_a);
	ft_csg_test_hit(shape_b, &ray_b, &t_b);
	ft_csg_combine(sh, ray, &ray_a, &ray_b, shape_a, shape_b);
}

void ft_csg_combine(t_shape *sh, t_ray *ray, t_ray *ray_a, t_ray *ray_b,
					t_shape *sa, t_shape *sb)
{
	int a_hit;
	int b_hit;

	a_hit = (ray_a->lenght > EPSILON_HIT);
	b_hit = (ray_b->lenght > EPSILON_HIT);
	if (sh->csg_op == CSG_UNION)
	{
		if (!a_hit && !b_hit)
			ray->lenght = -1;
		else if (a_hit && (!b_hit || ray_a->lenght <= ray_b->lenght))
		{
			ray->lenght = ray_a->lenght;
			ray->hit_n = ray_a->hit_n;
		}
		else
		{
			ray->lenght = ray_b->lenght;
			ray->hit_n = ray_b->hit_n;
		}
	}
	else if (sh->csg_op == CSG_INTERSECT)
		ft_csg_intersect(sh, ray, ray_a, ray_b, sa, sb);
	else if (sh->csg_op == CSG_DIFFERENCE)
		ft_csg_difference(sh, ray, ray_a, ray_b, sa, sb);
}

void ft_csg_intersect(t_shape *sh, t_ray *ray, t_ray *ray_a, t_ray *ray_b,
					  t_shape *sa, t_shape *sb)
{
	t_pt pa;
	t_pt pb;

	(void)sh;
	if (ray_a->lenght <= EPSILON_HIT || ray_b->lenght <= EPSILON_HIT)
	{
		ray->lenght = -1;
		return;
	}
	pa = ft_addition(ray->orig, ft_multi_scal(ray_a->lenght, ray->dir));
	pb = ft_addition(ray->orig, ft_multi_scal(ray_b->lenght, ray->dir));
	if (ray_a->lenght >= ray_b->lenght && ft_point_inside_shape(sb, pa))
	{
		ray->lenght = ray_a->lenght;
		ray->hit_n = ray_a->hit_n;
	}
	else if (ft_point_inside_shape(sa, pb))
	{
		ray->lenght = ray_b->lenght;
		ray->hit_n = ray_b->hit_n;
	}
	else
		ray->lenght = -1;
}

void ft_csg_difference(t_shape *sh, t_ray *ray, t_ray *ray_a, t_ray *ray_b,
					   t_shape *sa, t_shape *sb)
{
	t_pt pa;

	(void)sh;
	(void)sa;
	if (ray_a->lenght <= EPSILON_HIT)
	{
		ray->lenght = -1;
		return;
	}
	pa = ft_addition(ray->orig, ft_multi_scal(ray_a->lenght, ray->dir));
	if (!ft_point_inside_shape(sb, pa))
	{
		ray->lenght = ray_a->lenght;
		ray->hit_n = ray_a->hit_n;
	}
	else if (ray_b->lenght > EPSILON_HIT)
	{
		ray->lenght = ray_b->lenght;
		ray->hit_n = ray_b->hit_n;
		ft_inv_norm(&ray->hit_n);
	}
	else
		ray->lenght = -1;
}

t_shape *ft_get_shape_by_index(t_window *win, int idx)
{
	t_shape *cur;
	int i;

	cur = win->beg_sh;
	i = 0;
	while (cur)
	{
		if (i == idx)
			return (cur);
		i++;
		cur = cur->next;
	}
	return (NULL);
}

void ft_which_shape_base(t_shape *sh, t_ray *ray)
{
	if (sh->id == SHAPE_SPHERE)
		ft_intersect_ray_sphere(sh, ray);
	else if (sh->id == SHAPE_PLANE)
		ft_intersect_ray_plan(sh, ray);
	else if (sh->id == SHAPE_SQUARE)
		ft_intersect_ray_square(sh, ray);
	else if (sh->id == SHAPE_CYLINDER)
		ft_intersect_ray_cylinder(sh, ray);
	else if (sh->id == SHAPE_TRIANGLE)
		ft_intersect_ray_triangle(sh, ray);
	else if (sh->id == SHAPE_CONE)
		ft_intersect_ray_cone(sh, ray);
	else if (sh->id == SHAPE_DISK)
		ft_intersect_ray_disk(sh, ray);
	else if (sh->id == SHAPE_ELLIPSOID)
		ft_intersect_ray_ellipsoid(sh, ray);
	else if (sh->id == SHAPE_BOX)
		ft_intersect_ray_box(sh, ray);
	else if (sh->id == SHAPE_TORUS)
		ft_intersect_ray_torus(sh, ray);
	else if (sh->id == SHAPE_HYPERBOLOID)
		ft_intersect_ray_hyperboloid(sh, ray);
	else if (sh->id == SHAPE_PARABOLOID)
		ft_intersect_ray_paraboloid(sh, ray);
}
