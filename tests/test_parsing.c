#include "mini_rt.h"
#include "minirt_test.h"

/* ========== ft_which_id tests ========== */

TEST(test_which_id_sphere)
{
	ASSERT_TRUE(ft_which_id("sp 0,0,0 1.0 255,0,0") == 's');
}

TEST(test_which_id_plane)
{
	ASSERT_TRUE(ft_which_id("pl 0,0,0 0,1,0 255,255,255") == 'p');
}

TEST(test_which_id_square)
{
	ASSERT_TRUE(ft_which_id("sq 0,0,0 0,0,1 2.0 0,255,0") == 'q');
}

TEST(test_which_id_cylinder)
{
	ASSERT_TRUE(ft_which_id("cy 0,0,0 0,1,0 1.0 3.0 0,255,0") == 'y');
}

TEST(test_which_id_triangle)
{
	ASSERT_TRUE(ft_which_id("tr -1,0,0 1,0,0 0,1,0 0,0,255") == 't');
}

TEST(test_which_id_cone)
{
	ASSERT_TRUE(ft_which_id("co 0,0,0 0,1,0 1.0 3.0 0,255,0") == 'o');
}

TEST(test_which_id_disk)
{
	ASSERT_TRUE(ft_which_id("dk 0,0,0 0,0,1 2.0 255,0,0") == 'k');
}

TEST(test_which_id_ellipsoid)
{
	ASSERT_TRUE(ft_which_id("el 0,0,0 2,1,1 255,0,0") == 'e');
}

TEST(test_which_id_box)
{
	ASSERT_TRUE(ft_which_id("bx 0,0,0 2,2,2 255,0,0") == 'b');
}

TEST(test_which_id_torus)
{
	ASSERT_TRUE(ft_which_id("to 0,0,0 0,1,0 2.0 0.5 255,0,0") == 'u');
}

TEST(test_which_id_hyperboloid)
{
	ASSERT_TRUE(ft_which_id("hy 0,0,0 0,1,0 1,1,0 4.0 255,0,0") == 'h');
}

TEST(test_which_id_paraboloid)
{
	ASSERT_TRUE(ft_which_id("pa 0,0,0 0,1,0 0.5 4.0 255,0,0") == 'a');
}

TEST(test_which_id_mesh)
{
	ASSERT_TRUE(ft_which_id("me model.obj 0,0,0 255,0,0") == 'm');
}

TEST(test_which_id_csg)
{
	ASSERT_TRUE(ft_which_id("csg U 0 1 255,0,0") == 'g');
}

TEST(test_which_id_invalid)
{
	ASSERT_TRUE(ft_which_id("xx 0,0,0") == -1);
	ASSERT_TRUE(ft_which_id("") == -1);
	ASSERT_TRUE(ft_which_id("s ") == -1);
	ASSERT_TRUE(ft_which_id("abc") == -1);
}

/* ========== ft_color_check tests ========== */

TEST(test_color_check_valid)
{
	t_argb c = {0, 0, 0, 0};
	ASSERT_TRUE(ft_color_check(NULL, c) == 0);

	t_argb c2 = {0, 255, 255, 255};
	ASSERT_TRUE(ft_color_check(NULL, c2) == 0);

	t_argb c3 = {0, 128, 64, 32};
	ASSERT_TRUE(ft_color_check(NULL, c3) == 0);
}

TEST(test_color_check_invalid)
{
	t_argb c1 = {0, -1, 0, 0};
	ASSERT_TRUE(ft_color_check(NULL, c1) == 9);

	t_argb c2 = {0, 0, 256, 0};
	ASSERT_TRUE(ft_color_check(NULL, c2) == 9);

	t_argb c3 = {0, 0, 0, 300};
	ASSERT_TRUE(ft_color_check(NULL, c3) == 9);
}

/* ========== ft_ori_check tests ========== */

TEST(test_ori_check_valid)
{
	t_pt o1 = {0, 0, 0};
	ASSERT_TRUE(ft_ori_check(NULL, o1) == 0);

	t_pt o2 = {1, -1, 0.5};
	ASSERT_TRUE(ft_ori_check(NULL, o2) == 0);

	t_pt o3 = {-1, 1, -1};
	ASSERT_TRUE(ft_ori_check(NULL, o3) == 0);
}

TEST(test_ori_check_invalid)
{
	t_pt o1 = {2, 0, 0};
	ASSERT_TRUE(ft_ori_check(NULL, o1) == 8);

	t_pt o2 = {0, -2, 0};
	ASSERT_TRUE(ft_ori_check(NULL, o2) == 8);

	t_pt o3 = {0, 0, 1.5};
	ASSERT_TRUE(ft_ori_check(NULL, o3) == 8);
}

/* ========== ft_pt_check tests ========== */

TEST(test_pt_check_valid)
{
	t_pt p = {100, -200, 300};
	ASSERT_TRUE(ft_pt_check(NULL, p) == 0);
}

/* ========== ft_iterate_in_line tests ========== */

TEST(test_iterate_in_line)
{
	char str[] = "123.45,67  next";
	char *ptr = str;
	ft_iterate_in_line(&ptr);
	ASSERT_TRUE(*ptr == 'n');
}

TEST(test_iterate_in_line_no_spaces)
{
	char str[] = "123abc";
	char *ptr = str;
	ft_iterate_in_line(&ptr);
	ASSERT_TRUE(*ptr == 'a');
}

/* ========== ft_window_init tests ========== */

TEST(test_window_init)
{
	t_window win;
	ft_window_init(&win);
	ASSERT_TRUE(win.fd == -1);
	ASSERT_TRUE(win.line == NULL);
	ASSERT_TRUE(win.beg_sh == NULL);
	ASSERT_TRUE(win.beg_cam == NULL);
	ASSERT_TRUE(win.beg_light == NULL);
	ASSERT_TRUE(win.mlx_ptr == NULL);
	ASSERT_TRUE(win.x == 0);
	ASSERT_TRUE(win.y == 0);
	ASSERT_DBL_EQ(-1.0, win.ratio);
	ASSERT_TRUE(win.resol == RESOL);
}

/* ========== ft_argb_init tests ========== */

TEST(test_argb_init)
{
	t_argb a;
	a.a = 99;
	a.r = 99;
	a.g = 99;
	a.b = 99;
	ft_argb_init(&a);
	ASSERT_DBL_EQ(0.0, a.a);
	ASSERT_DBL_EQ(0.0, a.r);
	ASSERT_DBL_EQ(0.0, a.g);
	ASSERT_DBL_EQ(0.0, a.b);
}

/* ========== ft_check_resol tests ========== */

TEST(test_check_resol_valid)
{
	t_window win;
	ft_window_init(&win);
	win.x = 1920;
	win.y = 1080;
	ASSERT_TRUE(ft_check_resol(&win) == 0);
	ASSERT_TRUE(win.x == 1920);
	ASSERT_TRUE(win.y == 1080);
}

TEST(test_check_resol_clamped)
{
	t_window win;
	ft_window_init(&win);
	win.x = 5000;
	win.y = 3000;
	ASSERT_TRUE(ft_check_resol(&win) == 0);
	ASSERT_TRUE(win.x == 2560);
	ASSERT_TRUE(win.y == 1440);
}

TEST(test_check_resol_invalid_zero)
{
	t_window win;
	ft_window_init(&win);
	win.x = 0;
	win.y = 1080;
	ASSERT_TRUE(ft_check_resol(&win) != 0);
}

/* ========== ft_check_amb_light tests ========== */

TEST(test_check_amb_light_valid)
{
	t_window win;
	ft_window_init(&win);
	win.ratio = 0.5;
	win.col = (t_argb){0, 128, 128, 128};
	ASSERT_TRUE(ft_check_amb_light(&win) == 0);
}

TEST(test_check_amb_light_no_ratio)
{
	t_window win;
	ft_window_init(&win);
	win.ratio = 0;
	ASSERT_TRUE(ft_check_amb_light(&win) != 0);
}

TEST(test_check_amb_light_bad_ratio)
{
	t_window win;
	ft_window_init(&win);
	win.ratio = 1.5;
	ASSERT_TRUE(ft_check_amb_light(&win) != 0);
}

TEST(test_check_amb_light_bad_color)
{
	t_window win;
	ft_window_init(&win);
	win.ratio = 0.5;
	win.col = (t_argb){0, 300, 0, 0};
	ASSERT_TRUE(ft_check_amb_light(&win) != 0);
}

/* ========== ft_check_cam_parsing tests ========== */

TEST(test_check_cam_valid)
{
	t_window win;
	t_cam cam;

	ft_window_init(&win);
	memset(&cam, 0, sizeof(t_cam));
	cam.coord = ft_pt_create(0, 0, 0);
	cam.ori = ft_pt_create(0, 0, -1);
	cam.fov = 70;
	ASSERT_TRUE(ft_check_cam_parsing(&win, &cam) == 0);
}

TEST(test_check_cam_null)
{
	t_window win;
	ft_window_init(&win);
	ASSERT_TRUE(ft_check_cam_parsing(&win, NULL) != 0);
}

TEST(test_check_cam_bad_fov)
{
	t_window win;
	t_cam cam;

	ft_window_init(&win);
	memset(&cam, 0, sizeof(t_cam));
	cam.fov = 200;
	cam.ori = ft_pt_create(0, 0, -1);
	ASSERT_TRUE(ft_check_cam_parsing(&win, &cam) != 0);
}

TEST(test_check_cam_bad_ori)
{
	t_window win;
	t_cam cam;

	ft_window_init(&win);
	memset(&cam, 0, sizeof(t_cam));
	cam.fov = 70;
	cam.ori = ft_pt_create(5, 0, 0);
	ASSERT_TRUE(ft_check_cam_parsing(&win, &cam) != 0);
}

/* ========== ft_check_light_parsing tests ========== */

TEST(test_check_light_valid)
{
	t_window win;
	t_light light;

	ft_window_init(&win);
	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, 10, 0);
	light.light_ratio = 0.7;
	light.col = (t_argb){0, 255, 255, 255};
	ASSERT_TRUE(ft_check_light_parsing(&win, &light) == 0);
}

TEST(test_check_light_null)
{
	t_window win;
	ft_window_init(&win);
	ASSERT_TRUE(ft_check_light_parsing(&win, NULL) != 0);
}

TEST(test_check_light_bad_ratio)
{
	t_window win;
	t_light light;

	ft_window_init(&win);
	memset(&light, 0, sizeof(t_light));
	light.light_ratio = 2.0;
	light.col = (t_argb){0, 255, 255, 255};
	ASSERT_TRUE(ft_check_light_parsing(&win, &light) != 0);
}

/* ========== ft_check_parsing (integration) ========== */

/* ========== Shape init tests ========== */

static t_shape	*alloc_shape(void)
{
	t_shape *sh;

	sh = ft_calloc(1, sizeof(t_shape));
	return (sh);
}

TEST(test_cone_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "co 0,0,-5 0,1,0 2.0 3.0 255,50,50";
	int ret = ft_cone_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'o');
	ASSERT_DBL_EQ(2.0, sh->diameter);
	ASSERT_DBL_EQ(3.0, sh->height);
	ASSERT_DBL_EQ(255.0, sh->color.r);
	free(sh);
}

TEST(test_disk_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "dk 0,0,-5 0,0,1 4.0 200,100,50";
	int ret = ft_disk_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'k');
	ASSERT_DBL_EQ(4.0, sh->diameter);
	free(sh);
}

TEST(test_torus_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "to 0,0,-5 0,1,0 2.0 0.5 255,0,255";
	int ret = ft_torus_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'u');
	ASSERT_DBL_EQ(2.0, sh->diameter);
	ASSERT_DBL_EQ(0.5, sh->height);
	free(sh);
}

TEST(test_ellipsoid_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "el 0,0,-5 2.0,1.0,1.0 100,200,50";
	int ret = ft_ellipsoid_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'e');
	ASSERT_DBL_EQ(100.0, sh->color.r);
	free(sh);
}

TEST(test_box_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "bx 0,0,-5 2,2,2 255,128,0";
	int ret = ft_box_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'b');
	ASSERT_DBL_EQ(2.0, sh->pt_1.x);
	free(sh);
}

TEST(test_hyperboloid_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "hy 0,0,-5 0,1,0 1.5,2.0,0 6.0 255,100,50";
	int ret = ft_hyperboloid_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'h');
	ASSERT_DBL_EQ(6.0, sh->height);
	free(sh);
}

TEST(test_paraboloid_init_valid)
{
	t_window win;
	t_shape *sh;

	ft_window_init(&win);
	sh = alloc_shape();
	char line[] = "pa 0,0,-5 0,1,0 0.5 4.0 50,200,255";
	int ret = ft_paraboloid_init(&win, &sh, line);
	ASSERT_TRUE(ret == 0);
	ASSERT_TRUE(sh->id == 'a');
	ASSERT_DBL_EQ(0.5, sh->diameter);
	ASSERT_DBL_EQ(4.0, sh->height);
	free(sh);
}

/* ========== Full parsing integration ========== */

TEST(test_check_parsing_full)
{
	t_window win;
	t_cam cam;

	ft_window_init(&win);
	win.x = 640;
	win.y = 480;
	win.ratio = 0.5;
	win.col = (t_argb){0, 255, 255, 255};

	memset(&cam, 0, sizeof(t_cam));
	cam.coord = ft_pt_create(0, 0, 0);
	cam.ori = ft_pt_create(0, 0, -1);
	cam.fov = 70;
	cam.next = NULL;
	win.beg_cam = &cam;

	ft_check_parsing(&win);
	ASSERT_TRUE(win.x == 640);
	ASSERT_TRUE(win.y == 480);
}

void	run_parsing_tests(void);

#ifndef TEST_ALL
int main(void)
{
	run_parsing_tests();
	TEST_REPORT();
}
#endif

void	run_parsing_tests(void)
{
	TEST_SUITE("ft_which_id");
	RUN_TEST(test_which_id_sphere);
	RUN_TEST(test_which_id_plane);
	RUN_TEST(test_which_id_square);
	RUN_TEST(test_which_id_cylinder);
	RUN_TEST(test_which_id_triangle);
	RUN_TEST(test_which_id_cone);
	RUN_TEST(test_which_id_disk);
	RUN_TEST(test_which_id_ellipsoid);
	RUN_TEST(test_which_id_box);
	RUN_TEST(test_which_id_torus);
	RUN_TEST(test_which_id_hyperboloid);
	RUN_TEST(test_which_id_paraboloid);
	RUN_TEST(test_which_id_mesh);
	RUN_TEST(test_which_id_csg);
	RUN_TEST(test_which_id_invalid);

	TEST_SUITE("ft_color_check");
	RUN_TEST(test_color_check_valid);
	RUN_TEST(test_color_check_invalid);

	TEST_SUITE("ft_ori_check");
	RUN_TEST(test_ori_check_valid);
	RUN_TEST(test_ori_check_invalid);

	TEST_SUITE("ft_pt_check");
	RUN_TEST(test_pt_check_valid);

	TEST_SUITE("ft_iterate_in_line");
	RUN_TEST(test_iterate_in_line);
	RUN_TEST(test_iterate_in_line_no_spaces);

	TEST_SUITE("ft_window_init / ft_argb_init");
	RUN_TEST(test_window_init);
	RUN_TEST(test_argb_init);

	TEST_SUITE("ft_check_resol");
	RUN_TEST(test_check_resol_valid);
	RUN_TEST(test_check_resol_clamped);
	RUN_TEST(test_check_resol_invalid_zero);

	TEST_SUITE("ft_check_amb_light");
	RUN_TEST(test_check_amb_light_valid);
	RUN_TEST(test_check_amb_light_no_ratio);
	RUN_TEST(test_check_amb_light_bad_ratio);
	RUN_TEST(test_check_amb_light_bad_color);

	TEST_SUITE("ft_check_cam_parsing");
	RUN_TEST(test_check_cam_valid);
	RUN_TEST(test_check_cam_null);
	RUN_TEST(test_check_cam_bad_fov);
	RUN_TEST(test_check_cam_bad_ori);

	TEST_SUITE("ft_check_light_parsing");
	RUN_TEST(test_check_light_valid);
	RUN_TEST(test_check_light_null);
	RUN_TEST(test_check_light_bad_ratio);

	TEST_SUITE("Shape init parsing");
	RUN_TEST(test_cone_init_valid);
	RUN_TEST(test_disk_init_valid);
	RUN_TEST(test_torus_init_valid);
	RUN_TEST(test_ellipsoid_init_valid);
	RUN_TEST(test_box_init_valid);
	RUN_TEST(test_hyperboloid_init_valid);
	RUN_TEST(test_paraboloid_init_valid);

	TEST_SUITE("ft_check_parsing (integration)");
	RUN_TEST(test_check_parsing_full);
}
