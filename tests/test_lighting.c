#include "mini_rt.h"
#include "minirt_test.h"

/* ========== ft_lstsize_light tests ========== */

TEST(test_lstsize_light_empty)
{
	t_window win;
	ft_window_init(&win);
	ASSERT_TRUE(ft_lstsize_light(&win) == 0);
}

TEST(test_lstsize_light_one)
{
	t_window win;
	t_light l1;

	ft_window_init(&win);
	memset(&l1, 0, sizeof(t_light));
	l1.next = NULL;
	win.beg_light = &l1;
	ASSERT_TRUE(ft_lstsize_light(&win) == 1);
}

TEST(test_lstsize_light_multiple)
{
	t_window win;
	t_light l1, l2, l3;

	ft_window_init(&win);
	memset(&l1, 0, sizeof(t_light));
	memset(&l2, 0, sizeof(t_light));
	memset(&l3, 0, sizeof(t_light));
	l3.next = NULL;
	l2.next = &l3;
	l1.next = &l2;
	win.beg_light = &l1;
	ASSERT_TRUE(ft_lstsize_light(&win) == 3);
}

/* ========== ft_shadow tests ========== */

TEST(test_shadow_no_shapes)
{
	t_window win;
	ft_window_init(&win);
	t_pt dir = ft_pt_create(0, 1, 0);
	t_pt origin = ft_pt_create(0, 0, 0);
	double result = ft_shadow(&win, dir, origin, 100.0);
	ASSERT_TRUE(result > 0);
}

TEST(test_shadow_with_blocking_shape)
{
	t_window win;
	t_shape sphere;

	ft_window_init(&win);
	memset(&sphere, 0, sizeof(t_shape));
	sphere.id = 's';
	sphere.pt_0 = ft_pt_create(0, 5, 0);
	sphere.diameter = 2.0;
	sphere.next = NULL;
	ft_precompute_shape(&sphere);
	win.beg_sh = &sphere;

	t_pt dir = ft_normal_vect(ft_pt_create(0, 1, 0));
	t_pt origin = ft_pt_create(0, 0, 0);
	double result = ft_shadow(&win, dir, origin, 100.0);
	ASSERT_TRUE(result < 0);
}

TEST(test_shadow_no_occlusion)
{
	t_window win;
	t_shape sphere;

	ft_window_init(&win);
	memset(&sphere, 0, sizeof(t_shape));
	sphere.id = 's';
	sphere.pt_0 = ft_pt_create(10, 0, 0);
	sphere.diameter = 1.0;
	sphere.next = NULL;
	ft_precompute_shape(&sphere);
	win.beg_sh = &sphere;

	t_pt dir = ft_normal_vect(ft_pt_create(0, 1, 0));
	t_pt origin = ft_pt_create(0, 0, 0);
	double result = ft_shadow(&win, dir, origin, 100.0);
	ASSERT_TRUE(result > 0);
}

/* ========== ft_light tests ========== */

TEST(test_light_ambient_only)
{
	t_window win;
	t_shape sh;

	ft_window_init(&win);
	win.ratio = 0.5;
	win.col = (t_argb){0, 255, 255, 255};
	win.beg_light = NULL;
	memset(&sh, 0, sizeof(t_shape));
	sh.n = ft_pt_create(0, 1, 0);

	t_pt n = ft_pt_create(0, 1, 0);
	t_pt p = ft_pt_create(0, 0, 0);
	t_pt result = ft_light(&win, n, p, &sh, ft_pt_create(0, 0, 1));

	ASSERT_DBL_EQ(0.5 + 0.5 * 255, result.x);
	ASSERT_DBL_EQ(0.5 + 0.5 * 255, result.y);
	ASSERT_DBL_EQ(0.5 + 0.5 * 255, result.z);
}

TEST(test_light_with_source)
{
	t_window win;
	t_shape sh;
	t_light light;

	ft_window_init(&win);
	win.ratio = 0.2;
	win.col = (t_argb){0, 255, 255, 255};
	win.beg_sh = NULL;

	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, 10, 0);
	light.light_ratio = 0.7;
	light.col = (t_argb){0, 255, 255, 255};
	light.next = NULL;
	win.beg_light = &light;

	memset(&sh, 0, sizeof(t_shape));
	sh.n = ft_pt_create(0, 1, 0);

	t_pt n = ft_pt_create(0, 1, 0);
	t_pt p = ft_pt_create(0, 0, 0);
	t_pt result = ft_light(&win, n, p, &sh, ft_pt_create(0, 0, 1));

	ASSERT_TRUE(result.x > 0);
	ASSERT_TRUE(result.y > 0);
	ASSERT_TRUE(result.z > 0);
}

/* ========== ft_pre_light tests ========== */

TEST(test_pre_light)
{
	t_window win;
	t_shape sh;
	t_ray ray;

	ft_window_init(&win);
	win.ratio = 0.3;
	win.col = (t_argb){0, 200, 200, 200};
	win.beg_light = NULL;

	memset(&sh, 0, sizeof(t_shape));
	ray.hit_n = ft_pt_create(0, 1, 0);

	ray.orig = ft_pt_create(0, 0, 0);
	ray.dir = ft_pt_create(0, 0, -1);

	t_pt result = ft_pre_light(&win, &sh, 5.0, &ray);
	ASSERT_TRUE(result.x > 0);
}

/* ========== ft_light with multiple lights ========== */

TEST(test_light_multiple_sources)
{
	t_window win;
	t_shape sh;
	t_light l1, l2;

	ft_window_init(&win);
	win.ratio = 0.1;
	win.col = (t_argb){0, 255, 255, 255};
	win.beg_sh = NULL;

	memset(&l1, 0, sizeof(t_light));
	l1.coord = ft_pt_create(0, 10, 0);
	l1.light_ratio = 0.5;
	l1.col = (t_argb){0, 255, 0, 0};
	l1.next = &l2;

	memset(&l2, 0, sizeof(t_light));
	l2.coord = ft_pt_create(10, 0, 0);
	l2.light_ratio = 0.5;
	l2.col = (t_argb){0, 0, 255, 0};
	l2.next = NULL;

	win.beg_light = &l1;

	memset(&sh, 0, sizeof(t_shape));
	sh.n = ft_pt_create(0, 1, 0);

	t_pt n = ft_pt_create(0, 1, 0);
	t_pt p = ft_pt_create(0, 0, 0);
	t_pt result = ft_light(&win, n, p, &sh, ft_pt_create(0, 0, 1));
	ASSERT_TRUE(result.x > 0);
	ASSERT_TRUE(result.y > 0);
}

TEST(test_light_behind_surface)
{
	t_window win;
	t_shape sh;
	t_light light;

	ft_window_init(&win);
	win.ratio = 0.1;
	win.col = (t_argb){0, 255, 255, 255};
	win.beg_sh = NULL;

	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, -10, 0);
	light.light_ratio = 0.7;
	light.col = (t_argb){0, 255, 255, 255};
	light.next = NULL;
	win.beg_light = &light;

	memset(&sh, 0, sizeof(t_shape));
	sh.n = ft_pt_create(0, 1, 0);

	t_pt n = ft_pt_create(0, 1, 0);
	t_pt p = ft_pt_create(0, 0, 0);
	t_pt result = ft_light(&win, n, p, &sh, ft_pt_create(0, 0, 1));
	double ambient = 0.1 + 0.1 * 255;
	ASSERT_DBL_EQ(ambient, result.x);
}

/* ========== ft_shadow with BVH off ========== */

TEST(test_shadow_shape_behind_light)
{
	t_window win;
	t_shape sphere;

	ft_window_init(&win);
	memset(&sphere, 0, sizeof(t_shape));
	sphere.id = 's';
	sphere.pt_0 = ft_pt_create(0, 50, 0);
	sphere.diameter = 2.0;
	sphere.next = NULL;
	ft_precompute_shape(&sphere);
	win.beg_sh = &sphere;

	t_pt dir = ft_normal_vect(ft_pt_create(0, 1, 0));
	t_pt origin = ft_pt_create(0, 0, 0);
	double result = ft_shadow(&win, dir, origin, 10.0);
	ASSERT_TRUE(result > 0);
}

/* ========== ft_pre_light with light ========== */

TEST(test_pre_light_with_light)
{
	t_window win;
	t_shape sh;
	t_light light;
	t_ray ray;

	ft_window_init(&win);
	win.ratio = 0.2;
	win.col = (t_argb){0, 255, 255, 255};

	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, 10, 0);
	light.light_ratio = 0.8;
	light.col = (t_argb){0, 255, 255, 255};
	light.next = NULL;
	win.beg_light = &light;
	win.beg_sh = NULL;

	memset(&sh, 0, sizeof(t_shape));
	ray.hit_n = ft_pt_create(0, 1, 0);

	ray.orig = ft_pt_create(0, 0, 0);
	ray.dir = ft_pt_create(0, 0, -1);

	t_pt result = ft_pre_light(&win, &sh, 5.0, &ray);
	ASSERT_TRUE(result.x > 0);
	ASSERT_TRUE(result.y > 0);
	ASSERT_TRUE(result.z > 0);
}

/* ========== ft_albedo tests ========== */

TEST(test_albedo_partial_light)
{
	t_pt light = ft_pt_create(127.5, 127.5, 127.5);
	t_argb col = {0, 100, 200, 50};
	t_argb r = ft_albedo(light, col);
	ASSERT_DBL_EQ(50.0, r.r);
	ASSERT_DBL_EQ(100.0, r.g);
	ASSERT_DBL_EQ(25.0, r.b);
}

TEST(test_albedo_zero_light)
{
	t_pt light = ft_pt_create(0, 0, 0);
	t_argb col = {0, 255, 255, 255};
	t_argb r = ft_albedo(light, col);
	ASSERT_DBL_EQ(0.0, r.r);
	ASSERT_DBL_EQ(0.0, r.g);
	ASSERT_DBL_EQ(0.0, r.b);
}

/* ========== ft_trace_ray tests ========== */

TEST(test_trace_ray_no_shapes)
{
	t_window win;
	t_cam cam;

	ft_window_init(&win);
	win.beg_sh = NULL;
	win.beg_light = NULL;

	memset(&cam, 0, sizeof(t_cam));
	cam.rij = ft_pt_create(0, 0, -1);
	cam.pij = ft_pt_create(0, 0, 0);

	t_argb result = ft_trace_ray(&win, &cam);
	ASSERT_DBL_EQ(0.0, result.r);
	ASSERT_DBL_EQ(0.0, result.g);
	ASSERT_DBL_EQ(0.0, result.b);
}

TEST(test_trace_ray_hits_sphere)
{
	t_window win;
	t_cam cam;
	t_shape sphere;
	t_light light;

	ft_window_init(&win);
	win.ratio = 0.2;
	win.col = (t_argb){0, 255, 255, 255};

	memset(&sphere, 0, sizeof(t_shape));
	sphere.id = 's';
	sphere.pt_0 = ft_pt_create(0, 0, -5);
	sphere.diameter = 2.0;
	sphere.color = (t_argb){0, 255, 0, 0};
	sphere.next = NULL;
	ft_precompute_shape(&sphere);
	win.beg_sh = &sphere;

	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, 10, 0);
	light.light_ratio = 0.7;
	light.col = (t_argb){0, 255, 255, 255};
	light.next = NULL;
	win.beg_light = &light;

	memset(&cam, 0, sizeof(t_cam));
	cam.rij = ft_pt_create(0, 0, -1);
	cam.pij = ft_pt_create(0, 0, 0);

	t_argb result = ft_trace_ray(&win, &cam);
	ASSERT_TRUE(result.r > 0);
}

void	run_lighting_tests(void);

#ifndef TEST_ALL
int main(void)
{
	run_lighting_tests();
	TEST_REPORT();
}
#endif

void	run_lighting_tests(void)
{
	TEST_SUITE("ft_lstsize_light");
	RUN_TEST(test_lstsize_light_empty);
	RUN_TEST(test_lstsize_light_one);
	RUN_TEST(test_lstsize_light_multiple);

	TEST_SUITE("ft_shadow");
	RUN_TEST(test_shadow_no_shapes);
	RUN_TEST(test_shadow_with_blocking_shape);
	RUN_TEST(test_shadow_no_occlusion);

	TEST_SUITE("ft_light");
	RUN_TEST(test_light_ambient_only);
	RUN_TEST(test_light_with_source);
	RUN_TEST(test_light_multiple_sources);
	RUN_TEST(test_light_behind_surface);

	TEST_SUITE("ft_shadow additional");
	RUN_TEST(test_shadow_shape_behind_light);

	TEST_SUITE("ft_pre_light");
	RUN_TEST(test_pre_light);
	RUN_TEST(test_pre_light_with_light);

	TEST_SUITE("ft_albedo");
	RUN_TEST(test_albedo_partial_light);
	RUN_TEST(test_albedo_zero_light);

	TEST_SUITE("ft_trace_ray");
	RUN_TEST(test_trace_ray_no_shapes);
	RUN_TEST(test_trace_ray_hits_sphere);
}
