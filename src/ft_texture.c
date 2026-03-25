/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_texture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/* Load an XPM image file as a texture and return its pixel data */
t_texture *ft_texture_load(char *path, void *mlx_ptr)
{
	t_texture *tex;

	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->img = mlx_xpm_file_to_image(mlx_ptr, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		free(tex);
		return (NULL);
	}
	tex->data =
		mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
	return (tex);
}

/* Destroy the texture image and free its memory */
void ft_texture_free(t_texture *tex, void *mlx_ptr)
{
	if (!tex)
		return;
	if (tex->img && mlx_ptr)
		mlx_destroy_image(mlx_ptr, tex->img);
	free(tex);
}

/* Sample a color from the texture at UV coordinates (wrapping) */
t_argb ft_texture_sample(t_texture *tex, double u, double v)
{
	int x;
	int y;
	int idx;
	t_argb color;

	if (!tex || !tex->data)
		return ((t_argb){0, 128, 128, 128});
	u = u - floor(u);
	v = v - floor(v);
	x = (int)(u * (tex->width - 1));
	y = (int)(v * (tex->height - 1));
	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	idx = y * tex->line_len + x * (tex->bpp / 8);
	color.b = (unsigned char)tex->data[idx];
	color.g = (unsigned char)tex->data[idx + 1];
	color.r = (unsigned char)tex->data[idx + 2];
	color.a = 0;
	return (color);
}

/* Compute spherical UV mapping from the hit point */
void ft_uv_sphere(t_shape *sh, t_ray *ray, double *u, double *v)
{
	t_pt r;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	r = ft_normal_vect(r);
	*u = 0.5 + atan2(r.z, r.x) / (2.0 * M_PI);
	*v = 0.5 - asin(r.y) / M_PI;
	*u *= sh->mat.tex_scale_u;
	*v *= sh->mat.tex_scale_v;
}

/* Compute planar UV mapping by projecting hit point onto local axes */
void ft_uv_plane(t_shape *sh, t_ray *ray, double *u, double *v)
{
	t_pt r;
	t_pt right;
	t_pt fwd;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	if (fabs(sh->ori.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(sh->ori, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(sh->ori, (t_pt){0, 1, 0}));
	fwd = ft_cross_product(right, sh->ori);
	*u = ft_dot_product(r, right) * sh->mat.tex_scale_u;
	*v = ft_dot_product(r, fwd) * sh->mat.tex_scale_v;
}

/* Compute cylindrical UV mapping using angle around axis and height */
void ft_uv_cylinder(t_shape *sh, t_ray *ray, double *u, double *v)
{
	t_pt r;
	t_pt d;
	t_pt right;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	d = ft_subtraction(r, sh->pt_0);
	*v = ft_dot_product(d, sh->ori) / sh->height;
	if (fabs(sh->ori.x) < 0.9)
		right = ft_normal_vect(ft_cross_product(sh->ori, (t_pt){1, 0, 0}));
	else
		right = ft_normal_vect(ft_cross_product(sh->ori, (t_pt){0, 1, 0}));
	*u = 0.5 + atan2(ft_dot_product(d, ft_cross_product(right, sh->ori)),
					 ft_dot_product(d, right)) /
				   (2.0 * M_PI);
	*u *= sh->mat.tex_scale_u;
	*v *= sh->mat.tex_scale_v;
}

/* Compute fallback spherical UV mapping for unsupported shape types */
void ft_uv_generic(t_shape *sh, t_ray *ray, double *u, double *v)
{
	t_pt r;

	r = ft_addition(ray->orig, ft_multi_scal(ray->lenght, ray->dir));
	r = ft_subtraction(r, sh->pt_0);
	*u = 0.5 + atan2(r.z, r.x) / (2.0 * M_PI);
	*v = 0.5 - asin(fmax(-1, fmin(1, r.y / ft_lenght(r)))) / M_PI;
	*u *= sh->mat.tex_scale_u;
	*v *= sh->mat.tex_scale_v;
}

/* Return the texture-mapped color at the ray hit point, or the base color */
t_argb ft_get_shape_color(t_shape *sh, t_ray *ray)
{
	double u;
	double v;

	if (!sh->mat.texture)
		return (sh->color);
	if (sh->id == SHAPE_SPHERE)
		ft_uv_sphere(sh, ray, &u, &v);
	else if (sh->id == SHAPE_PLANE || sh->id == SHAPE_SQUARE)
		ft_uv_plane(sh, ray, &u, &v);
	else if (sh->id == SHAPE_CYLINDER)
		ft_uv_cylinder(sh, ray, &u, &v);
	else
		ft_uv_generic(sh, ray, &u, &v);
	return (ft_texture_sample(sh->mat.texture, u, v));
}
