/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Return sky color for a ray direction using a gradient from horizon to zenith.
 * Simulates a simple physically-inspired sky: blue zenith, white-ish horizon,
 * warm ground hemisphere. Values are in [0,255] scale for consistency. */
t_argb ft_env_sky(t_pt dir)
{
	t_argb sky;
	double t;
	double horizon_r;
	double horizon_g;
	double horizon_b;
	double zenith_r;
	double zenith_g;
	double zenith_b;

	sky.a = 0;
	t = 0.5 * (dir.y + 1.0);
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	horizon_r = 255.0;
	horizon_g = 255.0;
	horizon_b = 255.0;
	zenith_r = 128.0;
	zenith_g = 178.0;
	zenith_b = 255.0;
	sky.r = (1.0 - t) * horizon_r + t * zenith_r;
	sky.g = (1.0 - t) * horizon_g + t * zenith_g;
	sky.b = (1.0 - t) * horizon_b + t * zenith_b;
	return (sky);
}
