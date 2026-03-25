/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Advance xorshift32 RNG state and return next pseudo-random value */
unsigned int ft_rand_next(unsigned int *seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 17;
	*seed ^= *seed << 5;
	return (*seed);
}

/* Return a pseudo-random double in [0, 1) */
double ft_rand_float(unsigned int *seed)
{
	return ((double)ft_rand_next(seed) / 4294967296.0);
}

/* Generate a deterministic seed from pixel coordinates and sample index */
unsigned int ft_rand_seed(int x, int y, int sample)
{
	unsigned int s;

	s = (unsigned int)(x * 73856093 ^ y * 19349663 ^ sample * 83492791);
	if (s == 0)
		s = 1;
	return (s);
}

/* Return a uniformly distributed random point on the unit disk */
t_pt ft_rand_disk(unsigned int *seed)
{
	t_pt p;

	p.x = 2.0 * ft_rand_float(seed) - 1.0;
	p.y = 2.0 * ft_rand_float(seed) - 1.0;
	p.z = 0.0;
	while (p.x * p.x + p.y * p.y >= 1.0)
	{
		p.x = 2.0 * ft_rand_float(seed) - 1.0;
		p.y = 2.0 * ft_rand_float(seed) - 1.0;
	}
	return (p);
}

/* Sample a cosine-weighted random direction in the hemisphere around normal */
t_pt ft_rand_hemisphere_cosine(t_pt normal, unsigned int *seed)
{
	t_pt disk;
	t_pt tangent;
	t_pt bitangent;
	t_pt dir;
	double up_comp;

	disk = ft_rand_disk(seed);
	up_comp = sqrt(1.0 - disk.x * disk.x - disk.y * disk.y);
	if (fabs(normal.x) < 0.9)
		tangent = ft_normal_vect(ft_cross_product(normal, (t_pt){1, 0, 0}));
	else
		tangent = ft_normal_vect(ft_cross_product(normal, (t_pt){0, 1, 0}));
	bitangent = ft_cross_product(normal, tangent);
	dir = ft_addition(ft_addition(ft_multi_scal(disk.x, tangent),
								  ft_multi_scal(disk.y, bitangent)),
					  ft_multi_scal(up_comp, normal));
	return (ft_normal_vect(dir));
}
