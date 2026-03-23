#include "mini_rt.h"
#include "minirt_test.h"

static t_shape	make_sphere(t_pt center, double diameter)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 's';
	sh.pt_0 = center;
	sh.diameter = diameter;
	return (sh);
}

static t_shape	make_plane(t_pt point, t_pt ori)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'p';
	sh.pt_0 = point;
	sh.ori = ori;
	return (sh);
}

static t_shape	make_triangle(t_pt p0, t_pt p1, t_pt p2)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 't';
	sh.pt_0 = p0;
	sh.pt_1 = p1;
	sh.pt_2 = p2;
	return (sh);
}

static t_shape	make_square(t_pt center, t_pt ori, double side)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'q';
	sh.pt_0 = center;
	sh.ori = ori;
	sh.height = side;
	return (sh);
}

static t_shape	make_cylinder(t_pt center, t_pt axis, double diam, double h)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'y';
	sh.pt_0 = center;
	sh.ori = axis;
	sh.diameter = diam;
	sh.height = h;
	return (sh);
}

static t_ray	make_ray(t_pt orig, t_pt dir)
{
	t_ray	ray;

	memset(&ray, 0, sizeof(t_ray));
	ray.orig = orig;
	ray.dir = ft_normal_vect(dir);
	ray.lenght = -1;
	return (ray);
}

/* ========== Sphere intersection tests ========== */

TEST(test_sphere_hit_front)
{
	t_shape sh = make_sphere(ft_pt_create(0, 0, -5), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_sphere(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
}

TEST(test_sphere_miss)
{
	t_shape sh = make_sphere(ft_pt_create(0, 0, -5), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 10, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_sphere(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_sphere_behind_ray)
{
	t_shape sh = make_sphere(ft_pt_create(0, 0, 5), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_sphere(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_sphere_normal_at_hit)
{
	t_shape sh = make_sphere(ft_pt_create(0, 0, -5), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_sphere(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(0.0, sh.n.x);
	ASSERT_DBL_EQ(0.0, sh.n.y);
	ASSERT_TRUE(sh.n.z > 0);
}

TEST(test_sphere_ray_along_x)
{
	t_shape sh = make_sphere(ft_pt_create(5, 0, 0), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(1, 0, 0));
	ft_intersect_ray_sphere(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
}

/* ========== Plane intersection tests ========== */

TEST(test_plane_hit)
{
	t_shape sh = make_plane(ft_pt_create(0, -1, 0), ft_pt_create(0, 1, 0));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, -1, 0));
	ft_intersect_ray_plan(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(1.0, ray.lenght);
}

TEST(test_plane_parallel_miss)
{
	t_shape sh = make_plane(ft_pt_create(0, -1, 0), ft_pt_create(0, 1, 0));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(1, 0, 0));
	ft_intersect_ray_plan(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_plane_behind_ray)
{
	t_shape sh = make_plane(ft_pt_create(0, 1, 0), ft_pt_create(0, 1, 0));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, -1, 0));
	ft_intersect_ray_plan(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_plane_at_angle)
{
	t_shape sh = make_plane(ft_pt_create(0, 0, -10), ft_pt_create(0, 0, 1));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_plan(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(10.0, ray.lenght);
}

/* ========== Triangle intersection tests ========== */

TEST(test_triangle_hit_center)
{
	t_shape sh = make_triangle(
		ft_pt_create(-1, -1, -5),
		ft_pt_create(1, -1, -5),
		ft_pt_create(0, 1, -5));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_triangle(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

TEST(test_triangle_miss_outside)
{
	t_shape sh = make_triangle(
		ft_pt_create(-1, -1, -5),
		ft_pt_create(1, -1, -5),
		ft_pt_create(0, 1, -5));
	t_ray ray = make_ray(ft_pt_create(5, 5, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_triangle(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_triangle_behind_ray)
{
	t_shape sh = make_triangle(
		ft_pt_create(-1, -1, 5),
		ft_pt_create(1, -1, 5),
		ft_pt_create(0, 1, 5));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_triangle(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Square intersection tests ========== */

TEST(test_square_hit_center)
{
	t_shape sh = make_square(
		ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_square(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

TEST(test_square_miss_outside)
{
	t_shape sh = make_square(
		ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 2.0);
	t_ray ray = make_ray(ft_pt_create(10, 10, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_square(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_square_behind_ray)
{
	t_shape sh = make_square(
		ft_pt_create(0, 0, 5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_square(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Cylinder intersection tests ========== */

TEST(test_cylinder_hit_side)
{
	t_shape sh = make_cylinder(
		ft_pt_create(0, -2, -5), ft_pt_create(0, 1, 0), 2.0, 4.0);
	t_ray ray = make_ray(ft_pt_create(5, 0, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_cylinder(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

TEST(test_cylinder_miss)
{
	t_shape sh = make_cylinder(
		ft_pt_create(0, -2, -5), ft_pt_create(0, 1, 0), 2.0, 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_cylinder(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== ft_which_shape dispatch tests ========== */

TEST(test_which_shape_sphere)
{
	t_shape sh = make_sphere(ft_pt_create(0, 0, -5), 2.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
}

TEST(test_which_shape_plane)
{
	t_shape sh = make_plane(ft_pt_create(0, -2, 0), ft_pt_create(0, 1, 0));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, -1, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(2.0, ray.lenght);
}

TEST(test_which_shape_triangle)
{
	t_shape sh = make_triangle(
		ft_pt_create(-2, -2, -5),
		ft_pt_create(2, -2, -5),
		ft_pt_create(0, 2, -5));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

/* ========== ft_no_ray / ft_shoot_ray tests ========== */

TEST(test_no_ray)
{
	t_ray ray = ft_no_ray();
	ASSERT_DBL_EQ(-1.0, ray.lenght);
}

TEST(test_shoot_ray)
{
	t_pt orig = ft_pt_create(0, 0, 0);
	t_pt dir = ft_pt_create(0, 0, -1);
	t_ray ray = ft_shoot_ray(orig, dir, 5.0);
	ASSERT_DBL_EQ(0.0, ray.orig.x);
	ASSERT_DBL_EQ(0.0, ray.orig.y);
	ASSERT_DBL_EQ(0.0, ray.orig.z);
	ASSERT_DBL_EQ(0.0, ray.dir.x);
	ASSERT_DBL_EQ(0.0, ray.dir.y);
	ASSERT_DBL_EQ(-1.0, ray.dir.z);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

int	main(void)
{
	TEST_SUITE("Sphere intersections");
	RUN_TEST(test_sphere_hit_front);
	RUN_TEST(test_sphere_miss);
	RUN_TEST(test_sphere_behind_ray);
	RUN_TEST(test_sphere_normal_at_hit);
	RUN_TEST(test_sphere_ray_along_x);

	TEST_SUITE("Plane intersections");
	RUN_TEST(test_plane_hit);
	RUN_TEST(test_plane_parallel_miss);
	RUN_TEST(test_plane_behind_ray);
	RUN_TEST(test_plane_at_angle);

	TEST_SUITE("Triangle intersections");
	RUN_TEST(test_triangle_hit_center);
	RUN_TEST(test_triangle_miss_outside);
	RUN_TEST(test_triangle_behind_ray);

	TEST_SUITE("Square intersections");
	RUN_TEST(test_square_hit_center);
	RUN_TEST(test_square_miss_outside);
	RUN_TEST(test_square_behind_ray);

	TEST_SUITE("Cylinder intersections");
	RUN_TEST(test_cylinder_hit_side);
	RUN_TEST(test_cylinder_miss);

	TEST_SUITE("Shape dispatch & ray utilities");
	RUN_TEST(test_which_shape_sphere);
	RUN_TEST(test_which_shape_plane);
	RUN_TEST(test_which_shape_triangle);
	RUN_TEST(test_no_ray);
	RUN_TEST(test_shoot_ray);

	TEST_REPORT();
}
