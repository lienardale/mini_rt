#include "mini_rt.h"
#include "minirt_test.h"

static t_shape	make_sphere(t_pt center, double diameter)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 's';
	sh.pt_0 = center;
	sh.diameter = diameter;
	ft_precompute_shape(&sh);
	return (sh);
}

static t_shape	make_plane(t_pt point, t_pt ori)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'p';
	sh.pt_0 = point;
	sh.ori = ori;
	ft_precompute_shape(&sh);
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
	ft_precompute_shape(&sh);
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
	ft_precompute_shape(&sh);
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
	ft_precompute_shape(&sh);
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
	ASSERT_DBL_EQ(0.0, ray.hit_n.x);
	ASSERT_DBL_EQ(0.0, ray.hit_n.y);
	ASSERT_TRUE(ray.hit_n.z > 0);
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

/* ========== Cone intersection tests ========== */

static t_shape	make_cone(t_pt apex, t_pt axis, double diam, double h)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'o';
	sh.pt_0 = apex;
	sh.ori = axis;
	sh.diameter = diam;
	sh.height = h;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_cone_hit)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(2, 1, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_cone(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

TEST(test_cone_miss)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(0, 10, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_cone(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Disk intersection tests ========== */

static t_shape	make_disk(t_pt center, t_pt ori, double diam)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'k';
	sh.pt_0 = center;
	sh.ori = ori;
	sh.diameter = diam;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_disk_hit)
{
	t_shape sh = make_disk(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_disk(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

TEST(test_disk_miss_outside)
{
	t_shape sh = make_disk(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 2.0);
	t_ray ray = make_ray(ft_pt_create(10, 10, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_disk(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_disk_parallel_miss)
{
	t_shape sh = make_disk(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(1, 0, 0));
	ft_intersect_ray_disk(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Ellipsoid intersection tests ========== */

static t_shape	make_ellipsoid(t_pt center, t_pt radii)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'e';
	sh.pt_0 = center;
	sh.pt_1 = radii;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_ellipsoid_hit)
{
	t_shape sh = make_ellipsoid(ft_pt_create(0, 0, -5),
								ft_pt_create(2, 1, 1));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_ellipsoid(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
}

TEST(test_ellipsoid_miss)
{
	t_shape sh = make_ellipsoid(ft_pt_create(0, 0, -5),
								ft_pt_create(1, 1, 1));
	t_ray ray = make_ray(ft_pt_create(10, 10, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_ellipsoid(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Box intersection tests ========== */

static t_shape	make_box(t_pt center, t_pt dims)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'b';
	sh.pt_0 = center;
	sh.pt_1 = dims;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_box_hit)
{
	t_shape sh = make_box(ft_pt_create(0, 0, -5), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
}

TEST(test_box_miss)
{
	t_shape sh = make_box(ft_pt_create(0, 0, -5), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(10, 10, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_box_normal)
{
	t_shape sh = make_box(ft_pt_create(0, 0, -5), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(0.0, ray.hit_n.x);
	ASSERT_DBL_EQ(0.0, ray.hit_n.y);
	ASSERT_TRUE(ray.hit_n.z > 0);
}

/* ========== Torus intersection tests ========== */

static t_shape	make_torus(t_pt center, t_pt axis, double big_r, double small_r)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'u';
	sh.pt_0 = center;
	sh.ori = axis;
	sh.diameter = big_r;
	sh.height = small_r;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_torus_hit)
{
	t_shape sh = make_torus(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
							2.0, 0.5);
	t_ray ray = make_ray(ft_pt_create(2, 0, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_torus(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

TEST(test_torus_miss)
{
	t_shape sh = make_torus(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
							2.0, 0.5);
	t_ray ray = make_ray(ft_pt_create(0, 10, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_torus(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

/* ========== Hyperboloid intersection tests ========== */

static t_shape	make_hyperboloid(t_pt center, t_pt axis, t_pt radii, double h)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'h';
	sh.pt_0 = center;
	sh.ori = axis;
	sh.pt_1 = radii;
	sh.height = h;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_hyperboloid_hit)
{
	t_shape sh = make_hyperboloid(ft_pt_create(0, 0, -5),
								  ft_pt_create(0, 1, 0),
								  ft_pt_create(1, 1, 0), 4.0);
	t_ray ray = make_ray(ft_pt_create(3, 0, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_hyperboloid(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Paraboloid intersection tests ========== */

static t_shape	make_paraboloid(t_pt center, t_pt axis, double focal, double h)
{
	t_shape	sh;

	memset(&sh, 0, sizeof(t_shape));
	sh.id = 'a';
	sh.pt_0 = center;
	sh.ori = axis;
	sh.diameter = focal;
	sh.height = h;
	ft_precompute_shape(&sh);
	return (sh);
}

TEST(test_paraboloid_hit)
{
	t_shape sh = make_paraboloid(ft_pt_create(0, 0, -5),
								 ft_pt_create(0, 1, 0), 1.0, 10.0);
	t_ray ray = make_ray(ft_pt_create(3, 2, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_paraboloid(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Additional cone tests ========== */

TEST(test_cone_normal_direction)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(2, 1, -5), ft_pt_create(-1, 0, 0));
	ft_intersect_ray_cone(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_TRUE(ft_lenght(ray.hit_n) > 0.9);
	ASSERT_TRUE(ft_lenght(ray.hit_n) < 1.1);
}

TEST(test_cone_behind_ray)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, 5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_cone(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_cone_dispatch)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(2, 1, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Additional disk tests ========== */

TEST(test_disk_normal)
{
	t_shape sh = make_disk(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_disk(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_TRUE(fabs(ray.hit_n.z) > 0.9);
}

/* ========== Additional ellipsoid tests ========== */

TEST(test_ellipsoid_normal)
{
	t_shape sh = make_ellipsoid(ft_pt_create(0, 0, -5),
								ft_pt_create(2, 1, 1));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_ellipsoid(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_TRUE(ft_lenght(ray.hit_n) > 0.9);
}

TEST(test_ellipsoid_behind_ray)
{
	t_shape sh = make_ellipsoid(ft_pt_create(0, 0, 5),
								ft_pt_create(1, 1, 1));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_ellipsoid(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_ellipsoid_dispatch)
{
	t_shape sh = make_ellipsoid(ft_pt_create(0, 0, -5),
								ft_pt_create(2, 1, 1));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Additional box tests ========== */

TEST(test_box_behind_ray)
{
	t_shape sh = make_box(ft_pt_create(0, 0, 5), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_box_hit_side_x)
{
	t_shape sh = make_box(ft_pt_create(5, 0, 0), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(1, 0, 0));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_TRUE(fabs(ray.hit_n.x) > 0.9);
}

TEST(test_box_hit_side_y)
{
	t_shape sh = make_box(ft_pt_create(0, 5, 0), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_box(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_TRUE(fabs(ray.hit_n.y) > 0.9);
}

/* ========== Additional torus tests ========== */

TEST(test_torus_through_hole)
{
	t_shape sh = make_torus(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1),
							2.0, 0.3);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_intersect_ray_torus(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_torus_dispatch)
{
	t_shape sh = make_torus(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
							2.0, 0.5);
	t_ray ray = make_ray(ft_pt_create(2, 0, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Additional hyperboloid tests ========== */

TEST(test_hyperboloid_miss)
{
	t_shape sh = make_hyperboloid(ft_pt_create(0, 0, -5),
								  ft_pt_create(0, 1, 0),
								  ft_pt_create(1, 1, 0), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 20, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_hyperboloid(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_hyperboloid_dispatch)
{
	t_shape sh = make_hyperboloid(ft_pt_create(0, 0, -5),
								  ft_pt_create(0, 1, 0),
								  ft_pt_create(1, 1, 0), 4.0);
	t_ray ray = make_ray(ft_pt_create(3, 0, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Additional paraboloid tests ========== */

TEST(test_paraboloid_miss)
{
	t_shape sh = make_paraboloid(ft_pt_create(0, 0, -5),
								 ft_pt_create(0, 1, 0), 1.0, 10.0);
	t_ray ray = make_ray(ft_pt_create(0, 20, 0), ft_pt_create(0, 1, 0));
	ft_intersect_ray_paraboloid(&sh, &ray);
	ASSERT_TRUE(ray.lenght < 0);
}

TEST(test_paraboloid_dispatch)
{
	t_shape sh = make_paraboloid(ft_pt_create(0, 0, -5),
								 ft_pt_create(0, 1, 0), 1.0, 10.0);
	t_ray ray = make_ray(ft_pt_create(3, 2, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== ft_trace_shapes tests ========== */

TEST(test_trace_shapes_finds_closest)
{
	t_shape s1 = make_sphere(ft_pt_create(0, 0, -5), 2.0);
	t_shape s2 = make_sphere(ft_pt_create(0, 0, -10), 2.0);
	s1.next = &s2;
	s2.next = NULL;

	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	double min = INFINITY;
	t_shape *min_sh = NULL;
	ft_trace_shapes(&s1, &ray, &min, &min_sh);
	ASSERT_TRUE(min_sh == &s1);
	ASSERT_DBL_EQ(4.0, min);
}

TEST(test_trace_shapes_empty)
{
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	double min = INFINITY;
	t_shape *min_sh = NULL;
	ft_trace_shapes(NULL, &ray, &min, &min_sh);
	ASSERT_TRUE(min_sh == NULL);
}

/* ========== ft_precompute_shapes tests ========== */

TEST(test_precompute_shapes_list)
{
	t_shape s1;
	t_shape s2;
	t_window win;

	memset(&s1, 0, sizeof(t_shape));
	memset(&s2, 0, sizeof(t_shape));
	ft_window_init(&win);
	s1.id = 's';
	s1.pt_0 = ft_pt_create(0, 0, 0);
	s1.diameter = 4.0;
	s1.next = &s2;
	s2.id = 'p';
	s2.pt_0 = ft_pt_create(0, -1, 0);
	s2.ori = ft_pt_create(0, 1, 0);
	s2.next = NULL;
	win.beg_sh = &s1;
	ft_precompute_shapes(&win);
	ASSERT_DBL_EQ(4.0, s1.radius_sq);
	ASSERT_TRUE(fabs(s2.plane_d) > 0);
}

/* ========== Cylinder additional tests ========== */

TEST(test_cylinder_along_axis)
{
	t_shape sh = make_cylinder(
		ft_pt_create(0, -2, -5), ft_pt_create(0, 1, 0), 2.0, 4.0);
	t_ray ray = make_ray(ft_pt_create(0, -10, -5), ft_pt_create(0, 1, 0));
	ft_intersect_ray_cylinder(&sh, &ray);
	ASSERT_TRUE(ray.lenght != 0);
}

TEST(test_cylinder_dispatch)
{
	t_shape sh = make_cylinder(
		ft_pt_create(0, -2, -5), ft_pt_create(0, 1, 0), 2.0, 4.0);
	t_ray ray = make_ray(ft_pt_create(5, 0, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

/* ========== Shape dispatch for new shapes ========== */

TEST(test_which_shape_cone)
{
	t_shape sh = make_cone(ft_pt_create(0, 0, -5), ft_pt_create(0, 1, 0),
						   2.0, 3.0);
	t_ray ray = make_ray(ft_pt_create(2, 1, -5), ft_pt_create(-1, 0, 0));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
}

TEST(test_which_shape_disk)
{
	t_shape sh = make_disk(ft_pt_create(0, 0, -5), ft_pt_create(0, 0, 1), 4.0);
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(5.0, ray.lenght);
}

TEST(test_which_shape_box)
{
	t_shape sh = make_box(ft_pt_create(0, 0, -5), ft_pt_create(2, 2, 2));
	t_ray ray = make_ray(ft_pt_create(0, 0, 0), ft_pt_create(0, 0, -1));
	ft_which_shape(&sh, &ray);
	ASSERT_TRUE(ray.lenght > 0);
	ASSERT_DBL_EQ(4.0, ray.lenght);
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

void	run_intersection_tests(void);

#ifndef TEST_ALL
int	main(void)
{
	run_intersection_tests();
	TEST_REPORT();
}
#endif

void	run_intersection_tests(void)
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

	TEST_SUITE("Cone intersections");
	RUN_TEST(test_cone_hit);
	RUN_TEST(test_cone_miss);
	RUN_TEST(test_cone_normal_direction);
	RUN_TEST(test_cone_behind_ray);
	RUN_TEST(test_cone_dispatch);

	TEST_SUITE("Disk intersections");
	RUN_TEST(test_disk_hit);
	RUN_TEST(test_disk_miss_outside);
	RUN_TEST(test_disk_parallel_miss);
	RUN_TEST(test_disk_normal);

	TEST_SUITE("Ellipsoid intersections");
	RUN_TEST(test_ellipsoid_hit);
	RUN_TEST(test_ellipsoid_miss);
	RUN_TEST(test_ellipsoid_normal);
	RUN_TEST(test_ellipsoid_behind_ray);
	RUN_TEST(test_ellipsoid_dispatch);

	TEST_SUITE("Box intersections");
	RUN_TEST(test_box_hit);
	RUN_TEST(test_box_miss);
	RUN_TEST(test_box_normal);
	RUN_TEST(test_box_behind_ray);
	RUN_TEST(test_box_hit_side_x);
	RUN_TEST(test_box_hit_side_y);

	TEST_SUITE("Torus intersections");
	RUN_TEST(test_torus_hit);
	RUN_TEST(test_torus_miss);
	RUN_TEST(test_torus_through_hole);
	RUN_TEST(test_torus_dispatch);

	TEST_SUITE("Hyperboloid intersections");
	RUN_TEST(test_hyperboloid_hit);
	RUN_TEST(test_hyperboloid_miss);
	RUN_TEST(test_hyperboloid_dispatch);

	TEST_SUITE("Paraboloid intersections");
	RUN_TEST(test_paraboloid_hit);
	RUN_TEST(test_paraboloid_miss);
	RUN_TEST(test_paraboloid_dispatch);

	TEST_SUITE("Shape dispatch & ray utilities");
	RUN_TEST(test_which_shape_sphere);
	RUN_TEST(test_which_shape_plane);
	RUN_TEST(test_which_shape_triangle);
	RUN_TEST(test_which_shape_cone);
	RUN_TEST(test_which_shape_disk);
	RUN_TEST(test_which_shape_box);
	RUN_TEST(test_no_ray);
	RUN_TEST(test_shoot_ray);

	TEST_SUITE("ft_trace_shapes");
	RUN_TEST(test_trace_shapes_finds_closest);
	RUN_TEST(test_trace_shapes_empty);

	TEST_SUITE("ft_precompute_shapes");
	RUN_TEST(test_precompute_shapes_list);

	TEST_SUITE("Cylinder additional");
	RUN_TEST(test_cylinder_along_axis);
	RUN_TEST(test_cylinder_dispatch);
}
