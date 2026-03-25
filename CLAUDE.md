# CLAUDE.md - miniRT Project Guide

## Project Overview

**miniRT** is a feature-rich ray tracing engine written in C, originally built as a 42 Paris common core project and extended with advanced rendering, multi-threading, 14 shape types, and a full test/CI pipeline. It renders 3D scenes with realistic lighting, shadows, reflections, refraction, and multiple geometric objects using the minilibX graphics library.

- **Author:** alienard (42 Paris)
- **Language:** C (100%)
- **Lines of code:** ~6,700 (6,126 C + 586 headers across 47 source files)
- **Platform:** macOS (Darwin) and Linux

## Build

```bash
make            # Full build (libft + mlx + miniRT executable)
make clean      # Remove object files
make fclean     # Remove objects + executable
make re         # Clean rebuild
make debug      # Build with AddressSanitizer + UBSan
make release    # Optimized release build (-O3 -DNDEBUG)
make test       # Run unit tests (math, camera, intersections, lighting, parsing)
make coverage   # Run tests with gcov/lcov coverage reporting
make regression # Run regression tests (compare renders against reference BMPs)
make benchmark  # Performance benchmarking
make lint       # Check code style with clang-format
```

- **Compiler:** `gcc -Wall -Wextra -Werror -Wpedantic -g3 -O3`
- **Output:** `miniRT` executable in project root
- **Platform auto-detected** via `uname -s`

### Dependencies

- **minilibX** (included in repo: `mlx/` for macOS, `mlx_linux/` for Linux)
- **libft** (included in repo: custom 42 utility library with printf, get_next_line)
- **System libs:** X11, Xext, m, z (Linux) / OpenGL, AppKit (macOS)

## Usage

```bash
./miniRT <scene_file.rt>              # Render scene in window
./miniRT <scene_file.rt> -save        # Render and save as BMP
./miniRT <scene_file.rt> --threads N  # Use N render threads
./miniRT <scene_file.rt> --threads=auto  # Auto-detect thread count
```

Multiple options can be combined: `./miniRT scene.rt -save --threads 4`

### Controls

All movement and look keys support continuous hold for smooth FPS-style motion.

| Action              | Key              | Description                              |
|---------------------|------------------|------------------------------------------|
| Move forward/back   | W / S            | Relative to camera facing (XZ plane)     |
| Strafe left/right   | A / D            | Perpendicular to camera facing (XZ)      |
| Move up/down        | PgUp / PgDn      | World Y axis                             |
| Look left/right     | Left / Right      | Continuous yaw rotation                  |
| Look up/down        | Up / Down          | Continuous pitch rotation                |
| Roll camera         | , / .            | Continuous roll rotation                 |
| FOV +/-             | Numpad + / -      | Adjust field of view (0–180)             |
| Toggle resolution   | R                | Switch between draft (4x) and full       |
| Window resize       | 1 / 2 / 3 / 4    | Set window scale                         |
| Help overlay        | H                | Toggle on-screen help text               |
| Info/stats overlay  | I                | Toggle render stats (FPS, resolution)    |
| Next camera         | Space            | Cycle through scene cameras              |
| Exit                | ESC              | Close window                             |

## Project Structure

```
mini_rt/
├── src/                     # 47 C source files
│   ├── ft_mini_rt.c         # Entry point, init, main render loop
│   ├── ft_ray.c / ft_ray_2.c    # Core ray casting & intersection tracing
│   ├── ft_trace.c           # Ray-scene tracing
│   ├── ft_light.c           # Phong lighting, shadows, specular
│   ├── ft_sphere.c          # Sphere intersection & normals
│   ├── ft_plane.c           # Plane intersection & normals
│   ├── ft_cylinder.c        # Cylinder intersection & normals
│   ├── ft_square.c          # Square intersection & bounds
│   ├── ft_triangle.c        # Triangle intersection (barycentric)
│   ├── ft_cone.c            # Cone intersection & normals
│   ├── ft_disk.c            # Disk intersection
│   ├── ft_torus.c           # Torus quartic intersection
│   ├── ft_ellipsoid.c       # Ellipsoid intersection
│   ├── ft_box.c             # Box intersection (AABB)
│   ├── ft_quadric.c         # Quadric surface helpers
│   ├── ft_hyperboloid.c     # Hyperboloid intersection
│   ├── ft_paraboloid.c      # Paraboloid intersection
│   ├── ft_csg.c             # CSG operations (union, intersect, difference)
│   ├── ft_mesh.c            # OBJ mesh loading
│   ├── ft_material.c        # Material properties (refl, trans, spec, ior, PBR)
│   ├── ft_texture.c         # Texture mapping
│   ├── ft_bump.c            # Bump/normal mapping
│   ├── ft_bvh.c / ft_bvh_2.c    # BVH acceleration structure
│   ├── ft_threads.c         # Multi-threaded rendering
│   ├── ft_dof.c             # Depth of field
│   ├── ft_motion.c          # Motion blur
│   ├── ft_pathtrace.c       # Path tracing / global illumination
│   ├── ft_pbr.c             # PBR shading (GGX/Cook-Torrance BRDF, Fresnel)
│   ├── ft_env.c             # Environment sky lighting
│   ├── ft_precompute.c      # Shape precomputation / caching
│   ├── ft_parsing.c / ft_parsing_2.c      # Scene file (.rt) parser
│   ├── ft_check_parsing.c / ft_check_parsing_2.c  # Input validation
│   ├── ft_cam.c / ft_cam_move.c  # Camera init, rotation, FPS movement
│   ├── ft_event.c           # Keyboard/mouse input (continuous)
│   ├── ft_hud.c             # On-screen HUD (help, stats)
│   ├── ft_vectors.c / ft_vectors_2.c  # Vector math
│   ├── ft_scalar.c          # Scalar-vector operations
│   ├── ft_matrix.c          # Matrix-vector transforms
│   ├── ft_pt.c              # Point utilities
│   ├── ft_argb.c            # Color (ARGB) operations
│   ├── ft_random.c          # Random number generation
│   ├── ft_close.c           # Memory cleanup & deallocation
│   ├── ft_save.c            # BMP file export
│   ├── ft_error.c           # Error handling
│   └── ft_bzero_struct.c    # Struct zero-initialization
├── includes/
│   ├── mini_rt.h            # Main header (structs, enums, 200+ prototypes)
│   └── keybinds.h           # Platform-specific key mappings
├── tests/                   # Unit & regression test suite
│   ├── minirt_test.h        # Test framework macros (ASSERT_*, TEST, RUN_TEST)
│   ├── test_all.c           # Test runner entry point
│   ├── test_math.c          # Vector/scalar math tests
│   ├── test_camera.c        # Camera direction tests
│   ├── test_intersections.c # Ray-shape intersection tests
│   ├── test_lighting.c      # Lighting and shadow tests
│   ├── test_parsing.c       # Parsing validation tests
│   ├── test_benchmark.c     # Performance benchmarks
│   ├── test_stubs.c         # Test stub helpers
│   └── regression/          # Reference BMPs for regression testing
├── tools/
│   └── gen_scene.c          # Procedural scene generator (8 presets)
├── gallery/                 # Showcase scene files (Cornell box, reflections, etc.)
├── libft/                   # Custom 42 utility library
├── mlx/                     # minilibX (macOS)
├── mlx_linux/               # minilibX (Linux)
├── scenes/                  # 20 example .rt scene files
├── saves/                   # Rendered BMP output directory
├── skills/                  # Technical documentation
├── .github/workflows/       # CI/CD (GitHub Actions)
├── Makefile                 # Build configuration
├── backlog.md               # Project backlog (10 completed sections)
└── README.md                # Project documentation
```

## Architecture

### Rendering Pipeline (multi-threaded)

```
ft_aff() main render loop
  ├── Divide image into horizontal bands (one per thread)
  └── Each thread, for each pixel (i, j):
      └── ft_ray(i, j) — cast ray from camera through pixel
          ├── Compute ray origin + direction
          ├── Apply camera rotation matrix
          ├── ft_trace_ray() — find closest intersection
          │   ├── Traverse BVH acceleration structure
          │   └── Test candidate shapes, keep nearest hit
          ├── ft_shape_norm() — compute surface normal at hit
          └── ft_light() — compute lighting
              ├── Apply ambient light
              └── For each light source:
                  ├── Shadow ray (occlusion test via BVH)
                  ├── Diffuse (Phong/Blinn-Phong) shading
                  └── Specular highlights
          ├── Recursive reflection rays (up to MAX_REFLECT_DEPTH=4)
          ├── Refraction rays (Snell's law with IOR)
          ├── PBR shading (GGX/Cook-Torrance BRDF, Fresnel, metallic-roughness)
          ├── ACES filmic tone mapping + sRGB gamma
          ├── Texture / bump map sampling
          ├── Depth of field (aperture jitter, DOF_SAMPLES=16)
          └── Motion blur (temporal sampling, MOTION_BLUR_SAMPLES=8)
```

Progressive rendering is available: renders at 8x → 4x → 2x → 1x resolution for interactive preview.

Path tracing mode (`--pathtrace N`) features Russian Roulette termination, environment sky lighting, emissive materials, and PBR-aware importance sampling.

### Supported Shapes (14 types)

#### Basic shapes

| Shape    | ID   | Key Properties                        |
|----------|------|---------------------------------------|
| Sphere   | `sp` | Center + diameter                     |
| Plane    | `pl` | Point + normal orientation            |
| Square   | `sq` | Center + normal + side length         |
| Cylinder | `cy` | Center + axis + diameter + height     |
| Triangle | `tr` | Three vertices                        |

#### Extended shapes

| Shape       | ID   | Key Properties                        |
|-------------|------|---------------------------------------|
| Cone        | `co` | Center + axis + diameter + height     |
| Disk        | `dk` | Center + normal + diameter            |
| Torus       | `to` | Center + axis + major/minor radius    |
| Ellipsoid   | `el` | Center + radii (x, y, z)             |
| Box         | `bx` | Center + dimensions (w, h, d)        |

#### Quadric surfaces

| Shape        | ID   | Key Properties                       |
|--------------|------|--------------------------------------|
| Hyperboloid  | `hy` | Center + axis + radii + height       |
| Paraboloid   | `pa` | Center + axis + scale + height       |

#### Composites

| Shape     | ID    | Key Properties                       |
|-----------|-------|--------------------------------------|
| CSG       | `csg` | Operation + two shape indices        |
| OBJ Mesh  | `me`  | File path + offset + color           |

### Scene File Format (.rt)

```
R   1920  1080                              # Resolution
A   0.2   255,255,255                       # Ambient light
c   0,0,0   0,0,-1   70                    # Camera: pos, orient, fov
l   0,10,0  0.7   255,255,255              # Light: pos, intensity, color
sp  0,0,-5  2.0   255,0,0                  # Sphere: pos, diameter, color
pl  0,-1,0  0,1,0   200,200,200            # Plane: pos, normal, color
cy  2,0,-5  0,1,0   1.0  3.0  0,255,0     # Cylinder: pos, axis, d, h, color
tr  -1,0,-3  1,0,-3  0,2,-3  0,0,255      # Triangle: 3 vertices, color
co  0,0,-3  0,1,0   2.0  4.0  30,255,30   # Cone: pos, axis, d, h, color
dk  0,2,-5  0,0,1   4.0  255,255,30       # Disk: pos, normal, diameter, color
to  0,5,-8  0,1,0   2.5  0.7  255,30,255  # Torus: pos, axis, R, r, color
el  4,1.5,-3  2.0,1.2,1.2  30,80,255      # Ellipsoid: pos, radii, color
bx  8,1.5,-3  3,3,3  255,140,0            # Box: pos, dimensions, color
hy  -5,2,-4  0,1,0  1.5,2.0,0  6.0  255,80,30   # Hyperboloid
pa  0,0,-4  0,1,0  0.8  5.0  30,200,255          # Paraboloid
csg 2 0 1                                  # CSG: op, shape_a, shape_b
me  model.obj  0,0,0  200,200,200          # Mesh: file, offset, color
```

Material properties can be appended to any shape line as `key:value` pairs:

```
sp  0,0,-5  2.0  255,0,0  spec:0.8 refl:0.3          # Specular + reflective sphere
sp  2,0,-5  1.5  255,255,255  trans:0.9 ior:1.5        # Glass sphere
```

| Property | Range | Effect |
|----------|-------|--------|
| `spec:`  | 0–1   | Specular highlight intensity |
| `refl:`  | 0–1   | Reflectivity (recursive, max depth 4) |
| `trans:` | 0–1   | Transparency |
| `ior:`   | >0    | Index of refraction (used with `trans`) |
| `metal:` | 0–1   | Metallic factor (PBR metallic-roughness workflow) |
| `rough:` | 0–1   | Surface roughness (0 = mirror, 1 = matte) |
| `emit:`  | R,G,B | Emissive light (HDR values allowed, e.g. `emit:500,400,300`) |

### Memory Management

- Manual `malloc`/`free` with linked lists for shapes, cameras, and lights
- Cleanup in `ft_close.c`: traverses and frees all linked lists
- `ft_calloc()` used for zero-initialized allocations
- `get_next_line()` handles line buffer allocation during parsing

### Data Structures (defined in `includes/mini_rt.h`)

- `e_shape_id` — enum for 14 shape types (sphere, plane, square, cylinder, triangle, cone, disk, torus, ellipsoid, box, hyperboloid, paraboloid, CSG, mesh)
- `e_csg_op` — enum for CSG operations (union, intersect, difference)
- `t_pt` — 3D point/vector (x, y, z)
- `t_ray` — ray with origin, direction, length, hit normal, motion time
- `t_mat` — 3x3 rotation matrix
- `t_argb` — color (a, r, g, b channels)
- `t_texture` — texture with mlx image pointer
- `t_material` — material properties (reflectivity, transparency, IOR, specular, shininess, texture, bump map)
- `t_shape` — linked list node for any geometric object (includes material)
- `t_cam` — linked list node for cameras (position, orientation, rotation matrix, aperture, focal distance)
- `t_light` — linked list node for light sources
- `t_bvh_node` — BVH acceleration tree node (AABB bounds, left/right children)
- `t_thread_data` — per-thread rendering context (scanline range, shared scene pointer)
- `t_window` — window/image context (mlx pointers, resolution, HUD state, key bitfield)
- `t_rt` — root struct containing all scene data

## Implementation Details

See `skills/` for in-depth technical documentation:

- **[skills/camera-and-rendering.md](skills/camera-and-rendering.md)** — Camera orientation system (Euler angles in PI-radians, not direction vectors), pixel coordinate conventions, Y-axis mapping, BMP save workflow, headless rendering with Xvfb, scene authoring guidelines, material properties, and shape IDs.

## Known Issues & Limitations

- **Epsilon values:** Named constants (EPSILON, EPSILON_SHADOW, EPSILON_TIGHT) are defined, but some magic numbers may remain in intersection math

## Testing

### Unit Tests (`make test`)

Custom test framework (`tests/minirt_test.h`) with macros: `TEST()`, `RUN_TEST()`, `ASSERT_DBL_EQ`, `ASSERT_PT_EQ`, `ASSERT_ARGB_EQ`, `ASSERT_TRUE`.

Test suites:
- **test_math.c** — vector/scalar operations (sqr, create, init, swap, inverse)
- **test_camera.c** — camera direction computation (forward, right vectors at various yaw angles)
- **test_intersections.c** — ray-shape intersection accuracy (sphere, plane, triangle, square)
- **test_lighting.c** — light list counting, shadow ray validation
- **test_parsing.c** — shape ID detection (all 14 types), color validation

### Regression Tests (`make regression`)

Renders scenes and compares pixel-by-pixel against reference BMPs in `tests/regression/`. Catches visual regressions from code changes.

### Performance Benchmarks (`make benchmark`)

Measures render times for standardized scenes.

### Coverage (`make coverage`)

Runs tests with gcov and generates lcov HTML reports.

### CI/CD (`.github/workflows/ci.yml`)

GitHub Actions pipeline: build validation, unit tests, regression tests, clang-format linting, coverage reporting.

### External Tester

[invalid_rt_file_tester](https://github.com/lienardale/invalid_rt_file_tester) — 198 invalid .rt file test cases for parsing validation.

### Scene Files

`scenes/` directory contains 19 test scenes covering basic shapes, extended shapes, quadrics, lighting, interior views, DOF, motion blur, and path tracing.
