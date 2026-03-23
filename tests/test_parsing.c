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

int	main(void)
{
	TEST_SUITE("ft_which_id");
	RUN_TEST(test_which_id_sphere);
	RUN_TEST(test_which_id_plane);
	RUN_TEST(test_which_id_square);
	RUN_TEST(test_which_id_cylinder);
	RUN_TEST(test_which_id_triangle);
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

	TEST_REPORT();
}
