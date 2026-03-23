#ifndef MINIRT_TEST_H
# define MINIRT_TEST_H

# include <stdio.h>
# include <math.h>
# include <string.h>

# define EPSILON 1e-6

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

# define TEST(name) static void name(void)

# define RUN_TEST(name) do { \
	g_tests_run++; \
	name(); \
} while (0)

# define ASSERT_DBL_EQ(expected, actual) do { \
	double _e = (expected), _a = (actual); \
	if (fabs(_e - _a) < EPSILON) { \
		g_tests_passed++; \
	} else { \
		g_tests_failed++; \
		printf("  FAIL %s:%d: expected %.10f, got %.10f\n", \
			__FILE__, __LINE__, _e, _a); \
	} \
} while (0)

# define ASSERT_PT_EQ(ex, ey, ez, actual) do { \
	ASSERT_DBL_EQ(ex, (actual).x); \
	ASSERT_DBL_EQ(ey, (actual).y); \
	ASSERT_DBL_EQ(ez, (actual).z); \
} while (0)

# define ASSERT_ARGB_EQ(er, eg, eb, actual) do { \
	ASSERT_DBL_EQ(er, (actual).r); \
	ASSERT_DBL_EQ(eg, (actual).g); \
	ASSERT_DBL_EQ(eb, (actual).b); \
} while (0)

# define ASSERT_TRUE(cond) do { \
	if (cond) { \
		g_tests_passed++; \
	} else { \
		g_tests_failed++; \
		printf("  FAIL %s:%d: condition false\n", __FILE__, __LINE__); \
	} \
} while (0)

# define TEST_SUITE(name) printf("--- %s ---\n", name)

# define TEST_REPORT() do { \
	printf("\n=== Results: %d tests, %d passed, %d failed ===\n", \
		g_tests_run, g_tests_passed, g_tests_failed); \
	return (g_tests_failed > 0) ? 1 : 0; \
} while (0)

#endif
