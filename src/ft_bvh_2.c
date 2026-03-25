/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bvh_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int ft_cmp_x(const void *a, const void *b)
{
	t_shape *sa;
	t_shape *sb;

	sa = *(t_shape **)a;
	sb = *(t_shape **)b;
	if (sa->pt_0.x < sb->pt_0.x)
		return (-1);
	if (sa->pt_0.x > sb->pt_0.x)
		return (1);
	return (0);
}

t_bvh_node *ft_bvh_build(t_shape **shapes, int count)
{
	t_bvh_node *node;
	int mid;
	int i;

	node = ft_calloc(1, sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	if (count == 1)
	{
		node->shape = shapes[0];
		node->box = ft_shape_aabb(shapes[0]);
		return (node);
	}
	qsort(shapes, count, sizeof(t_shape *), ft_cmp_x);
	mid = count / 2;
	node->left = ft_bvh_build(shapes, mid);
	node->right = ft_bvh_build(shapes + mid, count - mid);
	if (node->left && node->right)
	{
		node->box.min = node->left->box.min;
		node->box.max = node->left->box.max;
		i = -1;
		while (++i < 3)
		{
			if ((&node->right->box.min.x)[i] < (&node->box.min.x)[i])
				(&node->box.min.x)[i] = (&node->right->box.min.x)[i];
			if ((&node->right->box.max.x)[i] > (&node->box.max.x)[i])
				(&node->box.max.x)[i] = (&node->right->box.max.x)[i];
		}
	}
	return (node);
}

void ft_bvh_trace(t_bvh_node *node, t_ray *ray, double *min, t_shape **min_sh)
{
	t_pt best_n;
	double prev_min;

	if (!node)
		return;
	if (!ft_aabb_hit(&node->box, ray, *min))
		return;
	if (node->shape)
	{
		ft_which_shape(node->shape, ray);
		if (ray->lenght > 0.0001 && ray->lenght < *min)
		{
			*min = ray->lenght;
			*min_sh = node->shape;
		}
		return;
	}
	best_n = ray->hit_n;
	prev_min = *min;
	ft_bvh_trace(node->left, ray, min, min_sh);
	if (*min < prev_min)
	{
		best_n = ray->hit_n;
		prev_min = *min;
	}
	ft_bvh_trace(node->right, ray, min, min_sh);
	if (*min < prev_min)
		best_n = ray->hit_n;
	ray->hit_n = best_n;
}

void ft_bvh_free(t_bvh_node *node)
{
	if (!node)
		return;
	ft_bvh_free(node->left);
	ft_bvh_free(node->right);
	free(node);
}

void ft_build_scene_bvh(t_window *win)
{
	t_shape **arr;
	t_shape *cur;
	int count;
	int i;

	count = 0;
	cur = win->beg_sh;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	if (count == 0)
		return;
	arr = malloc(sizeof(t_shape *) * count);
	if (!arr)
		return;
	cur = win->beg_sh;
	i = 0;
	while (cur)
	{
		arr[i++] = cur;
		cur = cur->next;
	}
	win->bvh = ft_bvh_build(arr, count);
	free(arr);
}
