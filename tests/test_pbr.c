#include "mini_rt.h"
#include "minirt_test.h"

/* ========== ft_fresnel_schlick tests ========== */

TEST(test_fresnel_head_on)
{
	ASSERT_DBL_EQ(0.04, ft_fresnel_schlick(1.0, 0.04));
}

TEST(test_fresnel_grazing)
{
	ASSERT_DBL_EQ(1.0, ft_fresnel_schlick(0.0, 0.04));
}

TEST(test_fresnel_45deg)
{
	double cos_t = cos(M_PI / 4.0);
	double x = 1.0 - cos_t;
	double expected = 0.04 + 0.96 * x * x * x * x * x;

	ASSERT_DBL_EQ(expected, ft_fresnel_schlick(cos_t, 0.04));
}

TEST(test_fresnel_negative_clamp)
{
	ASSERT_DBL_EQ(1.0, ft_fresnel_schlick(-0.5, 0.04));
}

TEST(test_fresnel_metal)
{
	ASSERT_DBL_EQ(1.0, ft_fresnel_schlick(1.0, 1.0));
}

/* ========== ft_env_sky tests ========== */

TEST(test_env_sky_up)
{
	t_argb r = ft_env_sky((t_pt){0, 1, 0});

	ASSERT_DBL_EQ(128.0, r.r);
	ASSERT_DBL_EQ(178.0, r.g);
	ASSERT_DBL_EQ(255.0, r.b);
}

TEST(test_env_sky_down)
{
	t_argb r = ft_env_sky((t_pt){0, -1, 0});

	ASSERT_DBL_EQ(255.0, r.r);
	ASSERT_DBL_EQ(255.0, r.g);
	ASSERT_DBL_EQ(255.0, r.b);
}

TEST(test_env_sky_horizon)
{
	t_argb r = ft_env_sky((t_pt){1, 0, 0});

	ASSERT_DBL_EQ(191.5, r.r);
	ASSERT_DBL_EQ(216.5, r.g);
	ASSERT_DBL_EQ(255.0, r.b);
}

TEST(test_env_sky_below_clamp)
{
	t_argb r = ft_env_sky((t_pt){0, -2, 0});

	ASSERT_DBL_EQ(255.0, r.r);
	ASSERT_DBL_EQ(255.0, r.g);
	ASSERT_DBL_EQ(255.0, r.b);
}

/* ========== ft_material_default tests ========== */

TEST(test_material_default_values)
{
	t_material mat;

	memset(&mat, 0xFF, sizeof(t_material));
	ft_material_default(&mat);
	ASSERT_DBL_EQ(0.0, mat.reflectivity);
	ASSERT_DBL_EQ(0.0, mat.transparency);
	ASSERT_DBL_EQ(1.0, mat.refr_index);
	ASSERT_DBL_EQ(0.0, mat.specular);
	ASSERT_DBL_EQ(32.0, mat.shininess);
	ASSERT_TRUE(mat.texture == NULL);
	ASSERT_TRUE(mat.bump_map == NULL);
	ASSERT_DBL_EQ(1.0, mat.tex_scale_u);
	ASSERT_DBL_EQ(1.0, mat.tex_scale_v);
	ASSERT_DBL_EQ(1.0, mat.bump_strength);
	ASSERT_DBL_EQ(0.0, mat.metallic);
	ASSERT_DBL_EQ(0.0, mat.roughness);
	ASSERT_ARGB_EQ(0.0, 0.0, 0.0, mat.emission);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, mat.vel);
}

/* ========== ft_reflect_ray tests ========== */

TEST(test_reflect_straight_on)
{
	t_pt dir = {0, -1, 0};
	t_pt normal = {0, 1, 0};
	t_pt r = ft_reflect_ray(dir, normal);

	ASSERT_PT_EQ(0, 1, 0, r);
}

TEST(test_reflect_45deg)
{
	t_pt dir = ft_normal_vect((t_pt){1, -1, 0});
	t_pt normal = {0, 1, 0};
	t_pt r = ft_reflect_ray(dir, normal);
	double s = 1.0 / sqrt(2.0);

	ASSERT_PT_EQ(s, s, 0, r);
}

/* ========== ft_refract_ray tests ========== */

TEST(test_refract_normal_incidence)
{
	t_pt dir = {0, 0, -1};
	t_pt normal = {0, 0, 1};
	t_pt r = ft_refract_ray(dir, normal, 1.0);

	ASSERT_PT_EQ(0, 0, -1, r);
}

TEST(test_refract_total_internal_reflection)
{
	t_pt dir = ft_normal_vect((t_pt){0.866025, 0, -0.5});
	t_pt normal = {0, 0, 1};
	t_pt r = ft_refract_ray(dir, normal, 2.0);
	t_pt expected = ft_reflect_ray(dir, normal);

	ASSERT_PT_EQ(expected.x, expected.y, expected.z, r);
}

/* ========== ft_parse_material tests ========== */

TEST(test_parse_material_metal)
{
	t_window win;
	t_material mat;
	char line[] = "metal:0.8";
	char *p = line;

	ft_window_init(&win);
	ft_material_default(&mat);
	ft_parse_material(&win, &mat, &p);
	ASSERT_DBL_EQ(0.8, mat.metallic);
}

TEST(test_parse_material_rough)
{
	t_window win;
	t_material mat;
	char line[] = "rough:0.5";
	char *p = line;

	ft_window_init(&win);
	ft_material_default(&mat);
	ft_parse_material(&win, &mat, &p);
	ASSERT_DBL_EQ(0.5, mat.roughness);
}

TEST(test_parse_material_emit)
{
	t_window win;
	t_material mat;
	char line[] = "emit:10,20,30";
	char *p = line;

	ft_window_init(&win);
	ft_material_default(&mat);
	ft_parse_material(&win, &mat, &p);
	ASSERT_DBL_EQ(10.0, mat.emission.r);
	ASSERT_DBL_EQ(20.0, mat.emission.g);
	ASSERT_DBL_EQ(30.0, mat.emission.b);
}

TEST(test_parse_material_combined)
{
	t_window win;
	t_material mat;
	char line[] = "metal:0.9 rough:0.3 emit:5,10,15";
	char *p = line;

	ft_window_init(&win);
	ft_material_default(&mat);
	ft_parse_material(&win, &mat, &p);
	ASSERT_DBL_EQ(0.9, mat.metallic);
	ASSERT_DBL_EQ(0.3, mat.roughness);
	ASSERT_DBL_EQ(5.0, mat.emission.r);
	ASSERT_DBL_EQ(10.0, mat.emission.g);
	ASSERT_DBL_EQ(15.0, mat.emission.b);
}

TEST(test_parse_material_vel)
{
	t_window win;
	t_material mat;
	char line[] = "vel:1.5,2.0,-0.5";
	char *p = line;

	ft_window_init(&win);
	ft_material_default(&mat);
	ft_parse_material(&win, &mat, &p);
	ASSERT_DBL_EQ(1.5, mat.vel.x);
	ASSERT_DBL_EQ(2.0, mat.vel.y);
	ASSERT_DBL_EQ(-0.5, mat.vel.z);
}

/* ========== ft_pbr_shade tests ========== */

TEST(test_pbr_shade_head_on)
{
	t_material mat;
	t_argb result;

	ft_material_default(&mat);
	mat.metallic = 0.0;
	mat.roughness = 0.5;
	result = ft_pbr_shade((t_pt){0, 0, 1}, (t_pt){0, 0, 1}, (t_pt){0, 0, 1},
						  &mat, (t_argb){0, 255, 255, 255},
						  (t_argb){0, 255, 255, 255});
	ASSERT_TRUE(result.r > 0);
	ASSERT_TRUE(result.g > 0);
	ASSERT_TRUE(result.b > 0);
	ASSERT_DBL_EQ(result.r, result.g);
	ASSERT_DBL_EQ(result.g, result.b);
}

TEST(test_pbr_shade_backface)
{
	t_material mat;
	t_argb result;

	ft_material_default(&mat);
	mat.metallic = 0.0;
	mat.roughness = 0.5;
	result = ft_pbr_shade((t_pt){0, 0, 1}, (t_pt){0, 0, -1}, (t_pt){0, 0, 1},
						  &mat, (t_argb){0, 255, 255, 255},
						  (t_argb){0, 255, 255, 255});
	ASSERT_ARGB_EQ(0, 0, 0, result);
}

TEST(test_pbr_shade_metal)
{
	t_material mat;
	t_argb result;

	ft_material_default(&mat);
	mat.metallic = 1.0;
	mat.roughness = 0.3;
	result = ft_pbr_shade((t_pt){0, 0, 1}, (t_pt){0, 0, 1}, (t_pt){0, 0, 1},
						  &mat, (t_argb){0, 255, 255, 255},
						  (t_argb){0, 255, 255, 255});
	ASSERT_TRUE(result.r > 0);
}

/* ========== ft_light PBR branch test ========== */

TEST(test_light_pbr_branch)
{
	t_window win;
	t_shape sh;
	t_light light;

	ft_window_init(&win);
	win.ratio = 0.1;
	win.col = (t_argb){0, 255, 255, 255};
	win.beg_sh = NULL;

	memset(&light, 0, sizeof(t_light));
	light.coord = ft_pt_create(0, 10, 0);
	light.light_ratio = 0.8;
	light.col = (t_argb){0, 255, 255, 255};
	light.next = NULL;
	win.beg_light = &light;

	memset(&sh, 0, sizeof(t_shape));
	ft_material_default(&sh.mat);
	sh.mat.metallic = 0.5;
	sh.mat.roughness = 0.4;
	sh.color = (t_argb){0, 200, 200, 200};

	t_pt n = ft_pt_create(0, 1, 0);
	t_pt p = ft_pt_create(0, 0, 0);
	t_pt result = ft_light(&win, n, p, &sh, ft_pt_create(0, 0, 1));

	ASSERT_TRUE(result.x > win.ratio);
}

/* ========== ft_trace_ray_recursive env sky tests ========== */

TEST(test_trace_ray_env_sky_miss)
{
	t_window win;
	t_ray ray;

	ft_window_init(&win);
	win.use_env_sky = 1;
	win.beg_sh = NULL;
	win.beg_light = NULL;

	memset(&ray, 0, sizeof(t_ray));
	ray.dir = ft_pt_create(0, 1, 0);
	ray.orig = ft_pt_create(0, 0, 0);
	ray.lenght = -1;

	t_argb result = ft_trace_ray_recursive(&win, &ray, 0);
	t_argb expected = ft_env_sky((t_pt){0, 1, 0});

	ASSERT_ARGB_EQ(expected.r, expected.g, expected.b, result);
}

TEST(test_trace_ray_no_env_sky_miss)
{
	t_window win;
	t_ray ray;

	ft_window_init(&win);
	win.use_env_sky = 0;
	win.beg_sh = NULL;
	win.beg_light = NULL;

	memset(&ray, 0, sizeof(t_ray));
	ray.dir = ft_pt_create(0, 1, 0);
	ray.orig = ft_pt_create(0, 0, 0);
	ray.lenght = -1;

	t_argb result = ft_trace_ray_recursive(&win, &ray, 0);

	ASSERT_DBL_EQ(0.0, result.r);
	ASSERT_DBL_EQ(0.0, result.g);
	ASSERT_DBL_EQ(0.0, result.b);
}

/* ========== Runner ========== */

void	run_pbr_tests(void);

#ifndef TEST_ALL
int	main(void)
{
	run_pbr_tests();
	TEST_REPORT();
}
#endif

void	run_pbr_tests(void)
{
	TEST_SUITE("ft_fresnel_schlick");
	RUN_TEST(test_fresnel_head_on);
	RUN_TEST(test_fresnel_grazing);
	RUN_TEST(test_fresnel_45deg);
	RUN_TEST(test_fresnel_negative_clamp);
	RUN_TEST(test_fresnel_metal);

	TEST_SUITE("ft_env_sky");
	RUN_TEST(test_env_sky_up);
	RUN_TEST(test_env_sky_down);
	RUN_TEST(test_env_sky_horizon);
	RUN_TEST(test_env_sky_below_clamp);

	TEST_SUITE("ft_material_default");
	RUN_TEST(test_material_default_values);

	TEST_SUITE("ft_reflect_ray / ft_refract_ray");
	RUN_TEST(test_reflect_straight_on);
	RUN_TEST(test_reflect_45deg);
	RUN_TEST(test_refract_normal_incidence);
	RUN_TEST(test_refract_total_internal_reflection);

	TEST_SUITE("ft_parse_material (PBR)");
	RUN_TEST(test_parse_material_metal);
	RUN_TEST(test_parse_material_rough);
	RUN_TEST(test_parse_material_emit);
	RUN_TEST(test_parse_material_combined);
	RUN_TEST(test_parse_material_vel);

	TEST_SUITE("ft_pbr_shade");
	RUN_TEST(test_pbr_shade_head_on);
	RUN_TEST(test_pbr_shade_backface);
	RUN_TEST(test_pbr_shade_metal);

	TEST_SUITE("ft_light PBR branch");
	RUN_TEST(test_light_pbr_branch);

	TEST_SUITE("ft_trace_ray env sky");
	RUN_TEST(test_trace_ray_env_sky_miss);
	RUN_TEST(test_trace_ray_no_env_sky_miss);
}
