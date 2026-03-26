#include "mini_rt.h"
#include "minirt_test.h"

int g_tests_run = 0;
int g_tests_passed = 0;
int g_tests_failed = 0;

void	run_math_tests(void);
void	run_camera_tests(void);
void	run_intersection_tests(void);
void	run_lighting_tests(void);
void	run_parsing_tests(void);
void	run_pbr_tests(void);

int	main(void)
{
	run_math_tests();
	run_camera_tests();
	run_intersection_tests();
	run_lighting_tests();
	run_parsing_tests();
	run_pbr_tests();
	TEST_REPORT();
}
