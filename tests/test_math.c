#include "mini_rt.h"
#include "minirt_test.h"

/* ========== ft_pt.c tests ========== */

TEST(test_ft_sqr)
{
	ASSERT_DBL_EQ(0.0, ft_sqr(0.0));
	ASSERT_DBL_EQ(1.0, ft_sqr(1.0));
	ASSERT_DBL_EQ(1.0, ft_sqr(-1.0));
	ASSERT_DBL_EQ(4.0, ft_sqr(2.0));
	ASSERT_DBL_EQ(6.25, ft_sqr(2.5));
	ASSERT_DBL_EQ(0.25, ft_sqr(0.5));
}

TEST(test_ft_pt_create)
{
	t_pt p = ft_pt_create(1.0, 2.0, 3.0);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, p);

	t_pt zero = ft_pt_create(0.0, 0.0, 0.0);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, zero);

	t_pt neg = ft_pt_create(-1.5, -2.5, -3.5);
	ASSERT_PT_EQ(-1.5, -2.5, -3.5, neg);
}

TEST(test_ft_pt_init)
{
	t_pt p;
	ft_pt_init(&p, 4.0, 5.0, 6.0);
	ASSERT_PT_EQ(4.0, 5.0, 6.0, p);
}

TEST(test_ft_swap_pt)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_pt_create(4.0, 5.0, 6.0);
	ft_swap_pt(&a, &b);
	ASSERT_PT_EQ(4.0, 5.0, 6.0, a);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, b);
}

TEST(test_ft_inv_pt)
{
	t_pt p = ft_pt_create(2.0, 4.0, 5.0);
	ft_inv_pt(&p);
	ASSERT_DBL_EQ(0.5, p.x);
	ASSERT_DBL_EQ(0.25, p.y);
	ASSERT_DBL_EQ(0.2, p.z);
}

/* ========== ft_vectors.c tests ========== */

TEST(test_ft_lenght)
{
	t_pt a = ft_pt_create(3.0, 4.0, 0.0);
	ASSERT_DBL_EQ(5.0, ft_lenght(a));

	t_pt b = ft_pt_create(1.0, 0.0, 0.0);
	ASSERT_DBL_EQ(1.0, ft_lenght(b));

	t_pt c = ft_pt_create(0.0, 0.0, 0.0);
	ASSERT_DBL_EQ(0.0, ft_lenght(c));

	t_pt d = ft_pt_create(1.0, 1.0, 1.0);
	ASSERT_DBL_EQ(sqrt(3.0), ft_lenght(d));
}

TEST(test_ft_dot_product)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_pt_create(4.0, 5.0, 6.0);
	ASSERT_DBL_EQ(32.0, ft_dot_product(a, b));

	t_pt x = ft_pt_create(1.0, 0.0, 0.0);
	t_pt y = ft_pt_create(0.0, 1.0, 0.0);
	ASSERT_DBL_EQ(0.0, ft_dot_product(x, y));

	ASSERT_DBL_EQ(1.0, ft_dot_product(x, x));
}

TEST(test_ft_normal_vect)
{
	t_pt a = ft_pt_create(3.0, 0.0, 0.0);
	t_pt n = ft_normal_vect(a);
	ASSERT_PT_EQ(1.0, 0.0, 0.0, n);

	t_pt b = ft_pt_create(0.0, 5.0, 0.0);
	t_pt nb = ft_normal_vect(b);
	ASSERT_PT_EQ(0.0, 1.0, 0.0, nb);

	t_pt c = ft_pt_create(1.0, 1.0, 1.0);
	t_pt nc = ft_normal_vect(c);
	double inv_sqrt3 = 1.0 / sqrt(3.0);
	ASSERT_PT_EQ(inv_sqrt3, inv_sqrt3, inv_sqrt3, nc);
	ASSERT_DBL_EQ(1.0, ft_lenght(nc));
}

TEST(test_ft_inv_norm)
{
	t_pt p = ft_pt_create(1.0, -2.0, 3.0);
	ft_inv_norm(&p);
	ASSERT_PT_EQ(-1.0, 2.0, -3.0, p);
}

TEST(test_ft_div_vect)
{
	t_pt a = ft_pt_create(10.0, 20.0, 30.0);
	t_pt b = ft_pt_create(2.0, 4.0, 5.0);
	t_pt c = ft_div_vect(a, b);
	ASSERT_PT_EQ(5.0, 5.0, 6.0, c);
}

/* ========== ft_vectors_2.c tests ========== */

TEST(test_ft_cross_product)
{
	t_pt x = ft_pt_create(1.0, 0.0, 0.0);
	t_pt y = ft_pt_create(0.0, 1.0, 0.0);
	t_pt z = ft_cross_product(x, y);
	ASSERT_PT_EQ(0.0, 0.0, 1.0, z);

	t_pt z2 = ft_cross_product(y, x);
	ASSERT_PT_EQ(0.0, 0.0, -1.0, z2);

	t_pt self = ft_cross_product(x, x);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, self);
}

TEST(test_ft_addition)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_pt_create(4.0, 5.0, 6.0);
	t_pt c = ft_addition(a, b);
	ASSERT_PT_EQ(5.0, 7.0, 9.0, c);

	t_pt zero = ft_pt_create(0.0, 0.0, 0.0);
	t_pt d = ft_addition(a, zero);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, d);
}

TEST(test_ft_subtraction)
{
	t_pt a = ft_pt_create(5.0, 7.0, 9.0);
	t_pt b = ft_pt_create(4.0, 5.0, 6.0);
	t_pt c = ft_subtraction(a, b);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, c);

	t_pt self = ft_subtraction(a, a);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, self);
}

TEST(test_ft_multi_vect)
{
	t_pt a = ft_pt_create(2.0, 3.0, 4.0);
	t_pt b = ft_pt_create(5.0, 6.0, 7.0);
	t_pt c = ft_multi_vect(a, b);
	ASSERT_PT_EQ(10.0, 18.0, 28.0, c);
}

TEST(test_ft_neg_pt)
{
	t_pt a = ft_pt_create(1.0, -2.0, 3.0);
	t_pt b = ft_neg_pt(a);
	ASSERT_PT_EQ(-1.0, 2.0, -3.0, b);

	t_pt zero = ft_pt_create(0.0, 0.0, 0.0);
	t_pt nz = ft_neg_pt(zero);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, nz);
}

/* ========== ft_scalar.c tests ========== */

TEST(test_ft_multi_scal)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_multi_scal(2.0, a);
	ASSERT_PT_EQ(2.0, 4.0, 6.0, b);

	t_pt c = ft_multi_scal(0.0, a);
	ASSERT_PT_EQ(0.0, 0.0, 0.0, c);

	t_pt d = ft_multi_scal(-1.0, a);
	ASSERT_PT_EQ(-1.0, -2.0, -3.0, d);
}

TEST(test_ft_div_scal)
{
	t_pt a = ft_pt_create(4.0, 6.0, 8.0);
	t_pt b = ft_div_scal(2.0, a);
	ASSERT_PT_EQ(2.0, 3.0, 4.0, b);

	t_pt c = ft_div_scal(1.0, a);
	ASSERT_PT_EQ(4.0, 6.0, 8.0, c);
}

TEST(test_ft_add_scal)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_add_scal(10.0, a);
	ASSERT_PT_EQ(11.0, 12.0, 13.0, b);

	t_pt c = ft_add_scal(0.0, a);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, c);
}

TEST(test_ft_sub_scal)
{
	t_pt a = ft_pt_create(10.0, 20.0, 30.0);
	t_pt b = ft_sub_scal(5.0, a);
	ASSERT_PT_EQ(5.0, 15.0, 25.0, b);
}

/* ========== ft_matrix.c tests ========== */

TEST(test_ft_point_matrix_transl)
{
	t_mat identity;
	identity.x = ft_pt_create(1.0, 0.0, 0.0);
	identity.y = ft_pt_create(0.0, 1.0, 0.0);
	identity.z = ft_pt_create(0.0, 0.0, 1.0);

	t_pt p = ft_pt_create(3.0, 4.0, 5.0);
	t_pt r = ft_point_matrix_transl(p, identity);
	ASSERT_PT_EQ(3.0, 4.0, 5.0, r);

	t_mat scale2;
	scale2.x = ft_pt_create(2.0, 0.0, 0.0);
	scale2.y = ft_pt_create(0.0, 2.0, 0.0);
	scale2.z = ft_pt_create(0.0, 0.0, 2.0);

	t_pt s = ft_point_matrix_transl(p, scale2);
	ASSERT_PT_EQ(6.0, 8.0, 10.0, s);
}

TEST(test_ft_translation)
{
	t_pt coord = ft_pt_create(1.0, 2.0, 3.0);
	t_pt m1 = ft_pt_create(10.0, 20.0, 30.0);
	t_pt r = ft_translation(coord, m1);
	ASSERT_PT_EQ(11.0, 22.0, 33.0, r);

	t_pt zero = ft_pt_create(0.0, 0.0, 0.0);
	t_pt r2 = ft_translation(zero, m1);
	ASSERT_PT_EQ(10.0, 20.0, 30.0, r2);
}

/* ========== ft_argb.c tests ========== */

TEST(test_ft_multi_argb)
{
	t_argb a = {0, 0.5, 0.5, 0.5};
	t_argb b = {0, 2.0, 4.0, 6.0};
	t_argb c = ft_multi_argb(a, b);
	ASSERT_ARGB_EQ(1.0, 2.0, 3.0, c);
}

TEST(test_ft_albedo)
{
	t_pt a = ft_pt_create(255.0, 255.0, 255.0);
	t_argb col = {0, 255.0, 255.0, 255.0};
	t_argb r = ft_albedo(a, col);
	ASSERT_ARGB_EQ(255.0, 255.0, 255.0, r);

	t_pt half = ft_pt_create(127.5, 127.5, 127.5);
	t_argb r2 = ft_albedo(half, col);
	ASSERT_ARGB_EQ(127.5, 127.5, 127.5, r2);
}

TEST(test_ft_db_mult_to_add_pt)
{
	t_pt a = ft_pt_create(0.0, 0.0, 0.0);
	t_argb c = {0, 10.0, 20.0, 30.0};
	ft_db_mult_to_add_pt(&a, 2.0, c);
	ASSERT_PT_EQ(20.0, 40.0, 60.0, a);

	ft_db_mult_to_add_pt(&a, 0.5, c);
	ASSERT_PT_EQ(25.0, 50.0, 75.0, a);
}

/* ========== ft_cam.c rotation tests ========== */

TEST(test_ft_rotate_arr_x)
{
	t_pt p = ft_pt_create(0.0, 1.0, 0.0);
	t_pt r = ft_rotate_arr_x(p, M_PI / 2.0);
	ASSERT_PT_EQ(0.0, 0.0, -1.0, r);

	t_pt r2 = ft_rotate_arr_x(p, 0.0);
	ASSERT_PT_EQ(0.0, 1.0, 0.0, r2);
}

TEST(test_ft_rotate_arr_y)
{
	t_pt p = ft_pt_create(1.0, 0.0, 0.0);
	t_pt r = ft_rotate_arr_y(p, M_PI / 2.0);
	ASSERT_PT_EQ(0.0, 0.0, 1.0, r);
}

TEST(test_ft_rotate_arr_z)
{
	t_pt p = ft_pt_create(1.0, 0.0, 0.0);
	t_pt r = ft_rotate_arr_z(p, M_PI / 2.0);
	ASSERT_PT_EQ(0.0, -1.0, 0.0, r);
}

TEST(test_ft_rotate_identity)
{
	t_pt p = ft_pt_create(1.0, 2.0, 3.0);
	t_pt rx = ft_rotate_arr_x(p, 0.0);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, rx);
	t_pt ry = ft_rotate_arr_y(p, 0.0);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, ry);
	t_pt rz = ft_rotate_arr_z(p, 0.0);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, rz);
}

TEST(test_ft_rotate_full_circle)
{
	t_pt p = ft_pt_create(1.0, 2.0, 3.0);
	t_pt rx = ft_rotate_arr_x(p, 2.0 * M_PI);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, rx);
	t_pt ry = ft_rotate_arr_y(p, 2.0 * M_PI);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, ry);
	t_pt rz = ft_rotate_arr_z(p, 2.0 * M_PI);
	ASSERT_PT_EQ(1.0, 2.0, 3.0, rz);
}

/* ========== Mathematical property tests ========== */

TEST(test_dot_product_commutative)
{
	t_pt a = ft_pt_create(1.5, 2.7, -3.1);
	t_pt b = ft_pt_create(-0.5, 4.2, 1.8);
	ASSERT_DBL_EQ(ft_dot_product(a, b), ft_dot_product(b, a));
}

TEST(test_cross_product_anticommutative)
{
	t_pt a = ft_pt_create(1.5, 2.7, -3.1);
	t_pt b = ft_pt_create(-0.5, 4.2, 1.8);
	t_pt axb = ft_cross_product(a, b);
	t_pt bxa = ft_cross_product(b, a);
	ASSERT_PT_EQ(-bxa.x, -bxa.y, -bxa.z, axb);
}

TEST(test_cross_product_perpendicular)
{
	t_pt a = ft_pt_create(1.0, 2.0, 3.0);
	t_pt b = ft_pt_create(4.0, 5.0, 6.0);
	t_pt c = ft_cross_product(a, b);
	ASSERT_DBL_EQ(0.0, ft_dot_product(a, c));
	ASSERT_DBL_EQ(0.0, ft_dot_product(b, c));
}

TEST(test_normalize_unit_length)
{
	t_pt a = ft_pt_create(3.0, 4.0, 12.0);
	t_pt n = ft_normal_vect(a);
	ASSERT_DBL_EQ(1.0, ft_lenght(n));

	t_pt b = ft_pt_create(-7.0, 2.5, 0.3);
	t_pt nb = ft_normal_vect(b);
	ASSERT_DBL_EQ(1.0, ft_lenght(nb));
}

TEST(test_addition_subtraction_inverse)
{
	t_pt a = ft_pt_create(1.5, 2.7, -3.1);
	t_pt b = ft_pt_create(-0.5, 4.2, 1.8);
	t_pt sum = ft_addition(a, b);
	t_pt diff = ft_subtraction(sum, b);
	ASSERT_PT_EQ(a.x, a.y, a.z, diff);
}

TEST(test_scalar_mult_div_inverse)
{
	t_pt a = ft_pt_create(3.0, 6.0, 9.0);
	t_pt scaled = ft_multi_scal(4.0, a);
	t_pt back = ft_div_scal(4.0, scaled);
	ASSERT_PT_EQ(a.x, a.y, a.z, back);
}

TEST(test_neg_is_scalar_minus_one)
{
	t_pt a = ft_pt_create(1.5, -2.7, 3.1);
	t_pt neg = ft_neg_pt(a);
	t_pt scal = ft_multi_scal(-1.0, a);
	ASSERT_PT_EQ(scal.x, scal.y, scal.z, neg);
}

int	main(void)
{
	TEST_SUITE("ft_pt.c");
	RUN_TEST(test_ft_sqr);
	RUN_TEST(test_ft_pt_create);
	RUN_TEST(test_ft_pt_init);
	RUN_TEST(test_ft_swap_pt);
	RUN_TEST(test_ft_inv_pt);

	TEST_SUITE("ft_vectors.c");
	RUN_TEST(test_ft_lenght);
	RUN_TEST(test_ft_dot_product);
	RUN_TEST(test_ft_normal_vect);
	RUN_TEST(test_ft_inv_norm);
	RUN_TEST(test_ft_div_vect);

	TEST_SUITE("ft_vectors_2.c");
	RUN_TEST(test_ft_cross_product);
	RUN_TEST(test_ft_addition);
	RUN_TEST(test_ft_subtraction);
	RUN_TEST(test_ft_multi_vect);
	RUN_TEST(test_ft_neg_pt);

	TEST_SUITE("ft_scalar.c");
	RUN_TEST(test_ft_multi_scal);
	RUN_TEST(test_ft_div_scal);
	RUN_TEST(test_ft_add_scal);
	RUN_TEST(test_ft_sub_scal);

	TEST_SUITE("ft_matrix.c");
	RUN_TEST(test_ft_point_matrix_transl);
	RUN_TEST(test_ft_translation);

	TEST_SUITE("ft_argb.c");
	RUN_TEST(test_ft_multi_argb);
	RUN_TEST(test_ft_albedo);
	RUN_TEST(test_ft_db_mult_to_add_pt);

	TEST_SUITE("ft_cam.c rotations");
	RUN_TEST(test_ft_rotate_arr_x);
	RUN_TEST(test_ft_rotate_arr_y);
	RUN_TEST(test_ft_rotate_arr_z);
	RUN_TEST(test_ft_rotate_identity);
	RUN_TEST(test_ft_rotate_full_circle);

	TEST_SUITE("Mathematical properties");
	RUN_TEST(test_dot_product_commutative);
	RUN_TEST(test_cross_product_anticommutative);
	RUN_TEST(test_cross_product_perpendicular);
	RUN_TEST(test_normalize_unit_length);
	RUN_TEST(test_addition_subtraction_inverse);
	RUN_TEST(test_scalar_mult_div_inverse);
	RUN_TEST(test_neg_is_scalar_minus_one);

	TEST_REPORT();
}
