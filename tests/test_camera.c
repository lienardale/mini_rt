#include "mini_rt.h"
#include "minirt_test.h"

/* ========== ft_cam_forward_xz tests ========== */

TEST(test_cam_forward_xz_default)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	t_pt fwd = ft_cam_forward_xz(&cam);
	ASSERT_PT_EQ(0.0, 0.0, -1.0, fwd);
}

TEST(test_cam_forward_xz_quarter_turn)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0.5;
	t_pt fwd = ft_cam_forward_xz(&cam);
	ASSERT_PT_EQ(-1.0, 0.0, 0.0, fwd);
}

TEST(test_cam_forward_xz_half_turn)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 1.0;
	t_pt fwd = ft_cam_forward_xz(&cam);
	ASSERT_PT_EQ(0.0, 0.0, 1.0, fwd);
}

TEST(test_cam_forward_xz_neg_quarter)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = -0.5;
	t_pt fwd = ft_cam_forward_xz(&cam);
	ASSERT_PT_EQ(1.0, 0.0, 0.0, fwd);
}

/* ========== ft_cam_right_xz tests ========== */

TEST(test_cam_right_xz_default)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	t_pt right = ft_cam_right_xz(&cam);
	ASSERT_PT_EQ(1.0, 0.0, 0.0, right);
}

TEST(test_cam_right_xz_quarter_turn)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0.5;
	t_pt right = ft_cam_right_xz(&cam);
	ASSERT_PT_EQ(0.0, 0.0, -1.0, right);
}

/* ========== ft_cam_apply_movement tests ========== */

TEST(test_cam_apply_movement_forward)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	ft_cam_apply_movement(&cam, KEY_BIT_FWD);
	ASSERT_DBL_EQ(0.0, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(-MOVE_STEP, cam.coord.z);
}

TEST(test_cam_apply_movement_backward)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	ft_cam_apply_movement(&cam, KEY_BIT_BKW);
	ASSERT_DBL_EQ(0.0, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(MOVE_STEP, cam.coord.z);
}

TEST(test_cam_apply_movement_strafe_right)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	ft_cam_apply_movement(&cam, KEY_BIT_RIGHT);
	ASSERT_DBL_EQ(MOVE_STEP, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(0.0, cam.coord.z);
}

TEST(test_cam_apply_movement_strafe_left)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	ft_cam_apply_movement(&cam, KEY_BIT_LEFT);
	ASSERT_DBL_EQ(-MOVE_STEP, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(0.0, cam.coord.z);
}

TEST(test_cam_apply_movement_up_down)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	ft_cam_apply_movement(&cam, KEY_BIT_UP);
	ASSERT_DBL_EQ(MOVE_STEP, cam.coord.y);
	ft_cam_apply_movement(&cam, KEY_BIT_DOWN);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
}

TEST(test_cam_apply_movement_rotated)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0.5;
	ft_cam_apply_movement(&cam, KEY_BIT_FWD);
	ASSERT_DBL_EQ(-MOVE_STEP, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(0.0, cam.coord.z);
}

TEST(test_cam_apply_movement_combined)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	cam.ori.y = 0;
	ft_cam_apply_movement(&cam, KEY_BIT_FWD | KEY_BIT_RIGHT);
	ASSERT_DBL_EQ(MOVE_STEP, cam.coord.x);
	ASSERT_DBL_EQ(0.0, cam.coord.y);
	ASSERT_DBL_EQ(-MOVE_STEP, cam.coord.z);
}

/* ========== ft_cam_apply_look tests ========== */

TEST(test_cam_apply_look)
{
	t_cam cam;

	memset(&cam, 0, sizeof(t_cam));
	ft_cam_apply_look(&cam, KEY_BIT_LK_R);
	ASSERT_DBL_EQ(LOOK_STEP, cam.ori.y);
	ft_cam_apply_look(&cam, KEY_BIT_LK_L);
	ASSERT_DBL_EQ(0.0, cam.ori.y);
	ft_cam_apply_look(&cam, KEY_BIT_LK_D);
	ASSERT_DBL_EQ(LOOK_STEP, cam.ori.x);
	ft_cam_apply_look(&cam, KEY_BIT_LK_U);
	ASSERT_DBL_EQ(0.0, cam.ori.x);
	ft_cam_apply_look(&cam, KEY_BIT_LK_ZR);
	ASSERT_DBL_EQ(LOOK_STEP, cam.ori.z);
	ft_cam_apply_look(&cam, KEY_BIT_LK_ZL);
	ASSERT_DBL_EQ(0.0, cam.ori.z);
}

/* ========== ft_keycode_to_bit tests ========== */

TEST(test_keycode_to_bit)
{
	ASSERT_TRUE(ft_keycode_to_bit(MV_FWD) == KEY_BIT_FWD);
	ASSERT_TRUE(ft_keycode_to_bit(MV_BKW) == KEY_BIT_BKW);
	ASSERT_TRUE(ft_keycode_to_bit(MV_L) == KEY_BIT_LEFT);
	ASSERT_TRUE(ft_keycode_to_bit(MV_R) == KEY_BIT_RIGHT);
	ASSERT_TRUE(ft_keycode_to_bit(MV_UP) == KEY_BIT_UP);
	ASSERT_TRUE(ft_keycode_to_bit(MV_DWN) == KEY_BIT_DOWN);
	ASSERT_TRUE(ft_keycode_to_bit(LK_LFT) == KEY_BIT_LK_L);
	ASSERT_TRUE(ft_keycode_to_bit(LK_RGT) == KEY_BIT_LK_R);
	ASSERT_TRUE(ft_keycode_to_bit(LK_UP) == KEY_BIT_LK_U);
	ASSERT_TRUE(ft_keycode_to_bit(LK_DWN) == KEY_BIT_LK_D);
	ASSERT_TRUE(ft_keycode_to_bit(LK_ZL) == KEY_BIT_LK_ZL);
	ASSERT_TRUE(ft_keycode_to_bit(LK_ZR) == KEY_BIT_LK_ZR);
	ASSERT_TRUE(ft_keycode_to_bit(-1) == 0);
	ASSERT_TRUE(ft_keycode_to_bit(999999) == 0);
}

/* ========== Main ========== */

void	run_camera_tests(void);

#ifndef TEST_ALL
int	main(void)
{
	run_camera_tests();
	TEST_REPORT();
}
#endif

void	run_camera_tests(void)
{
	TEST_SUITE("ft_cam_move.c - forward/right vectors");
	RUN_TEST(test_cam_forward_xz_default);
	RUN_TEST(test_cam_forward_xz_quarter_turn);
	RUN_TEST(test_cam_forward_xz_half_turn);
	RUN_TEST(test_cam_forward_xz_neg_quarter);
	RUN_TEST(test_cam_right_xz_default);
	RUN_TEST(test_cam_right_xz_quarter_turn);
	TEST_SUITE("ft_cam_move.c - apply movement");
	RUN_TEST(test_cam_apply_movement_forward);
	RUN_TEST(test_cam_apply_movement_backward);
	RUN_TEST(test_cam_apply_movement_strafe_right);
	RUN_TEST(test_cam_apply_movement_strafe_left);
	RUN_TEST(test_cam_apply_movement_up_down);
	RUN_TEST(test_cam_apply_movement_rotated);
	RUN_TEST(test_cam_apply_movement_combined);
	TEST_SUITE("ft_cam_move.c - apply look");
	RUN_TEST(test_cam_apply_look);
	TEST_SUITE("ft_event.c - keycode to bit");
	RUN_TEST(test_keycode_to_bit);
}
