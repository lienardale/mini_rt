# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alienard <alienard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/09 18:51:33 by alienard          #+#    #+#              #
#    Updated: 2022/06/14 16:46:17 by alienard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 		miniRT

OS_NAME := $(shell uname -s)

MLX_DEF = -D $(OS_NAME)

MLX = libmlx.a
MLX_LIBS = -lmlx
MLX_INC = -L$(MLX_DIR)

MLX_DIR =
ifeq ($(OS_NAME),Darwin)
	MLX_DIR += mlx
	MLX_INC += -framework OpenGL -framework AppKit
else
	MLX_DIR += mlx_linux
	MLX_LIBS += -lXext -lX11 -lm -lz
	KB_LAYOUT += $(shell setxkbmap -query | grep -oP "layout:\s+\K\w+")
endif

LIB = libft.a
LIB_INC = -L$(LIBPATH) -lft
LIBPATH = libft

INC = -I includes -I$(LIBPATH) -I$(MLX_DIR)

SRC_DIR = src

SRC = $(addprefix $(SRC_DIR)/,$(SRC_NAME))

SRC_NAME =		ft_mini_rt.c \
			ft_parsing.c \
			ft_parsing_2.c \
			ft_bzero_struct.c \
			ft_check_parsing.c \
			ft_check_parsing_2.c \
			ft_cam.c \
			ft_light.c \
			ft_sphere.c \
			ft_plane.c \
			ft_square.c \
			ft_cylinder.c \
			ft_triangle.c \
			ft_cone.c \
			ft_disk.c \
			ft_torus.c \
			ft_ellipsoid.c \
			ft_box.c \
			ft_quadric.c \
			ft_paraboloid.c \
			ft_csg.c \
			ft_mesh.c \
			ft_material.c \
			ft_texture.c \
			ft_bump.c \
			ft_argb.c \
			ft_event.c \
			ft_hud.c \
			ft_cam_move.c \
			ft_vectors.c \
			ft_vectors_2.c \
			ft_scalar.c \
			ft_matrix.c \
			ft_ray.c \
			ft_ray_2.c \
			ft_pt.c \
			ft_precompute.c \
			ft_bvh.c \
			ft_bvh_2.c \
			ft_threads.c \
			ft_random.c \
			ft_dof.c \
			ft_motion.c \
			ft_pathtrace.c \
			ft_pbr.c \
			ft_env.c \
			ft_close.c \
			ft_save.c \
			ft_error.c

OBJ_DIR = obj

OBJ_NAME = $(SRC_NAME:.c=.o)

OBJ = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))

CC =		gcc

RM = 		rm -f

CFLAGS =	-Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wno-strict-prototypes -g3 -O3

DBGFLAGS =	-Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wno-strict-prototypes -g3 -O0 \
			-fsanitize=address,undefined -fno-omit-frame-pointer

RELFLAGS =	-Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wno-strict-prototypes -O3 -DNDEBUG

SANFLAGS =	$(DBGFLAGS)

LDFLAGS =

all:		$(LIB) $(MLX) $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	    $(CC) $(INC) $(CFLAGS) $(MLX_DEF) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $@


$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJ) $(INC) $(LIB_INC) $(MLX_INC) $(MLX_LIBS) -lpthread

$(MLX):
		@make -C $(MLX_DIR) $(MLX_LIBS)

$(LIB):
		@make -C $(LIBPATH)

clean:
			$(RM) -r $(OBJ_DIR)
			@make -C $(LIBPATH) clean

fclean:		clean
			$(RM) $(NAME)
			@make -C $(MLX_DIR) clean
			@make -C $(LIBPATH) fclean

re:			fclean all

debug:		$(MLX)
			@make -C $(LIBPATH) fclean
			@make -C $(LIBPATH)
			$(RM) -r $(OBJ_DIR)
			$(RM) $(NAME)
			$(MAKE) CFLAGS="$(DBGFLAGS)" LDFLAGS="-fsanitize=address,undefined" $(NAME)

release:	$(MLX)
			@make -C $(LIBPATH) fclean
			@make -C $(LIBPATH)
			$(RM) -r $(OBJ_DIR)
			$(RM) $(NAME)
			$(MAKE) CFLAGS="$(RELFLAGS)" $(NAME)

sanitize:	debug

TEST_MATH_SRCS = ft_vectors.c ft_vectors_2.c ft_scalar.c ft_matrix.c \
				 ft_pt.c ft_argb.c ft_cam.c ft_cam_move.c

SHAPE_SRCS = ft_sphere.c ft_plane.c ft_square.c \
			 ft_cylinder.c ft_triangle.c \
			 ft_cone.c ft_disk.c ft_torus.c ft_ellipsoid.c \
			 ft_box.c ft_quadric.c ft_paraboloid.c ft_csg.c \
			 ft_mesh.c ft_material.c ft_texture.c ft_bump.c

TEST_INTERSECT_SRCS = $(TEST_MATH_SRCS) $(SHAPE_SRCS) \
					  ft_ray_2.c ft_ray.c ft_light.c ft_precompute.c \
					  ft_bvh.c ft_bvh_2.c \
					  ft_random.c ft_dof.c ft_motion.c ft_pathtrace.c ft_pbr.c ft_env.c \
					  ft_parsing.c ft_parsing_2.c ft_check_parsing.c \
					  ft_check_parsing_2.c ft_bzero_struct.c

TEST_PARSING_SRCS = $(TEST_MATH_SRCS) ft_parsing.c ft_parsing_2.c \
					ft_check_parsing.c ft_check_parsing_2.c ft_bzero_struct.c \
					$(SHAPE_SRCS) ft_precompute.c \
					ft_bvh.c ft_bvh_2.c ft_ray.c ft_ray_2.c ft_light.c \
					ft_random.c ft_dof.c ft_motion.c ft_pathtrace.c ft_pbr.c ft_env.c

TEST_DIR = tests

TEST_INC = -I includes -I$(LIBPATH) -I$(MLX_DIR) -I$(TEST_DIR)
TEST_LINK = $(LIB_INC) -lm

test:		$(LIB)
			@echo "=== Running math tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_math.c \
				$(addprefix $(SRC_DIR)/,$(TEST_MATH_SRCS)) \
				$(TEST_LINK) -o run_test_math && ./run_test_math
			@echo ""
			@echo "=== Running camera tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_camera.c \
				$(addprefix $(SRC_DIR)/,$(TEST_MATH_SRCS)) \
				$(TEST_LINK) -o run_test_camera && ./run_test_camera
			@echo ""
			@echo "=== Running intersection tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_intersections.c $(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_INTERSECT_SRCS)) \
				$(TEST_LINK) -o run_test_intersections && ./run_test_intersections
			@echo ""
			@echo "=== Running lighting tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_lighting.c $(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_INTERSECT_SRCS)) \
				$(TEST_LINK) -o run_test_lighting && ./run_test_lighting
			@echo ""
			@echo "=== Running parsing tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_parsing.c $(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_PARSING_SRCS)) \
				$(TEST_LINK) -o run_test_parsing && ./run_test_parsing
			@echo ""
			@echo "=== Running PBR tests ==="
			@$(CC) -Wall -Wextra -Werror -g3 $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_pbr.c $(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_INTERSECT_SRCS)) \
				$(TEST_LINK) -o run_test_pbr && ./run_test_pbr

COV_FLAGS = -Wall -Wextra -g3 --coverage -fprofile-arcs -ftest-coverage

coverage:	$(LIB)
			@mkdir -p coverage
			@$(CC) $(COV_FLAGS) -DTEST_ALL $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_all.c \
				$(TEST_DIR)/test_math.c \
				$(TEST_DIR)/test_camera.c \
				$(TEST_DIR)/test_intersections.c \
				$(TEST_DIR)/test_lighting.c \
				$(TEST_DIR)/test_parsing.c \
				$(TEST_DIR)/test_pbr.c \
				$(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_INTERSECT_SRCS)) \
				$(TEST_LINK) -o cov_test_all && ./cov_test_all
			@lcov --capture --directory . --output-file coverage/coverage.info \
				--ignore-errors mismatch 2>/dev/null
			@lcov --remove coverage/coverage.info '*/libft/*' '*/tests/*' \
				'/usr/*' --output-file coverage/coverage.info \
				--ignore-errors unused 2>/dev/null
			@genhtml coverage/coverage.info --output-directory coverage/html 2>/dev/null
			@lcov --summary coverage/coverage.info 2>/dev/null || true
			@echo "\nCoverage report: coverage/html/index.html"

regression:	$(NAME)
			@$(TEST_DIR)/regression/run_regression.sh

regression-generate:	$(NAME)
			@$(TEST_DIR)/regression/run_regression.sh --generate

lint:
			@clang-format --dry-run --Werror src/*.c includes/*.h 2>&1 \
				&& echo "Lint: OK" || echo "Lint: formatting issues found"

TEST_BENCH_SRCS = $(TEST_MATH_SRCS) $(SHAPE_SRCS) \
				  ft_ray_2.c ft_ray.c ft_light.c ft_precompute.c \
				  ft_bvh.c ft_bvh_2.c ft_bzero_struct.c \
				  ft_random.c ft_dof.c ft_motion.c ft_pathtrace.c ft_pbr.c ft_env.c \
				  ft_parsing.c ft_parsing_2.c ft_check_parsing.c \
				  ft_check_parsing_2.c

benchmark:	$(LIB)
			@$(CC) -Wall -Wextra -Werror -g3 -ofast $(MLX_DEF) $(TEST_INC) \
				$(TEST_DIR)/test_benchmark.c $(TEST_DIR)/test_stubs.c \
				$(addprefix $(SRC_DIR)/,$(TEST_BENCH_SRCS)) \
				$(TEST_LINK) -lpthread -o run_benchmark \
				&& ./run_benchmark

gen_scene:
			@$(CC) -Wall -Wextra -Werror -o gen_scene tools/gen_scene.c -lm
			@echo "Built gen_scene tool"

render-cathedral:	$(NAME) gen_scene
			@scripts/render_cathedral.sh $(ARGS)

testclean:
			$(RM) run_test_math run_test_camera run_test_intersections run_test_lighting run_test_parsing run_test_pbr run_benchmark
			$(RM) cov_test_math cov_test_intersections cov_test_lighting cov_test_parsing cov_test_all
			$(RM) -r coverage
			$(RM) *.gcno *.gcda src/*.gcno src/*.gcda tests/*.gcno tests/*.gcda

.PHONY:		re all clean fclean sanitize debug release test testclean coverage regression regression-generate lint benchmark gen_scene render-cathedral
