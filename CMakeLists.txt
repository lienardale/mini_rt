cmake_minimum_required(VERSION 3.16)
project(miniRT LANGUAGES C)

# ---------- Platform detection ----------
if(APPLE)
    set(MLX_DIR ${CMAKE_SOURCE_DIR}/mlx)
    set(PLATFORM_DEF Darwin)
else()
    set(MLX_DIR ${CMAKE_SOURCE_DIR}/mlx_linux)
    set(PLATFORM_DEF Linux)
endif()

# ---------- Build type ----------
set(CMAKE_C_STANDARD 99)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_C_FLAGS_DEBUG   "-g3 -O0 -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# ---------- libft (subdirectory build, own flags) ----------
add_subdirectory(libft)

# ---------- minilibX (built via its own Makefile) ----------
add_custom_command(
    OUTPUT ${MLX_DIR}/libmlx.a
    COMMAND make -C ${MLX_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building minilibX"
)
add_custom_target(mlx_build DEPENDS ${MLX_DIR}/libmlx.a)

add_library(mlx STATIC IMPORTED)
set_target_properties(mlx PROPERTIES IMPORTED_LOCATION ${MLX_DIR}/libmlx.a)

# ---------- miniRT sources ----------
set(MINIRT_SRCS
    src/ft_mini_rt.c
    src/ft_parsing.c
    src/ft_parsing_2.c
    src/ft_bzero_struct.c
    src/ft_check_parsing.c
    src/ft_check_parsing_2.c
    src/ft_cam.c
    src/ft_light.c
    src/ft_sphere.c
    src/ft_plane.c
    src/ft_square.c
    src/ft_cylinder.c
    src/ft_triangle.c
    src/ft_cone.c
    src/ft_disk.c
    src/ft_torus.c
    src/ft_ellipsoid.c
    src/ft_box.c
    src/ft_quadric.c
    src/ft_paraboloid.c
    src/ft_csg.c
    src/ft_mesh.c
    src/ft_material.c
    src/ft_texture.c
    src/ft_bump.c
    src/ft_argb.c
    src/ft_event.c
    src/ft_cam_move.c
    src/ft_vectors.c
    src/ft_vectors_2.c
    src/ft_scalar.c
    src/ft_matrix.c
    src/ft_ray.c
    src/ft_ray_2.c
    src/ft_pt.c
    src/ft_precompute.c
    src/ft_bvh.c
    src/ft_bvh_2.c
    src/ft_threads.c
    src/ft_close.c
    src/ft_save.c
    src/ft_error.c
)

add_executable(miniRT ${MINIRT_SRCS})
add_dependencies(miniRT mlx_build)
target_include_directories(miniRT PRIVATE includes ${MLX_DIR})
target_compile_definitions(miniRT PRIVATE ${PLATFORM_DEF})
target_compile_options(miniRT PRIVATE
    -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion)

# ---------- Link libraries ----------
if(APPLE)
    target_link_libraries(miniRT PRIVATE ft mlx
        "-framework OpenGL" "-framework AppKit" pthread)
else()
    target_link_libraries(miniRT PRIVATE ft mlx Xext X11 m z pthread)
endif()

# Propagate sanitizer flags to linker in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_options(miniRT PRIVATE -fsanitize=address,undefined)
endif()

# ---------- Tests ----------
enable_testing()

set(TEST_MATH_SRCS
    src/ft_vectors.c src/ft_vectors_2.c src/ft_scalar.c src/ft_matrix.c
    src/ft_pt.c src/ft_argb.c src/ft_cam.c src/ft_cam_move.c
)

add_executable(run_test_math EXCLUDE_FROM_ALL
    tests/test_math.c ${TEST_MATH_SRCS})
target_include_directories(run_test_math PRIVATE includes ${MLX_DIR} tests)
target_compile_definitions(run_test_math PRIVATE ${PLATFORM_DEF})
target_link_libraries(run_test_math PRIVATE ft m)
add_test(NAME math COMMAND run_test_math)

add_executable(run_test_camera EXCLUDE_FROM_ALL
    tests/test_camera.c ${TEST_MATH_SRCS})
target_include_directories(run_test_camera PRIVATE includes ${MLX_DIR} tests)
target_compile_definitions(run_test_camera PRIVATE ${PLATFORM_DEF})
target_link_libraries(run_test_camera PRIVATE ft m)
add_test(NAME camera COMMAND run_test_camera)
