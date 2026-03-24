# miniRT

![Screenshot 1](./saves/cylinder.bmp?raw=true)

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
- Total 2253 lines
  - C : 1933 lines, 57.7%
  - headers : 982 lines, 14.9%
- Git
  - 1 branche
  - 0 pull requests
  - 53 commits   

### Tester :
- 198 invalid configurations
- Shell : 361 lines

## Stack :  

<a href="https://www.cprogramming.com/" target="_blank" rel="noreferrer"> 
	<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg" alt="c" width="40" height="40"/> 
</a> 

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
| Next camera | Space | Cycle through scene cameras |
| Exit | ESC | Close window |

## Features

- 5 simple geometric objects (plane, sphere, cylinder, square, triangle)
- Extended shapes: cone, disk, torus, ellipsoid, box, hyperboloid, paraboloid
- Constructive Solid Geometry (CSG: union, intersect, difference)
- OBJ mesh loading
- Multiple light spots (intensity, shadows, ambient light)
- Multiple cameras (cycle with Space)
- Phong specular highlights, reflections, transparency, refraction
- Texture mapping and bump mapping
- BVH acceleration structure
- Multi-threaded rendering
- FPS-style camera controls (WASD relative to camera orientation)
- BMP file export with `-save` flag

## Screenshots :

![Screenshot 2](./saves/example.bmp?raw=true)
![Screenshot 3](./saves/lights.bmp?raw=true)
![Screenshot 4](./saves/objects.bmp?raw=true)
![Screenshot 5](./saves/triangle.bmp?raw=true)

## Gallery :

Scenes showcasing enhanced rendering features (specular highlights, soft shadows, reflections, refraction, anti-aliasing, gamma correction).

| Cornell Box | Reflections | Glass Spheres |
|:-----------:|:-----------:|:-------------:|
| ![Cornell Box](./saves/cornell_box.bmp?raw=true) | ![Reflections](./saves/reflections.bmp?raw=true) | ![Glass Spheres](./saves/glass_spheres.bmp?raw=true) |

| Solar System | Spiral | Mixed Shapes |
|:------------:|:------:|:------------:|
| ![Solar System](./saves/solar_system.bmp?raw=true) | ![Spiral](./saves/spiral.bmp?raw=true) | ![Mixed Shapes](./saves/mixed_shapes.bmp?raw=true) |

Scene files are in `gallery/`. Use `make gen_scene` to build the procedural scene generator.