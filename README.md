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

## Features :

- clean window handling (no problem when resized, minimized or so on)

- 5 simple geometric objects (plan, sphere, cylinder, square, triangle)

- multiple light spots (intensity, shadows, ambiant light)

- multiple cameras (pov)

- objects intersection

- color and size of objects are defined in parsing and can be modified in the input file

- transformations (translation and rotation)

- "- save" option that saves the outpout into a .bmp file

- exiting wiht the 'esc' key or the red cross button

- changing the pov with the 'space' key

- low resolution mode with the 'r' key (pressing it again will make the resolution go back to high)

- mooving the pov forward, back, left and right with the 'w', 's', 'a' and 'd' keys

- mooving the pov up and down with the 'page up' and 'page down' keys

- rotating the pov up, down, left, right with the corresponding arrow keys

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