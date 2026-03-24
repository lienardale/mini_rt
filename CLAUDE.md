# CLAUDE.md - miniRT Project Guide

## Project Overview

**miniRT** is a basic ray tracing engine written in C, built as a 42 Paris common core project. It renders 3D scenes with realistic lighting, shadows, and multiple geometric objects using the minilibX graphics library.

- **Author:** alienard (42 Paris)
- **Language:** C (100%)
- **Lines of code:** ~2,270 (1,933 C + 982 headers)
- **Platform:** macOS (Darwin) and Linux

## Build

```bash
make        # Full build (libft + mlx + miniRT executable)
make clean  # Remove object files
make fclean # Remove objects + executable
make re     # Clean rebuild
```

- **Compiler:** `gcc -Wall -Wextra -Werror -g3 -ofast`
- **Output:** `miniRT` executable in project root
- **Platform auto-detected** via `uname -s`

### Dependencies

- **minilibX** (included in repo: `mlx/` for macOS, `mlx_linux/` for Linux)
- **libft** (included in repo: custom 42 utility library with printf, get_next_line)
- **System libs:** X11, Xext, m, z (Linux) / OpenGL, AppKit (macOS)

## Usage

```bash
./miniRT <scene_file.rt>        # Render scene in window
./miniRT <scene_file.rt> -save  # Render and save as BMP
```

### Controls

| Action         | Linux Key     |
|----------------|---------------|
| Move           | W/A/S/D       |
| Look around    | Arrow keys    |
| Move Up/Down   | PgUp/PgDn     |
| Rotate Z       | , / .         |
| FOV +/-        | Numpad +/-    |
| Toggle res     | R             |
| Next camera    | Space         |
| Exit           | ESC           |

## Project Structure

```
mini_rt/
├── src/                  # 25 C source files
│   ├── ft_mini_rt.c      # Entry point, init, main render loop
│   ├── ft_ray.c          # Core ray casting & intersection tracing
│   ├── ft_light.c        # Phong lighting, shadows, albedo
│   ├── ft_sphere.c       # Sphere intersection & normals
│   ├── ft_cylinder.c     # Cylinder intersection & normals
│   ├── ft_plane.c        # Plane intersection & normals
│   ├── ft_square.c       # Square intersection & bounds
│   ├── ft_triangle.c     # Triangle intersection (barycentric)
│   ├── ft_parsing.c      # Scene file (.rt) parser
│   ├── ft_check_parsing.c # Input validation
│   ├── ft_cam.c          # Camera init & rotation matrices
│   ├── ft_vectors.c      # Vector math (dot, normalize, etc.)
│   ├── ft_scalar.c       # Scalar-vector operations
│   ├── ft_matrix.c       # Matrix-vector transforms
│   ├── ft_event.c        # Keyboard/mouse input handling
│   ├── ft_close.c        # Memory cleanup & deallocation
│   ├── ft_save.c         # BMP file export
│   ├── ft_error.c        # Error handling
│   ├── ft_argb.c         # Color (ARGB) operations
│   └── ...               # Additional helper files
├── includes/
│   ├── mini_rt.h         # Main header (all structs & prototypes)
│   └── keybinds.h        # Platform-specific key mappings
├── libft/                # Custom 42 utility library
├── mlx/                  # minilibX (macOS)
├── mlx_linux/            # minilibX (Linux)
├── scenes/               # 11 example .rt scene files
├── saves/                # Rendered BMP output directory
├── Makefile              # Build configuration
└── README.md             # Project documentation
```

## Architecture

### Rendering Pipeline (single-threaded)

```
ft_aff() main render loop
  └── For each pixel (i, j):
      └── ft_ray(i, j) — cast ray from camera through pixel
          ├── Compute ray origin + direction
          ├── Apply camera rotation matrix
          ├── ft_trace_ray() — find closest intersection
          │   └── Test ALL shapes, keep nearest hit
          ├── ft_shape_norm() — compute surface normal at hit
          └── ft_light() — compute lighting
              ├── Apply ambient light
              └── For each light source:
                  ├── Shadow ray (occlusion test)
                  └── Diffuse (Phong) shading
```

### Supported Shapes

| Shape    | ID   | Key Properties                        |
|----------|------|---------------------------------------|
| Sphere   | `sp` | Center + diameter                     |
| Plane    | `pl` | Point + normal orientation            |
| Square   | `sq` | Center + normal + side length         |
| Cylinder | `cy` | Center + axis + diameter + height     |
| Triangle | `tr` | Three vertices                        |

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
```

### Memory Management

- Manual `malloc`/`free` with linked lists for shapes, cameras, and lights
- Cleanup in `ft_close.c`: traverses and frees all linked lists
- `ft_calloc()` used for zero-initialized allocations
- `get_next_line()` handles line buffer allocation during parsing

### Data Structures (defined in `includes/mini_rt.h`)

- `t_shape` — linked list node for any geometric object
- `t_cam` — linked list node for cameras (position, orientation, rotation matrix)
- `t_light` — linked list node for light sources
- `t_pt` — 3D point/vector (x, y, z)
- `t_argb` — color (a, r, g, b channels)
- `t_win` — window/image context (mlx pointers, resolution)
- `t_rt` — root struct containing all scene data

## Implementation Details

See `skills/` for in-depth technical documentation:

- **[skills/camera-and-rendering.md](skills/camera-and-rendering.md)** — Camera orientation system (Euler angles in PI-radians, not direction vectors), pixel coordinate conventions, Y-axis mapping, BMP save workflow, headless rendering with Xvfb, scene authoring guidelines, material properties, and shape IDs.

## Known Issues & Limitations

- **Hardcoded epsilon values:** Magic numbers (0.0001, 0.001) in intersection math
- **No inline documentation:** Code follows 42 norm (no comments)

## Testing

- **Manual testing:** Render scene files and visually inspect output
- **External tester:** [invalid_rt_file_tester](https://github.com/lienardale/invalid_rt_file_tester) — 198 invalid .rt file test cases
- **Scene files:** `scenes/` directory contains 11 test scenes
- **BMP output:** Use `-save` flag to generate reference images in `saves/`
