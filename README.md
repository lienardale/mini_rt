# miniRT

![Cathedral Ruins](./saves/cathedral_ruins.bmp?raw=true)

**A basic RayTracer using minilibX**    
project of 42 Paris's common-core

<a href="https://github.com/JaeSeoKim/badge42"><img src="https://badge42.vercel.app/api/v2/cl4cq9w7c004409k3asnbj82v/project/1633981" alt="alienard's 42 miniRT Score" /></a>

## Goals :	
- introduct us to the RayTracing algorithms
- to the implementation of mathematics formulas
- step up in terms of parsing and error handling

## Tester :
The configuration file had very precise restrictions, which lead me to implement my very first testing automation : [invalid_rt_file_tester](https://github.com/lienardale/invalid_rt_file_tester)   
It tests :
- invalid configurations (in 198 different ways)
- memory leaks for each of these configurations   

Feel free to use it, fork it, and report eventual issues.

## Code stats :

### Project :
- 1 contributor (solo project)
- 47 C source files, ~6,700 lines total (6,126 C + 586 headers)
- 14 supported shape types, multi-threaded rendering, BVH acceleration
- 19 test scenes, unit test suite, regression tests, CI/CD pipeline

### Tester :
- 198 invalid configurations
- Shell : 361 lines

## Stack :  

<a href="https://www.cprogramming.com/" target="_blank" rel="noreferrer"> 
	<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/> 
</a> 

## Build

```bash
make            # Full build (libft + mlx + miniRT executable)
make test       # Run unit tests
make regression # Run regression tests (compare against reference BMPs)
make lint       # Check code style with clang-format
make coverage   # Run tests with coverage reporting
make benchmark  # Performance benchmarking
```

## Usage

```bash
./miniRT <scene_file.rt>              # Render scene in window
./miniRT <scene_file.rt> -save        # Render and save as BMP
./miniRT <scene_file.rt> --threads N  # Use N render threads
./miniRT <scene_file.rt> --threads=auto  # Auto-detect thread count
```

Multiple options can be combined: `./miniRT scene.rt -save --threads 4`

## Controls

All movement and look keys support continuous hold for smooth motion.

| Action | Key | Description |
|---|---|---|
| Move forward/back | W / S | Relative to camera facing direction (XZ plane) |
| Strafe left/right | A / D | Perpendicular to camera facing (XZ plane) |
| Move up/down | PgUp / PgDn | World Y axis |
| Look left/right | Left / Right arrows | Hold for continuous yaw rotation |
| Look up/down | Up / Down arrows | Hold for continuous pitch rotation |
| Roll camera | , / . | Hold for continuous roll rotation |
| FOV increase/decrease | Numpad + / - | Adjust field of view (0-180) |
| Toggle resolution | R | Switch between draft (4x) and full resolution |
| Window resize | 1 / 2 / 3 / 4 | Set window scale |
| Help overlay | H | Toggle on-screen help text |
| Info/stats overlay | I | Toggle render stats (FPS, resolution) |
| Next camera | Space | Cycle through scene cameras |
| Exit | ESC | Close window |

## Supported Shapes

### Basic shapes

| ID | Shape | Parameters | Example |
|---|---|---|---|
| `sp` | Sphere | center, diameter, color | `sp 0,0,-5 2.0 255,0,0` |
| `pl` | Plane | point, normal, color | `pl 0,-1,0 0,1,0 200,200,200` |
| `sq` | Square | center, normal, side, color | `sq 0,0,-5 0,0,1 4.0 255,255,0` |
| `cy` | Cylinder | center, axis, diameter, height, color | `cy 0,0,-5 0,1,0 2.0 4.0 0,255,0` |
| `tr` | Triangle | vertex1, vertex2, vertex3, color | `tr -1,0,-3 1,0,-3 0,2,-3 0,0,255` |

### Extended shapes

| ID | Shape | Parameters | Example |
|---|---|---|---|
| `co` | Cone | center, axis, diameter, height, color | `co 0,0,-3 0,1,0 2.0 4.0 30,255,30` |
| `dk` | Disk | center, normal, diameter, color | `dk 0,2,-5 0,0,1 4.0 255,255,30` |
| `to` | Torus | center, axis, major radius, minor radius, color | `to 0,5,-8 0,1,0 2.5 0.7 255,30,255` |
| `el` | Ellipsoid | center, radii (x,y,z), color | `el 4,1.5,-3 2.0,1.2,1.2 30,80,255` |
| `bx` | Box | center, dimensions (w,h,d), color | `bx 8,1.5,-3 3,3,3 255,140,0` |

### Quadric surfaces

| ID | Shape | Parameters | Example |
|---|---|---|---|
| `hy` | Hyperboloid | center, axis, radii (a,b,c), height, color | `hy -5,2,-4 0,1,0 1.5,2.0,0 6.0 255,80,30` |
| `pa` | Paraboloid | center, axis, scale, height, color | `pa 0,0,-4 0,1,0 0.8 5.0 30,200,255` |

### Composites

| ID | Shape | Parameters | Example |
|---|---|---|---|
| `csg` | CSG operation | op (0=union, 1=intersect, 2=difference), shape_idx_a, shape_idx_b | `csg 2 0 1` |
| `me` | OBJ Mesh | file path, offset, color | `me model.obj 0,0,0 200,200,200` |

### Material properties

Material properties can be appended to any shape line as `key:value` pairs:

| Property | Range | Effect |
|---|---|---|
| `spec:` | 0–1 | Specular highlight intensity |
| `refl:` | 0–1 | Reflectivity (recursive, max depth 4) |
| `trans:` | 0–1 | Transparency |
| `ior:` | >0 | Index of refraction (used with `trans`) |

Example: `sp 0,0,-5 2.0 255,0,0 spec:0.8 refl:0.3`

## Features

- Multiple light spots (intensity, shadows, ambient light)
- Multiple cameras (cycle with Space)
- Phong/Blinn-Phong specular highlights, reflections, transparency, refraction
- Texture mapping and bump/normal mapping
- Depth of field (aperture simulation)
- Motion blur (temporal sampling)
- Path tracing / global illumination
- Anti-aliasing and gamma correction
- Progressive rendering (8x → 4x → 2x → 1x for interactive preview)
- Constructive Solid Geometry (CSG: union, intersect, difference)
- OBJ mesh loading
- BVH acceleration structure
- Multi-threaded rendering (`--threads N` or `--threads=auto`)
- FPS-style camera controls (WASD relative to camera orientation)
- On-screen HUD: help overlay (H) and render stats (I)
- BMP file export with `-save` flag

## Screenshots :

![Taurus](./saves/taurus.bmp?raw=true)

![Screenshot 2](./saves/example.bmp?raw=true)

![Screenshot 3](./saves/lights.bmp?raw=true)

![Screenshot 4](./saves/objects.bmp?raw=true)

![Screenshot 5](./saves/triangle.bmp?raw=true)

## Gallery :

Scenes showcasing enhanced rendering features (specular highlights, soft shadows, reflections, refraction, anti-aliasing, gamma correction).

### Cornell Box
![Cornell Box](./saves/cornell_box.bmp?raw=true)

### Reflections
![Reflections](./saves/reflections.bmp?raw=true)

### Glass Spheres
![Glass Spheres](./saves/glass_spheres.bmp?raw=true)

### Solar System
![Solar System](./saves/solar_system.bmp?raw=true)

### Spiral
![Spiral](./saves/spiral.bmp?raw=true)

### Mixed Shapes
![Mixed Shapes](./saves/mixed_shapes.bmp?raw=true)

Scene files are in `gallery/`. Use `make gen_scene` to build the procedural scene generator.