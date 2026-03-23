/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/29 16:18:11 by alienard          #+#    #+#             */
/*   Updated: 2022/06/14 16:33:43 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
#define MINI_RT_H

#include "get_next_line_bonus.h"
#include "keybinds.h"
#include "libft.h"
#include "libftprintf.h"
#include "mlx.h"
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RESOL 1
#define MAX_THREADS 64

typedef struct s_fileheader
{
	unsigned short int id;
	unsigned int size;
	unsigned int reserved;
	unsigned int offset;
} t_fileheader;

typedef struct s_infoheader
{
	unsigned int headersize;
	unsigned int width;
	unsigned int height;
	unsigned short int pane;
	unsigned short int bitcount;
	unsigned char offset[24];
} t_infoheader;

typedef struct s_pt
{
	double x;
	double y;
	double z;
} t_pt;

typedef struct s_ray
{
	t_pt orig;
	t_pt dir;
	double lenght;
	t_pt ray;
	t_pt unit;
} t_ray;

typedef struct s_mat
{
	t_pt x;
	t_pt y;
	t_pt z;
	t_pt a;
} t_mat;

typedef struct s_argb
{
	double a;
	double r;
	double g;
	double b;
} t_argb;

typedef struct s_cam
{
	t_pt coord;
	t_pt ori;
	int fov;
	t_pt pij;
	t_pt rij;
	struct s_cam *next;
} t_cam;

typedef struct s_light
{
	double light_ratio;
	t_argb col;
	t_pt coord;
	struct s_light *next;
} t_light;

typedef struct s_aabb
{
	t_pt min;
	t_pt max;
} t_aabb;

typedef struct s_bvh_node
{
	t_aabb box;
	struct s_bvh_node *left;
	struct s_bvh_node *right;
	struct s_shape *shape;
} t_bvh_node;

typedef struct s_shape
{
	int id;
	t_argb color;
	t_pt pt_0;
	t_pt pt_1;
	t_pt pt_2;
	t_pt pt_3;
	t_pt ori;
	t_pt n;
	int in;
	double diameter;
	double height;
	double radius_sq;
	double plane_d;
	t_pt tri_edge0;
	t_pt tri_edge1;
	t_pt tri_edge2;
	t_pt tri_norm;
	t_pt cyl_top;
	t_pt cyl_axis;
	double cyl_axis_len_sq;
	struct s_shape *next;
} t_shape;

typedef struct s_window
{
	int fd;
	char *line;
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	char *data;
	int depth;
	int size_line;
	int endian;
	int resol;
	unsigned int x;
	unsigned int y;
	double ratio;
	t_argb col;
	t_cam *beg_cam;
	t_cam *cur_cam;
	t_shape *beg_sh;
	t_light *beg_light;
	t_bvh_node *bvh;
	int num_threads;
} t_window;

typedef struct s_thread_data
{
	t_window *win;
	t_cam *cam;
	int y_start;
	int y_end;
} t_thread_data;

void ft_parse(int *check, t_window *win, int fd);
void ft_parse_resol(int *res, t_window *win, char *line);
void ft_parse_amb(int *amb, t_window *win, char *line);
int ft_which_id(char *line);
void ft_which_shape(t_shape *sh, t_ray *ray);
void ft_iterate_in_line(char **line);

void ft_mlx_init(t_window *win, int ac, char **av);
int ft_window_init(t_window *win);
void ft_argb_init(t_argb *a);
void ft_pt_init(t_pt *a, double x, double y, double z);
int ft_amb_light_init(t_window *amb_light, char *line);
int ft_resol_init(t_window *resol, char *line);
int ft_cam_init(t_window *win, t_cam **begin, char *line);
int ft_light_init(t_window *win, t_light **begin, char *line);
int ft_shape_init(t_window *win, t_shape **begin, char *line);
int ft_color_init(t_window *win, t_argb *col, char **line);
int ft_point_init(t_window *win, t_pt *pt, char **line);
int ft_sphere_init(t_window *win, t_shape **current, char *line);
int ft_plane_init(t_window *win, t_shape **current, char *line);
int ft_square_init(t_window *win, t_shape **current, char *line);
int ft_cylinder_init(t_window *win, t_shape **current, char *line);
int ft_triangle_init(t_window *win, t_shape **current, char *line);

void ft_precompute_shape(t_shape *sh);
void ft_precompute_shapes(t_window *win);
int ft_aabb_hit(t_aabb *box, t_ray *ray, double t_max);
t_aabb ft_shape_aabb(t_shape *sh);
t_bvh_node *ft_bvh_build(t_shape **shapes, int count);
void ft_bvh_trace(t_bvh_node *node, t_ray *ray, double *min, t_shape **min_sh);
void ft_bvh_free(t_bvh_node *node);
void ft_build_scene_bvh(t_window *win);
void ft_check_parsing(t_window *win);
int ft_check_resol(t_window *win);
int ft_check_amb_light(t_window *win);
int ft_check_cam_parsing(t_window *win, t_cam *current);
int ft_check_light_parsing(t_window *win, t_light *current);
int ft_check_shape_parsing(t_window *win, t_shape *current);
int ft_color_check(t_window *win, t_argb color);
int ft_pt_check(t_window *win, t_pt pt);
int ft_ori_check(t_window *win, t_pt ori);
int ft_sphere_check(t_window *win, t_shape **current);
int ft_plane_check(t_window *win, t_shape **current);
int ft_square_check(t_window *win, t_shape **current);
int ft_cylinder_check(t_window *win, t_shape **current);
int ft_triangle_check(t_window *win, t_shape **current);

t_pt ft_pt_create(double x, double y, double z);
void ft_swap_pt(t_pt *a, t_pt *b);

int ft_error(int error, t_window *win, const char *str);
void ft_error_id(t_window *win);
void ft_error_param(int amb, int res, t_window *win);
void ft_cleanup(t_window *win);
int ft_close(t_window *win);
int ft_free_lst_cam(t_window *win);
int ft_free_lst_light(t_window *win);
int ft_free_lst_sh(t_window *win);

int ft_key(int keycode, t_window *param);
void ft_key_moove(int keycode, t_window *param);
void ft_key_look(int keycode, t_window *param);
void ft_key_fov(int keycode, t_window *param);
int ft_mouse(int button, int x, int y, t_window *param);

double ft_sqr(double d);
double ft_dot_product(t_pt a, t_pt b);
double ft_lenght(t_pt a);
t_pt ft_normal_vect(t_pt a);
void ft_orthogonality(t_pt a, t_pt b);
t_pt ft_cross_product(t_pt a, t_pt b);
t_pt ft_addition(t_pt a, t_pt b);
t_pt ft_add_scal(double a, t_pt b);
t_pt ft_subtraction(t_pt a, t_pt b);
t_pt ft_sub_scal(double a, t_pt b);
t_pt ft_multi_vect(t_pt a, t_pt b);
t_pt ft_multi_scal(double a, t_pt b);
t_pt ft_div_vect(t_pt a, t_pt b);
t_pt ft_div_scal(double a, t_pt b);
void ft_inv_pt(t_pt *dir);
t_pt ft_neg_pt(t_pt dir);
t_pt ft_translation(t_pt coord, t_pt m1);
t_pt ft_rotation_vect(t_pt p, t_pt ori);
t_pt ft_rotate_arr_x(t_pt p, double a);
t_pt ft_rotate_arr_y(t_pt p, double a);
t_pt ft_rotate_arr_z(t_pt p, double a);
t_pt ft_rot_angle(t_pt p, double a);
t_pt ft_point_matrix_transl(t_pt p, t_mat m);

t_pt ft_pre_light(t_window *win, t_shape *sh, double clos, t_ray *ray);
t_pt ft_light(t_window *win, t_pt n, t_pt p, t_shape *sh);
double ft_shadow(t_window *win, t_pt n, t_pt p, double light_dist);
int ft_lstsize_light(t_window *win);
t_argb ft_albedo(t_pt a, t_argb col);
t_argb ft_multi_argb(t_argb a, t_argb col);
void ft_pix(int x, int y, t_window *w, t_argb color);
void ft_db_mult_to_add_pt(t_pt *a, double b, t_argb c);

int ft_aff(t_window *win);
void ft_ray(double x, double y, t_window *win, t_cam *cur_cam);
t_argb ft_trace_ray(t_window *win, t_cam *cam);
t_ray ft_shoot_ray(t_pt orig, t_pt dir, double t);
t_ray ft_no_ray(void);
void ft_trace_shapes(t_shape *cur_shape, t_ray *ray, double *min,
					 t_shape **min_sh);
void ft_init_ray_cam(t_cam *cam, t_ray *ray);

void ft_intersect_ray_sphere(t_shape *sh, t_ray *ray);
void ft_intersect_ray_plan(t_shape *sh, t_ray *ray);
void ft_intersect_ray_square(t_shape *sh, t_ray *ray);
void ft_intersect_ray_cylinder(t_shape *sh, t_ray *ray);
void ft_intersect_ray_triangle(t_shape *sh, t_ray *ray);

void ft_shape_norm(t_shape *sh, t_ray *ray);
void ft_sphere_norm(t_shape *sh, t_ray *ray);
void ft_sphere_inv_norm(t_shape *sh, t_ray *ray);
void ft_plane_norm(t_shape *sh);
void ft_square_norm(t_shape *sh);
void ft_cylinder_norm(t_shape *sh, t_ray *ray);
void ft_cylinder_inv_norm(t_shape *sh, t_ray *ray);
void ft_triangle_norm(t_shape *sh);
void ft_inv_norm(t_pt *dir);

double ft_is_in_triangle(t_pt r, t_shape *sh, t_ray *ray);
double ft_is_in_square(t_ray *ray, t_shape *sh, t_pt n);
void ft_cylinder_calc_one(t_shape *sh, t_ray *ray, t_mat *tmp, t_argb *dot);
void ft_cylinder_calc_two(t_pt *calc, t_ray *ray, t_mat *tmp, t_argb *dot);
double ft_cylinder_calc_three(t_pt *calc, t_argb *dist, t_argb *dot,
							  t_ray *ray);
void ft_cylinder_calc_four(t_pt *calc, t_argb *dist, t_argb *dot);
double ft_cylinder_calc_five(t_shape *sh, t_argb *dist, t_argb *dot,
							 t_ray *ray);

int ft_aff_threaded(t_window *win);
int ft_get_num_cores(void);

int ft_save(t_window *win);
int ft_file_header(int fd, t_window *rt);
int ft_info_header(int fd, t_window *rt);
int ft_pixel_data(int fd, t_window *win);

#endif
