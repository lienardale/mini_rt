/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by alienard          #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PI 3.14159265358979323846

typedef struct s_color
{
	int r;
	int g;
	int b;
}	t_color;

typedef struct s_vec
{
	double x;
	double y;
	double z;
}	t_vec;

typedef struct s_config
{
	int		width;
	int		height;
	int		num_objects;
	double	scene_radius;
	int		seed;
	char	preset[64];
}	t_config;

static t_color g_palettes[][6] = {
	{{255, 87, 51}, {255, 189, 51}, {51, 255, 87}, {51, 189, 255},
		{189, 51, 255}, {255, 51, 189}},
	{{220, 20, 60}, {255, 165, 0}, {255, 215, 0}, {50, 205, 50},
		{30, 144, 255}, {148, 103, 189}},
	{{245, 245, 245}, {200, 200, 200}, {150, 150, 150}, {100, 100, 100},
		{60, 60, 60}, {30, 30, 30}},
	{{255, 107, 107}, {255, 159, 67}, {254, 202, 87}, {29, 209, 161},
		{72, 219, 251}, {243, 104, 224}},
};

static int g_num_palettes = 4;

static double randf(void)
{
	return ((double)rand() / (double)RAND_MAX);
}

static double randf_range(double min, double max)
{
	return (min + randf() * (max - min));
}

static t_color pick_color(int palette, int idx)
{
	return (g_palettes[palette % g_num_palettes][idx % 6]);
}

static void write_header(FILE *f, t_config *cfg)
{
	fprintf(f, "R\t%d\t%d\n", cfg->width, cfg->height);
	fprintf(f, "A\t0.15\t255,255,255\n\n");
}

static t_vec dir_to_euler(t_vec dir)
{
	t_vec	euler;
	double	len;

	len = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	dir.x /= len;
	dir.y /= len;
	dir.z /= len;
	euler.x = -asin(dir.y) / PI;
	euler.y = atan2(-dir.x, -dir.z) / PI;
	euler.z = 0;
	return (euler);
}

static void write_camera(FILE *f, t_vec pos, t_vec dir, int fov)
{
	t_vec	ori;

	ori = dir_to_euler(dir);
	if (ori.x == 0.0)
		ori.x = 0.0;
	if (ori.y == 0.0)
		ori.y = 0.0;
	if (ori.z == 0.0)
		ori.z = 0.0;
	fprintf(f, "c\t%.1f,%.1f,%.1f\t%.4f,%.4f,%.4f\t%d\n",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, fov);
}

static void write_light(FILE *f, t_vec pos, double intensity, t_color col)
{
	fprintf(f, "l\t%.1f,%.1f,%.1f\t%.1f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, intensity, col.r, col.g, col.b);
}

static void write_sphere(FILE *f, t_vec pos, double diam, t_color col,
						  const char *mat)
{
	fprintf(f, "sp\t%.2f,%.2f,%.2f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, diam, col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_plane(FILE *f, t_vec pos, t_vec ori, t_color col)
{
	fprintf(f, "pl\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, col.r, col.g, col.b);
}

static void write_cylinder(FILE *f, t_vec pos, t_vec ori, double diam,
							double height, t_color col)
{
	fprintf(f, "cy\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%.2f\t%.2f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z,
		diam, height, col.r, col.g, col.b);
}

static void write_cylinder_mat(FILE *f, t_vec pos, t_vec ori, double diam,
							   double height, t_color col, const char *mat)
{
	fprintf(f, "cy\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%.2f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z,
		diam, height, col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_cone(FILE *f, t_vec pos, t_vec ori, double diam,
						double height, t_color col)
{
	fprintf(f, "co\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%.2f\t%.2f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z,
		diam, height, col.r, col.g, col.b);
}

static void write_cone_mat(FILE *f, t_vec pos, t_vec ori, double diam,
						   double height, t_color col, const char *mat)
{
	fprintf(f, "co\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%.2f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z,
		diam, height, col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_square(FILE *f, t_vec pos, t_vec ori, double side,
						  t_color col)
{
	fprintf(f, "sq\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.0f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, side,
		col.r, col.g, col.b);
}

static void write_square_mat(FILE *f, t_vec pos, t_vec ori, double side,
							 t_color col, const char *mat)
{
	fprintf(f, "sq\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.0f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, side,
		col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_box(FILE *f, t_vec pos, t_vec size, t_color col)
{
	fprintf(f, "bx\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%d,%d,%d\n",
		pos.x, pos.y, pos.z, size.x, size.y, size.z, col.r, col.g, col.b);
}

static void write_box_mat(FILE *f, t_vec pos, t_vec size, t_color col,
						   const char *mat)
{
	fprintf(f, "bx\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, size.x, size.y, size.z, col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_disk(FILE *f, t_vec pos, t_vec ori, double diam,
					   t_color col, const char *mat)
{
	fprintf(f, "dk\t%.2f,%.2f,%.2f\t%.1f,%.1f,%.1f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, diam,
		col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_torus(FILE *f, t_vec pos, t_vec ori, double major,
						double minor, t_color col, const char *mat)
{
	fprintf(f, "to\t%.2f,%.2f,%.2f\t%.1f,%.1f,%.1f\t%.2f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, major, minor,
		col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_ellipsoid(FILE *f, t_vec pos, t_vec radii, t_color col,
							const char *mat)
{
	fprintf(f, "el\t%.2f,%.2f,%.2f\t%.2f,%.2f,%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, radii.x, radii.y, radii.z,
		col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void write_triangle(FILE *f, t_vec p1, t_vec p2, t_vec p3,
						   t_color col)
{
	fprintf(f, "tr\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f\t%.1f,%.1f,%.1f"
		"\t%d,%d,%d\n",
		p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z,
		col.r, col.g, col.b);
}

static void write_paraboloid(FILE *f, t_vec pos, t_vec ori, double scale,
							 double height, t_color col, const char *mat)
{
	fprintf(f, "pa\t%.2f,%.2f,%.2f\t%.1f,%.1f,%.1f\t%.2f\t%.2f\t%d,%d,%d",
		pos.x, pos.y, pos.z, ori.x, ori.y, ori.z, scale, height,
		col.r, col.g, col.b);
	if (mat)
		fprintf(f, "\t%s", mat);
	fprintf(f, "\n");
}

static void gen_cornell_box(FILE *f, t_config *cfg)
{
	write_header(f, cfg);
	fprintf(f, "c\t0.0,5.0,0.0\t0.0000,0.0000,0.0000\t70\n");
	fprintf(f, "\n");
	write_light(f, (t_vec){0, 9.5, 5}, 0.9, (t_color){255, 250, 240});
	write_light(f, (t_vec){2, 8, 3}, 0.3, (t_color){255, 230, 210});
	fprintf(f, "\n");
	write_square(f, (t_vec){0, 0, 5}, (t_vec){0, 1, 0}, 10,
		(t_color){150, 150, 150});
	write_square(f, (t_vec){0, 10, 5}, (t_vec){0, -1, 0}, 10,
		(t_color){150, 150, 150});
	write_square(f, (t_vec){0, 5, 10}, (t_vec){0, 0, -1}, 10,
		(t_color){150, 150, 150});
	write_square(f, (t_vec){-5, 5, 5}, (t_vec){1, 0, 0}, 10,
		(t_color){180, 30, 30});
	write_square(f, (t_vec){5, 5, 5}, (t_vec){-1, 0, 0}, 10,
		(t_color){30, 180, 30});
	fprintf(f, "\n");
	write_sphere(f, (t_vec){-1.5, 1.5, 6}, 3.0, (t_color){240, 240, 255},
		"refl:0.8 spec:0.9");
	write_sphere(f, (t_vec){2, 1, 3.5}, 2.0, (t_color){255, 255, 255},
		"trans:0.9 ior:1.5 spec:0.5");
	write_box(f, (t_vec){2, 3, 8}, (t_vec){1.8, 6, 1.8},
		(t_color){200, 200, 200});
}

static void gen_checkerboard_spheres(FILE *f, t_config *cfg)
{
	int i;
	int j;
	int pal;
	t_color col;

	write_header(f, cfg);
	write_camera(f, (t_vec){0, 8, 18}, (t_vec){0, -0.3, -1}, 70);
	fprintf(f, "\n");
	write_light(f, (t_vec){-10, 20, 10}, 0.8, (t_color){255, 255, 255});
	write_light(f, (t_vec){10, 15, 15}, 0.6, (t_color){255, 240, 220});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -0.5, 0}, (t_vec){0, 1, 0},
		(t_color){180, 180, 190});
	fprintf(f, "\n");
	pal = cfg->seed % g_num_palettes;
	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 5)
		{
			col = pick_color(pal, (i + j) % 6);
			write_sphere(f, (t_vec){(i - 2) * 3.0, 1.0, (j - 2) * 3.0 - 5},
				2.0, col, ((i + j) % 3 == 0) ? "refl:0.4 spec:0.6" : NULL);
			j++;
		}
		i++;
	}
}

static void gen_solar_system(FILE *f, t_config *cfg)
{
	double angles[8];
	double radii[8];
	double sizes[8];
	t_color colors[8];
	int i;

	radii[0] = 3;
	radii[1] = 5;
	radii[2] = 7;
	radii[3] = 9;
	radii[4] = 13;
	radii[5] = 17;
	radii[6] = 21;
	radii[7] = 25;
	sizes[0] = 0.8;
	sizes[1] = 1.8;
	sizes[2] = 2.0;
	sizes[3] = 1.2;
	sizes[4] = 4.0;
	sizes[5] = 3.5;
	sizes[6] = 3.0;
	sizes[7] = 2.8;
	colors[0] = (t_color){180, 140, 100};
	colors[1] = (t_color){230, 200, 150};
	colors[2] = (t_color){70, 130, 200};
	colors[3] = (t_color){200, 80, 50};
	colors[4] = (t_color){220, 180, 130};
	colors[5] = (t_color){210, 190, 160};
	colors[6] = (t_color){170, 210, 230};
	colors[7] = (t_color){80, 100, 200};
	write_header(f, cfg);
	write_camera(f, (t_vec){0, 30, 40}, (t_vec){0, -0.5, -1}, 70);
	fprintf(f, "\n");
	write_light(f, (t_vec){0, 5, 0}, 1.0, (t_color){255, 250, 200});
	fprintf(f, "\n");
	write_sphere(f, (t_vec){0, 0, 0}, 5.0, (t_color){255, 200, 50},
		"spec:0.8");
	i = 0;
	while (i < 8)
	{
		srand(cfg->seed + i);
		angles[i] = randf() * 2.0 * PI;
		write_sphere(f, (t_vec){radii[i] * cos(angles[i]), 0,
			radii[i] * sin(angles[i])}, sizes[i], colors[i], NULL);
		i++;
	}
}

static void gen_spiral(FILE *f, t_config *cfg)
{
	int i;
	int n;
	double angle;
	double radius;
	double y;
	t_color col;
	int pal;

	write_header(f, cfg);
	write_camera(f, (t_vec){0, 15, 25}, (t_vec){0, -0.4, -1}, 70);
	fprintf(f, "\n");
	write_light(f, (t_vec){5, 20, 10}, 0.9, (t_color){255, 255, 255});
	write_light(f, (t_vec){-5, 15, 15}, 0.5, (t_color){200, 200, 255});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -1, 0}, (t_vec){0, 1, 0},
		(t_color){200, 200, 210});
	fprintf(f, "\n");
	n = cfg->num_objects;
	if (n < 5)
		n = 20;
	pal = cfg->seed % g_num_palettes;
	i = 0;
	while (i < n)
	{
		angle = (double)i * 0.5;
		radius = 2.0 + (double)i * 0.3;
		y = (double)i * 0.4;
		col = pick_color(pal, i % 6);
		write_sphere(f, (t_vec){radius * cos(angle), y,
			radius * sin(angle) - 10}, 1.2 + 0.3 * sin(i * 0.7), col,
			(i % 4 == 0) ? "refl:0.3 spec:0.5" : NULL);
		i++;
	}
}

static void gen_grid(FILE *f, t_config *cfg)
{
	int ix;
	int iz;
	int n;
	double spacing;
	t_color col;
	int pal;
	char *shapes[] = {"sphere", "box", "cylinder"};
	int shape_type;

	write_header(f, cfg);
	write_camera(f, (t_vec){0, 12, 20}, (t_vec){0, -0.35, -1}, 70);
	fprintf(f, "\n");
	write_light(f, (t_vec){-8, 20, 8}, 0.8, (t_color){255, 255, 255});
	write_light(f, (t_vec){8, 15, 12}, 0.6, (t_color){255, 240, 200});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -0.5, 0}, (t_vec){0, 1, 0},
		(t_color){190, 190, 200});
	fprintf(f, "\n");
	n = (int)sqrt((double)cfg->num_objects);
	if (n < 3)
		n = 4;
	spacing = cfg->scene_radius * 2.0 / (n - 1);
	pal = cfg->seed % g_num_palettes;
	ix = 0;
	while (ix < n)
	{
		iz = 0;
		while (iz < n)
		{
			srand(cfg->seed + ix * 100 + iz);
			col = pick_color(pal, (ix + iz) % 6);
			shape_type = (ix + iz) % 3;
			(void)shapes;
			if (shape_type == 0)
				write_sphere(f,
					(t_vec){(ix - n / 2) * spacing, 1.0,
						(iz - n / 2) * spacing - 5},
					1.8, col, NULL);
			else if (shape_type == 1)
				write_box(f,
					(t_vec){(ix - n / 2) * spacing, 1.0,
						(iz - n / 2) * spacing - 5},
					(t_vec){1.5, 1.5, 1.5}, col);
			else
				write_cylinder(f,
					(t_vec){(ix - n / 2) * spacing, 0,
						(iz - n / 2) * spacing - 5},
					(t_vec){0, 1, 0}, 1.2, 2.0, col);
			iz++;
		}
		ix++;
	}
}

static void gen_random_scatter(FILE *f, t_config *cfg)
{
	int i;
	int n;
	t_vec pos;
	double diam;
	t_color col;
	int pal;
	int shape;

	write_header(f, cfg);
	write_camera(f, (t_vec){0, 10, 20}, (t_vec){0, -0.25, -1}, 70);
	fprintf(f, "\n");
	write_light(f, (t_vec){0, 25, 10}, 0.8, (t_color){255, 255, 255});
	write_light(f, (t_vec){-10, 10, 20}, 0.5, (t_color){200, 220, 255});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -1, 0}, (t_vec){0, 1, 0},
		(t_color){200, 200, 210});
	fprintf(f, "\n");
	n = cfg->num_objects;
	if (n < 3)
		n = 15;
	pal = cfg->seed % g_num_palettes;
	srand(cfg->seed);
	i = 0;
	while (i < n)
	{
		pos.x = randf_range(-cfg->scene_radius, cfg->scene_radius);
		pos.z = randf_range(-cfg->scene_radius - 5, -2);
		diam = randf_range(0.8, 3.0);
		pos.y = diam / 2.0;
		col = pick_color(pal, i % 6);
		shape = (int)(randf() * 3);
		if (shape == 0)
			write_sphere(f, pos, diam, col,
				(randf() > 0.7) ? "refl:0.5 spec:0.7" : NULL);
		else if (shape == 1)
			write_box(f, pos, (t_vec){diam / 2, diam / 2, diam / 2}, col);
		else
			write_cone(f, (t_vec){pos.x, 0, pos.z}, (t_vec){0, 1, 0},
				diam * 0.8, diam * 1.5, col);
		i++;
	}
}

static void gen_reflections(FILE *f, t_config *cfg)
{
	write_header(f, cfg);
	write_camera(f, (t_vec){0, 5, 15}, (t_vec){0, -0.15, -1}, 65);
	fprintf(f, "\n");
	write_light(f, (t_vec){-5, 15, 5}, 0.8, (t_color){255, 255, 255});
	write_light(f, (t_vec){5, 10, 10}, 0.5, (t_color){220, 220, 255});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -1, 0}, (t_vec){0, 1, 0},
		(t_color){200, 200, 210});
	fprintf(f, "\n");
	write_sphere(f, (t_vec){0, 2, -3}, 4.0, (t_color){240, 240, 255},
		"refl:0.9 spec:0.95");
	write_sphere(f, (t_vec){-4, 1.2, 0}, 2.4, (t_color){255, 50, 50},
		"refl:0.3 spec:0.6");
	write_sphere(f, (t_vec){4, 1.2, 0}, 2.4, (t_color){50, 50, 255},
		"refl:0.3 spec:0.6");
	write_sphere(f, (t_vec){-2, 0.8, 3}, 1.6, (t_color){50, 220, 50},
		"spec:0.4");
	write_sphere(f, (t_vec){2, 0.8, 3}, 1.6, (t_color){255, 200, 50},
		"spec:0.4");
	write_sphere(f, (t_vec){0, 1.0, 5}, 2.0, (t_color){200, 50, 200},
		"trans:0.7 ior:1.5 spec:0.5");
}

static void gen_glass_spheres(FILE *f, t_config *cfg)
{
	write_header(f, cfg);
	write_camera(f, (t_vec){0, 4, 12}, (t_vec){0, -0.1, -1}, 65);
	fprintf(f, "\n");
	write_light(f, (t_vec){-8, 15, 5}, 0.9, (t_color){255, 255, 255});
	write_light(f, (t_vec){8, 12, 8}, 0.4, (t_color){200, 220, 255});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, -1, 0}, (t_vec){0, 1, 0},
		(t_color){220, 220, 230});
	write_plane(f, (t_vec){0, 0, -15}, (t_vec){0, 0, 1},
		(t_color){180, 180, 200});
	fprintf(f, "\n");
	write_sphere(f, (t_vec){-4, 2, -2}, 4.0, (t_color){255, 255, 255},
		"trans:0.85 ior:1.5 spec:0.9 refl:0.1");
	write_sphere(f, (t_vec){0, 1.5, 2}, 3.0, (t_color){230, 255, 230},
		"trans:0.9 ior:1.3 spec:0.7");
	write_sphere(f, (t_vec){4, 1, 0}, 2.0, (t_color){255, 230, 230},
		"trans:0.8 ior:1.7 spec:0.8 refl:0.15");
	write_sphere(f, (t_vec){-1, 0.6, 5}, 1.2, (t_color){255, 50, 50}, NULL);
	write_sphere(f, (t_vec){2, 0.6, 4}, 1.2, (t_color){50, 50, 255}, NULL);
	write_sphere(f, (t_vec){0, 0.6, 6}, 1.2, (t_color){50, 200, 50}, NULL);
}

/* ---- Cathedral Ruins helpers ---- */

static void cathedral_column(FILE *f, double x, double z, double height)
{
	t_color shaft = {200, 185, 160};
	t_color base = {190, 175, 150};

	write_cylinder_mat(f, (t_vec){x, 0, z}, (t_vec){0, 1, 0},
		1.4, height, shaft, "spec:0.2");
	write_disk(f, (t_vec){x, 0.01, z}, (t_vec){0, 1, 0}, 2.4, base, NULL);
	if (height >= 10.0)
		write_sphere(f, (t_vec){x, height, z}, 1.6, shaft, "spec:0.2");
}

static void cathedral_walls(FILE *f)
{
	t_color s1 = {190, 175, 150};
	t_color s2 = {185, 170, 145};
	t_color s3 = {195, 180, 155};

	write_box(f, (t_vec){-12, 6, -10}, (t_vec){1, 12, 40}, s1);
	write_box(f, (t_vec){-12, 14, -25}, (t_vec){1, 4, 10}, s2);
	write_box(f, (t_vec){-12, 3, 8}, (t_vec){1, 6, 8}, s3);
	write_box(f, (t_vec){12, 7, -10}, (t_vec){1, 14, 40}, s1);
	write_box(f, (t_vec){12, 16, -5}, (t_vec){1, 2, 15}, s2);
	write_box(f, (t_vec){0, 7, -30}, (t_vec){10, 14, 1},
		(t_color){200, 185, 160});
	write_box(f, (t_vec){-6, 8, -29.5}, (t_vec){2, 16, 1}, s3);
	write_box(f, (t_vec){6, 8, -29.5}, (t_vec){2, 16, 1}, s3);
	write_box(f, (t_vec){-9, 5, 15}, (t_vec){3, 10, 1}, s1);
	write_box(f, (t_vec){9, 5, 15}, (t_vec){3, 10, 1}, s1);
	write_box(f, (t_vec){0, 15, 15}, (t_vec){12, 3, 1}, s2);
	write_box(f, (t_vec){-12, 5, -15}, (t_vec){3, 10, 1}, s3);
	write_box(f, (t_vec){12, 5, -15}, (t_vec){3, 10, 1}, s3);
	write_box(f, (t_vec){-12, 5, -16}, (t_vec){1, 10, 3}, s1);
	write_box(f, (t_vec){12, 5, -16}, (t_vec){1, 10, 3}, s1);
	write_box(f, (t_vec){-12.5, 4, 0}, (t_vec){1.5, 8, 2}, s2);
	write_box(f, (t_vec){12.5, 4, 0}, (t_vec){1.5, 8, 2}, s2);
	write_box(f, (t_vec){-12.5, 4, -20}, (t_vec){1.5, 8, 2},
		(t_color){180, 165, 140});
	write_box(f, (t_vec){12.5, 4, -20}, (t_vec){1.5, 8, 2},
		(t_color){180, 165, 140});
}

static void cathedral_arches(FILE *f)
{
	t_color arch = {205, 190, 165};
	t_color big = {210, 195, 170};
	double left_z[] = {7.5, 2.5, -2.5, -7.5, -17.5, -22.5};
	double right_z[] = {7.5, 2.5, -7.5, -10.0, -17.5, -22.5};
	int i;

	i = 0;
	while (i < 6)
	{
		write_torus(f, (t_vec){-6, 14, left_z[i]}, (t_vec){0, 0, 1},
			2.5, 0.4, arch, "spec:0.1");
		write_torus(f, (t_vec){6, 14, right_z[i]}, (t_vec){0, 0, 1},
			2.5, 0.4, arch, "spec:0.1");
		i++;
	}
	write_torus(f, (t_vec){0, 15, -15}, (t_vec){1, 0, 0}, 6, 0.5, big,
		"spec:0.15");
	write_torus(f, (t_vec){0, 15, -5}, (t_vec){1, 0, 0}, 6, 0.5, big,
		"spec:0.15");
	write_torus(f, (t_vec){0, 15, 5}, (t_vec){1, 0, 0}, 6, 0.5, big,
		"spec:0.15");
	write_torus(f, (t_vec){0, 14, -28}, (t_vec){1, 0, 0}, 5, 0.5, big,
		"spec:0.15");
	write_torus(f, (t_vec){-3, 1, -12}, (t_vec){0.3, 0.1, 0.7},
		2.0, 0.3, (t_color){195, 180, 155}, NULL);
	write_torus(f, (t_vec){5, 0.5, 3}, (t_vec){0.5, 0.2, 0.6},
		1.8, 0.3, (t_color){190, 175, 150}, NULL);
}

static void cathedral_rose_window(FILE *f)
{
	t_color outer[] = {
		{200, 50, 50}, {220, 100, 30}, {230, 200, 50}, {50, 200, 50},
		{50, 150, 200}, {80, 50, 200}, {150, 50, 200}, {200, 50, 150},
		{50, 100, 220}, {100, 200, 100}, {220, 180, 50}, {200, 80, 80}};
	t_color inner[] = {
		{255, 220, 100}, {100, 180, 255}, {255, 100, 100},
		{100, 255, 150}, {200, 100, 255}, {255, 200, 100}};
	double cx = 0.0;
	double cy = 12.0;
	double cz = -29.3;
	double r = 3.2;
	double ri = 1.3;
	double angle;
	int i;

	write_torus(f, (t_vec){0, 12, -29.2}, (t_vec){0, 0, 1}, 3.5, 0.3,
		(t_color){180, 170, 155}, "spec:0.3");
	write_torus(f, (t_vec){0, 12, -29.2}, (t_vec){0, 0, 1}, 2.2, 0.2,
		(t_color){175, 165, 150}, "spec:0.3");
	i = 0;
	while (i < 12)
	{
		angle = (double)i * PI / 6.0;
		write_sphere(f, (t_vec){cx + r * sin(angle), cy + r * cos(angle), cz},
			0.8, outer[i], "trans:0.7 ior:1.3");
		i++;
	}
	i = 0;
	while (i < 6)
	{
		angle = (double)i * PI / 3.0;
		write_sphere(f, (t_vec){cx + ri * sin(angle), cy + ri * cos(angle),
			cz}, 0.5, inner[i], "trans:0.8 ior:1.5");
		i++;
	}
	write_sphere(f, (t_vec){0, 12, -29.3}, 0.7,
		(t_color){255, 255, 220}, "trans:0.85 ior:1.5\tspec:0.5");
}

static void cathedral_altar(FILE *f)
{
	t_color gold = {150, 120, 40};

	write_box_mat(f, (t_vec){0, 1.5, -27}, (t_vec){3, 1.5, 1.5},
		(t_color){220, 210, 190}, "spec:0.3");
	write_box(f, (t_vec){0, 0.5, -27}, (t_vec){3.5, 0.5, 2},
		(t_color){210, 200, 180});
	write_cylinder_mat(f, (t_vec){-1, 2.25, -27}, (t_vec){0, 1, 0}, 0.15,
		1.5, (t_color){240, 230, 200}, "spec:0.4");
	write_sphere(f, (t_vec){-1, 3.75, -27}, 0.25,
		(t_color){255, 220, 100}, "spec:0.8");
	write_cylinder_mat(f, (t_vec){1, 2.25, -27}, (t_vec){0, 1, 0}, 0.15,
		1.5, (t_color){240, 230, 200}, "spec:0.4");
	write_sphere(f, (t_vec){1, 3.75, -27}, 0.25,
		(t_color){255, 220, 100}, "spec:0.8");
	write_cylinder_mat(f, (t_vec){0, 2.25, -27.5}, (t_vec){0, 1, 0}, 0.2,
		3.0, gold, "spec:0.6 refl:0.2");
	write_cylinder_mat(f, (t_vec){0, 4.0, -27.5}, (t_vec){1, 0, 0}, 0.2,
		1.5, gold, "spec:0.6 refl:0.2");
	write_box(f, (t_vec){0, 0.15, -25.5}, (t_vec){5, 0.15, 1},
		(t_color){200, 190, 170});
	write_box(f, (t_vec){0, 0.30, -24.5}, (t_vec){6, 0.15, 1},
		(t_color){200, 190, 170});
}

static void cathedral_debris(FILE *f, int seed)
{
	int i;
	double x;
	double z;
	double sz;
	int base_r;

	write_cylinder(f, (t_vec){-4, 0.5, -11}, (t_vec){0.2, 0.1, 0.9},
		1.3, 3.0, (t_color){195, 180, 155});
	write_cylinder(f, (t_vec){-3, 0.6, -9}, (t_vec){0.8, 0.1, 0.3},
		1.3, 2.5, (t_color){200, 185, 160});
	write_cylinder(f, (t_vec){5, 0.5, 1}, (t_vec){0.1, 0.05, 0.95},
		1.3, 4.0, (t_color){190, 175, 150});
	write_cylinder(f, (t_vec){7, 0.6, -18}, (t_vec){0.7, 0.15, 0.5},
		1.2, 2.0, (t_color){195, 180, 155});
	srand(seed);
	i = 0;
	while (i < 15)
	{
		x = randf_range(-9.0, 9.0);
		z = randf_range(-25.0, 9.0);
		sz = randf_range(0.4, 1.0);
		base_r = 175 + (int)(randf() * 15);
		write_box(f, (t_vec){x, sz / 2.0, z},
			(t_vec){sz, sz * randf_range(0.5, 1.0), sz * randf_range(0.5, 1.0)},
			(t_color){base_r, base_r - 15, base_r - 30});
		i++;
	}
	i = 0;
	while (i < 15)
	{
		x = randf_range(-9.0, 9.0);
		z = randf_range(-25.0, 9.0);
		sz = randf_range(0.2, 0.4);
		base_r = 170 + (int)(randf() * 15);
		write_sphere(f, (t_vec){x, sz / 2.0, z}, sz,
			(t_color){base_r, base_r - 15, base_r - 30}, NULL);
		i++;
	}
	write_triangle(f, (t_vec){-3.5, 0.1, -10}, (t_vec){-2.5, 0.8, -10.5},
		(t_vec){-3, 0.1, -11}, (t_color){185, 170, 145});
	write_triangle(f, (t_vec){4, 0.1, -12}, (t_vec){5, 0.6, -11.5},
		(t_vec){4.5, 0.1, -12.5}, (t_color){180, 165, 140});
	write_triangle(f, (t_vec){-7, 0.1, -5}, (t_vec){-6.5, 0.5, -4.5},
		(t_vec){-7.5, 0.1, -5.5}, (t_color){175, 160, 135});
	write_triangle(f, (t_vec){7.5, 0.1, -8}, (t_vec){8, 0.7, -7.5},
		(t_vec){7, 0.1, -8.5}, (t_color){185, 170, 145});
	write_triangle(f, (t_vec){-1.5, 0.1, 7}, (t_vec){-0.5, 0.6, 6.5},
		(t_vec){-1, 0.1, 7.5}, (t_color){180, 165, 140});
	write_triangle(f, (t_vec){2.5, 0.1, 2.5}, (t_vec){3.5, 0.5, 2},
		(t_vec){3, 0.1, 3}, (t_color){175, 160, 135});
	write_sphere(f, (t_vec){-6, 0.6, -10.5}, 0.7,
		(t_color){195, 180, 155}, NULL);
	write_sphere(f, (t_vec){6, 0.5, 0.5}, 0.6,
		(t_color){200, 185, 160}, NULL);
	write_sphere(f, (t_vec){-5.5, 0.4, -20.5}, 0.5,
		(t_color){190, 175, 150}, NULL);
	write_sphere(f, (t_vec){6.5, 0.55, -15.5}, 0.65,
		(t_color){195, 180, 155}, NULL);
}

static void cathedral_roof(FILE *f)
{
	t_color r1 = {175, 165, 145};
	t_color r2 = {180, 170, 150};

	write_triangle(f, (t_vec){-6, 14, -8}, (t_vec){-2, 10, -6},
		(t_vec){-6, 14, -4}, r1);
	write_triangle(f, (t_vec){-2, 10, -6}, (t_vec){2, 10, -6},
		(t_vec){0, 16, -6}, r2);
	write_triangle(f, (t_vec){6, 14, -8}, (t_vec){2, 10, -6},
		(t_vec){6, 14, -4}, r1);
	write_triangle(f, (t_vec){-6, 15, 2}, (t_vec){-3, 12, 3},
		(t_vec){-6, 15, 4}, r2);
	write_triangle(f, (t_vec){6, 15, 2}, (t_vec){3, 12, 3},
		(t_vec){6, 15, 4}, r1);
	write_triangle(f, (t_vec){-4, 16, -20}, (t_vec){0, 18, -20},
		(t_vec){4, 16, -20}, r2);
	write_square(f, (t_vec){-4, 10, -10}, (t_vec){0.3, 0.7, 0.1}, 4,
		(t_color){170, 160, 140});
	write_square(f, (t_vec){5, 11, -3}, (t_vec){0.2, 0.8, 0.3}, 3.5,
		(t_color){175, 165, 145});
	write_square(f, (t_vec){-3, 9, 5}, (t_vec){0.4, 0.6, 0.2}, 3,
		(t_color){165, 155, 135});
	write_triangle(f, (t_vec){-8, 0.2, -8}, (t_vec){-5, 0.2, -7},
		(t_vec){-7, 0.2, -10}, (t_color){170, 160, 140});
	write_triangle(f, (t_vec){3, 0.2, 4}, (t_vec){5, 0.2, 3},
		(t_vec){4, 0.2, 6}, (t_color){165, 155, 135});
	write_triangle(f, (t_vec){-2, 0.2, -18}, (t_vec){1, 0.2, -17},
		(t_vec){-1, 0.2, -20}, (t_color){175, 165, 145});
	write_triangle(f, (t_vec){7, 0.2, -20}, (t_vec){9, 0.2, -19},
		(t_vec){8, 0.2, -22}, (t_color){170, 160, 140});
	write_box(f, (t_vec){0, 17, -5}, (t_vec){0.4, 0.4, 15},
		(t_color){190, 180, 160});
	write_box(f, (t_vec){0, 17, -22}, (t_vec){0.4, 0.4, 6},
		(t_color){185, 175, 155});
	write_cylinder(f, (t_vec){-3, 15.5, -5}, (t_vec){0.5, 0.3, 0}, 0.3,
		7.0, (t_color){195, 185, 165});
	write_cylinder(f, (t_vec){3, 15.5, -5}, (t_vec){0.5, 0.3, 0}, 0.3,
		7.0, (t_color){195, 185, 165});
	write_cylinder(f, (t_vec){-3, 15.5, -20}, (t_vec){0.5, 0.3, 0}, 0.3,
		7.0, (t_color){190, 180, 160});
}

static void cathedral_tower(FILE *f)
{
	write_cylinder_mat(f, (t_vec){-14, 0, 14}, (t_vec){0, 1, 0}, 5.0, 18.0,
		(t_color){195, 180, 155}, "spec:0.15");
	write_cylinder_mat(f, (t_vec){-14, 18, 14}, (t_vec){0, 1, 0}, 4.5, 5.0,
		(t_color){190, 175, 150}, "spec:0.15");
	write_cone_mat(f, (t_vec){-14, 23, 14}, (t_vec){0, 1, 0}, 5.0, 4.0,
		(t_color){120, 100, 80}, "spec:0.2");
	write_box(f, (t_vec){-11.8, 12, 14}, (t_vec){0.5, 2, 1},
		(t_color){60, 55, 50});
	write_box(f, (t_vec){-14, 12, 11.8}, (t_vec){1, 2, 0.5},
		(t_color){60, 55, 50});
	write_box(f, (t_vec){-11.8, 8, 14}, (t_vec){0.5, 1.5, 0.8},
		(t_color){65, 60, 55});
	write_box(f, (t_vec){-14, 8, 11.8}, (t_vec){0.8, 1.5, 0.5},
		(t_color){65, 60, 55});
	write_box(f, (t_vec){-12, 23, 14}, (t_vec){0.8, 1, 0.8},
		(t_color){200, 185, 160});
	write_box(f, (t_vec){-14, 23, 12}, (t_vec){0.8, 1, 0.8},
		(t_color){200, 185, 160});
	write_box(f, (t_vec){-16, 23, 14}, (t_vec){0.8, 1, 0.8},
		(t_color){195, 180, 155});
	write_box(f, (t_vec){-14, 23, 16}, (t_vec){0.8, 1, 0.8},
		(t_color){195, 180, 155});
	write_cylinder(f, (t_vec){-10, 0.4, 11}, (t_vec){0.6, 0.1, 0.7}, 2.0,
		3.0, (t_color){190, 175, 150});
	write_sphere(f, (t_vec){-11, 0.5, 10}, 0.8,
		(t_color){185, 170, 145}, NULL);
}

static void cathedral_atmosphere(FILE *f)
{
	int i;
	double dust_x[] = {-2, -1, 0.5, -0.5, 1};
	double dust_y[] = {8, 10, 9, 11, 7};
	double dust_z[] = {-4, -3, -5, -6, -2};
	double dust_sz[] = {0.15, 0.12, 0.10, 0.13, 0.11};

	i = 0;
	while (i < 5)
	{
		write_sphere(f, (t_vec){dust_x[i], dust_y[i], dust_z[i]}, dust_sz[i],
			(t_color){255, 250, 230}, "trans:0.9 ior:1.0");
		i++;
	}
	write_disk(f, (t_vec){-2, 0.02, -8}, (t_vec){0, 1, 0}, 3.0,
		(t_color){100, 110, 130}, "refl:0.6 spec:0.8");
	write_disk(f, (t_vec){3, 0.02, -3}, (t_vec){0, 1, 0}, 2.5,
		(t_color){95, 105, 125}, "refl:0.6 spec:0.8");
	write_disk(f, (t_vec){-5, 0.02, 4}, (t_vec){0, 1, 0}, 2.0,
		(t_color){100, 110, 130}, "refl:0.5 spec:0.7");
	write_disk(f, (t_vec){7, 0.02, -15}, (t_vec){0, 1, 0}, 1.8,
		(t_color){95, 105, 125}, "refl:0.5 spec:0.7");
	write_disk(f, (t_vec){0, 0.02, -18}, (t_vec){0, 1, 0}, 3.5,
		(t_color){100, 115, 135}, "refl:0.7 spec:0.9");
	write_disk(f, (t_vec){-8, 0.02, -12}, (t_vec){0, 1, 0}, 1.5,
		(t_color){90, 100, 120}, "refl:0.5 spec:0.7");
	write_disk(f, (t_vec){4, 0.02, 8}, (t_vec){0, 1, 0}, 2.2,
		(t_color){100, 110, 130}, "refl:0.6 spec:0.8");
	write_disk(f, (t_vec){-1, 0.02, -24}, (t_vec){0, 1, 0}, 2.8,
		(t_color){95, 110, 130}, "refl:0.65 spec:0.85");
}

static void cathedral_stained_glass(FILE *f)
{
	write_square_mat(f, (t_vec){-11.5, 10, -5}, (t_vec){1, 0, 0}, 2,
		(t_color){180, 40, 40}, "trans:0.75 ior:1.4");
	write_square_mat(f, (t_vec){-11.5, 10, -15}, (t_vec){1, 0, 0}, 2,
		(t_color){40, 80, 180}, "trans:0.75 ior:1.4");
	write_square_mat(f, (t_vec){11.5, 10, -10}, (t_vec){1, 0, 0}, 2.5,
		(t_color){40, 160, 80}, "trans:0.7 ior:1.4");
	write_square_mat(f, (t_vec){11.5, 10, 0}, (t_vec){1, 0, 0}, 2,
		(t_color){180, 160, 40}, "trans:0.75 ior:1.4");
	write_square_mat(f, (t_vec){11.5, 10, -20}, (t_vec){1, 0, 0}, 2,
		(t_color){160, 40, 160}, "trans:0.7 ior:1.4");
	write_ellipsoid(f, (t_vec){-3, 0.1, -6}, (t_vec){0.5, 0.05, 0.3},
		(t_color){200, 50, 50}, "trans:0.6 ior:1.3");
	write_ellipsoid(f, (t_vec){2, 0.1, -14}, (t_vec){0.4, 0.05, 0.4},
		(t_color){50, 50, 200}, "trans:0.6 ior:1.3");
	write_ellipsoid(f, (t_vec){-6, 0.1, -19}, (t_vec){0.3, 0.05, 0.5},
		(t_color){50, 180, 50}, "trans:0.6 ior:1.3");
	write_ellipsoid(f, (t_vec){4, 0.1, 5}, (t_vec){0.5, 0.05, 0.3},
		(t_color){200, 180, 50}, "trans:0.6 ior:1.3");
	write_ellipsoid(f, (t_vec){-1, 0.1, -2}, (t_vec){0.4, 0.05, 0.4},
		(t_color){180, 50, 180}, "trans:0.6 ior:1.3");
	write_square_mat(f, (t_vec){0, 11, -29.5}, (t_vec){0, 0, 1}, 3,
		(t_color){240, 220, 180}, "trans:0.5 ior:1.4 spec:0.3");
	write_sphere(f, (t_vec){-11.5, 12.5, -5}, 0.5,
		(t_color){220, 60, 60}, "trans:0.6 ior:1.3");
	write_sphere(f, (t_vec){-11.5, 12.5, -15}, 0.5,
		(t_color){60, 100, 220}, "trans:0.6 ior:1.3");
	write_sphere(f, (t_vec){11.5, 12.5, -10}, 0.5,
		(t_color){60, 200, 100}, "trans:0.6 ior:1.3");
	write_sphere(f, (t_vec){11.5, 12.5, 0}, 0.5,
		(t_color){220, 200, 60}, "trans:0.6 ior:1.3");
}

static void cathedral_decorations(FILE *f)
{
	write_paraboloid(f, (t_vec){-3, 0, 10}, (t_vec){0, 1, 0}, 0.6, 1.5,
		(t_color){200, 190, 170}, "spec:0.3");
	write_disk(f, (t_vec){-3, 1.5, 10}, (t_vec){0, 1, 0}, 1.8,
		(t_color){180, 190, 210}, "refl:0.3 spec:0.5");
	write_box_mat(f, (t_vec){3, 0.2, -22}, (t_vec){2.5, 0.2, 1.2},
		(t_color){170, 165, 155}, "spec:0.15");
	write_sphere(f, (t_vec){3, 0.45, -22}, 0.3,
		(t_color){160, 155, 145}, NULL);
	write_cylinder_mat(f, (t_vec){-2, 0, -20}, (t_vec){0, 1, 0}, 0.2, 4.0,
		(t_color){80, 60, 50}, "spec:0.7 refl:0.3");
	write_disk(f, (t_vec){-2, 4, -20}, (t_vec){0, 1, 0}, 1.2,
		(t_color){80, 60, 50}, "spec:0.7 refl:0.3");
	write_sphere(f, (t_vec){-2, 4.2, -20}, 0.15,
		(t_color){255, 200, 80}, "spec:0.9");
	write_sphere(f, (t_vec){-1.5, 4.2, -20}, 0.15,
		(t_color){255, 200, 80}, "spec:0.9");
	write_sphere(f, (t_vec){-2.5, 4.2, -20}, 0.15,
		(t_color){255, 200, 80}, "spec:0.9");
	write_ellipsoid(f, (t_vec){-11.3, 3, 5}, (t_vec){0.8, 1.5, 0.5},
		(t_color){40, 80, 30}, NULL);
	write_ellipsoid(f, (t_vec){-11.3, 5, 4}, (t_vec){0.6, 1.2, 0.4},
		(t_color){35, 75, 25}, NULL);
	write_ellipsoid(f, (t_vec){-11.3, 2, 6}, (t_vec){0.7, 1, 0.6},
		(t_color){45, 85, 35}, NULL);
	write_ellipsoid(f, (t_vec){11.3, 4, -7}, (t_vec){0.5, 1.3, 0.5},
		(t_color){40, 80, 30}, NULL);
	write_ellipsoid(f, (t_vec){11.3, 6, -8}, (t_vec){0.6, 1, 0.4},
		(t_color){35, 70, 25}, NULL);
}

static void gen_cathedral_ruins(FILE *f, t_config *cfg)
{
	double left_h[] = {14, 14, 14, 14, 8, 14, 5, 14};
	double right_h[] = {14, 14, 6, 14, 14, 10, 14, 14};
	double col_z;
	int i;

	fprintf(f, "R\t%d\t%d\n", cfg->width, cfg->height);
	fprintf(f, "A\t0.08\t200,190,170\n\n");
	fprintf(f, "c\t0.0,12.0,25.0\t0.055,0,0\t70\n");
	fprintf(f, "c\t-4.0,7.0,18.0\t0.04,-0.03,0\t70\n");
	fprintf(f, "c\t8.0,6.0,-15.0\t0.03,0.3,0\t70\n");
	fprintf(f, "c\t0.0,2.0,16.0\t0.0,0,0\t70\n\n");
	write_light(f, (t_vec){-3, 25, -5}, 0.9, (t_color){255, 240, 200});
	write_light(f, (t_vec){5, 20, 5}, 0.6, (t_color){255, 235, 190});
	write_light(f, (t_vec){-8, 15, -15}, 0.5, (t_color){255, 220, 180});
	write_light(f, (t_vec){0, 12, -25}, 0.4, (t_color){255, 200, 150});
	write_light(f, (t_vec){10, 18, -10}, 0.3, (t_color){200, 210, 255});
	write_light(f, (t_vec){0, 3, 10}, 0.2, (t_color){180, 190, 210});
	fprintf(f, "\n");
	write_plane(f, (t_vec){0, 0, 0}, (t_vec){0, 1, 0},
		(t_color){170, 160, 140});
	fprintf(f, "\n");
	write_square(f, (t_vec){-2, 0.01, -20}, (t_vec){0, 1, 0}, 8,
		(t_color){140, 130, 110});
	write_square(f, (t_vec){3, 0.01, -8}, (t_vec){0, 1, 0}, 6,
		(t_color){155, 145, 125});
	write_square(f, (t_vec){-5, 0.01, 0}, (t_vec){0, 1, 0}, 5,
		(t_color){145, 135, 120});
	write_square(f, (t_vec){6, 0.01, -15}, (t_vec){0, 1, 0}, 7,
		(t_color){160, 150, 130});
	fprintf(f, "\n");
	cathedral_walls(f);
	fprintf(f, "\n");
	i = 0;
	while (i < 8)
	{
		col_z = 10.0 - (double)i * 5.0;
		cathedral_column(f, -6.0, col_z, left_h[i]);
		i++;
	}
	fprintf(f, "\n");
	i = 0;
	while (i < 8)
	{
		col_z = 10.0 - (double)i * 5.0;
		cathedral_column(f, 6.0, col_z, right_h[i]);
		i++;
	}
	fprintf(f, "\n");
	cathedral_arches(f);
	fprintf(f, "\n");
	cathedral_rose_window(f);
	fprintf(f, "\n");
	cathedral_altar(f);
	fprintf(f, "\n");
	cathedral_debris(f, cfg->seed);
	fprintf(f, "\n");
	cathedral_roof(f);
	fprintf(f, "\n");
	cathedral_tower(f);
	fprintf(f, "\n");
	cathedral_atmosphere(f);
	fprintf(f, "\n");
	cathedral_stained_glass(f);
	fprintf(f, "\n");
	cathedral_decorations(f);
}

static void print_usage(void)
{
	printf("Usage: gen_scene [OPTIONS]\n\n");
	printf("Options:\n");
	printf("  --preset NAME    Preset scene (cornell, checkerboard, solar,\n");
	printf("                   spiral, grid, scatter, reflections, glass,\n");
	printf("                   cathedral)\n");
	printf("  --width N        Image width (default: 1920)\n");
	printf("  --height N       Image height (default: 1080)\n");
	printf("  --objects N      Number of objects (default: 20)\n");
	printf("  --radius R       Scene radius (default: 10.0)\n");
	printf("  --seed N         Random seed (default: time-based)\n");
	printf("  --output FILE    Output file (default: stdout)\n");
	printf("  --list           List available presets\n");
	printf("  --help           Show this help\n");
}

static void list_presets(void)
{
	printf("Available presets:\n");
	printf("  cornell      - Cornell box with reflective/refractive spheres\n");
	printf("  checkerboard - 5x5 grid of colored spheres on a floor\n");
	printf("  solar        - Solar system with 8 planets\n");
	printf("  spiral       - Spiral arrangement of spheres\n");
	printf("  grid         - Grid of mixed shapes (spheres, boxes, cylinders)\n");
	printf("  scatter      - Random scatter of mixed shapes\n");
	printf("  reflections  - Showcase of reflective and refractive materials\n");
	printf("  glass        - Glass/transparent sphere showcase\n");
	printf("  cathedral    - Medieval cathedral ruins with columns, arches,"
		" rose window\n");
}

static void generate_scene(FILE *f, t_config *cfg)
{
	if (strcmp(cfg->preset, "cornell") == 0)
		gen_cornell_box(f, cfg);
	else if (strcmp(cfg->preset, "checkerboard") == 0)
		gen_checkerboard_spheres(f, cfg);
	else if (strcmp(cfg->preset, "solar") == 0)
		gen_solar_system(f, cfg);
	else if (strcmp(cfg->preset, "spiral") == 0)
		gen_spiral(f, cfg);
	else if (strcmp(cfg->preset, "grid") == 0)
		gen_grid(f, cfg);
	else if (strcmp(cfg->preset, "scatter") == 0)
		gen_random_scatter(f, cfg);
	else if (strcmp(cfg->preset, "reflections") == 0)
		gen_reflections(f, cfg);
	else if (strcmp(cfg->preset, "glass") == 0)
		gen_glass_spheres(f, cfg);
	else if (strcmp(cfg->preset, "cathedral") == 0)
		gen_cathedral_ruins(f, cfg);
	else
	{
		fprintf(stderr, "Error: unknown preset '%s'\n", cfg->preset);
		fprintf(stderr, "Use --list to see available presets.\n");
		exit(1);
	}
}

int main(int ac, char **av)
{
	t_config cfg;
	FILE *out;
	char *outfile;
	int i;

	cfg.width = 1920;
	cfg.height = 1080;
	cfg.num_objects = 20;
	cfg.scene_radius = 10.0;
	cfg.seed = (int)time(NULL);
	strcpy(cfg.preset, "checkerboard");
	out = stdout;
	outfile = NULL;
	i = 1;
	while (i < ac)
	{
		if (strcmp(av[i], "--help") == 0 || strcmp(av[i], "-h") == 0)
		{
			print_usage();
			return (0);
		}
		else if (strcmp(av[i], "--list") == 0)
		{
			list_presets();
			return (0);
		}
		else if (strcmp(av[i], "--preset") == 0 && i + 1 < ac)
			strncpy(cfg.preset, av[++i], 63);
		else if (strcmp(av[i], "--width") == 0 && i + 1 < ac)
			cfg.width = atoi(av[++i]);
		else if (strcmp(av[i], "--height") == 0 && i + 1 < ac)
			cfg.height = atoi(av[++i]);
		else if (strcmp(av[i], "--objects") == 0 && i + 1 < ac)
			cfg.num_objects = atoi(av[++i]);
		else if (strcmp(av[i], "--radius") == 0 && i + 1 < ac)
			cfg.scene_radius = atof(av[++i]);
		else if (strcmp(av[i], "--seed") == 0 && i + 1 < ac)
			cfg.seed = atoi(av[++i]);
		else if (strcmp(av[i], "--output") == 0 && i + 1 < ac)
			outfile = av[++i];
		else
		{
			fprintf(stderr, "Unknown option: %s\n", av[i]);
			print_usage();
			return (1);
		}
		i++;
	}
	if (outfile)
	{
		out = fopen(outfile, "w");
		if (!out)
		{
			fprintf(stderr, "Error: cannot open '%s'\n", outfile);
			return (1);
		}
	}
	generate_scene(out, &cfg);
	if (outfile)
		fclose(out);
	return (0);
}
