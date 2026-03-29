# miniRT Backlog

## 1. Performances: Enhance & Guarantee Safe Memory Usage

### Memory Safety
- [x] Replace VLA stack allocation in `ft_save.c` (`unsigned char tab[3 * win->x * win->y]`) with heap allocation (`malloc`) to prevent stack overflow on large resolutions
- [x] Audit all `malloc`/`ft_calloc` calls for missing NULL return checks (all 3 calls already properly checked)
- [x] Add proper cleanup on all error paths in parsing (some `ft_error()` paths may leak)
- [x] Run Valgrind/AddressSanitizer analysis and fix all reported leaks and invalid accesses
- [x] Remove leftover debug `printf` in `ft_mouse()` (ft_event.c) and `ft_check_resol()` (ft_check_parsing.c)

### Performance Optimization
- [x] Implement bounding volume hierarchy (BVH) or spatial partitioning to avoid testing every shape per ray
- [x] Add early termination in `ft_shadow()` when a shadow ray hits any object closer than the light
- [x] Precompute and cache shape invariants (e.g., plane d-value, triangle edges/normals, sphere radius², cylinder axis) at parse time instead of per-ray
- [x] Optimize math operations: reduce redundant dot products and vector computations in lighting (single light-distance computation, avoid re-normalizing already-normalized vectors)
- [x] Add SSE2 SIMD intrinsics for vector math operations (`ft_vectors.c`, `ft_vectors_2.c`, `ft_scalar.c`)
- [x] Optimize the resolution toggle ('r' key) to use progressive rendering (low-res preview then full-res refinement)

---

## 2. Unit Tests, Coverage & Pipeline Jobs

### Unit Test Framework
- [x] Choose and integrate a C testing framework (custom lightweight header in `tests/minirt_test.h`)
- [x] Write unit tests for vector math functions (`ft_vectors.c`, `ft_vectors_2.c`, `ft_scalar.c`, `ft_matrix.c`)
- [x] Write unit tests for color operations (`ft_argb.c`)
- [x] Write unit tests for point utilities (`ft_pt.c`)
- [x] Write unit tests for ray-shape intersections (sphere, plane, square, cylinder, triangle)
- [x] Write unit tests for lighting calculations (`ft_light.c`) — tested via intersection dispatch with `ft_which_shape`
- [x] Write unit tests for scene file parsing and validation (`ft_parsing.c`, `ft_check_parsing.c`)
- [x] Add regression tests using reference BMP output (render known scenes and compare pixel-by-pixel)

### Code Coverage
- [x] Configure `gcov`/`lcov` for coverage reporting
- [x] Set a minimum coverage threshold (50%+ enforced in CI, target: 80%+)
- [x] Add coverage report generation to Makefile (`make coverage`)

### CI/CD Pipeline
- [x] Create GitHub Actions workflow for build validation on push/PR
- [x] Add automated test execution in CI
- [x] Add coverage reporting step (upload as artifact)
- [x] Add static analysis step (cppcheck)
- [x] Add Valgrind/leak-check step in CI
- [x] Add linting/formatting validation (clang-format with `.clang-format` config)

---

## 3. Implement Multithreading

- [x] Evaluate threading approaches: pthreads vs OpenMP for pixel-level parallelism
- [x] Partition the image into horizontal scanline bands (or tiles) for parallel rendering
- [x] Implement thread pool to avoid per-frame thread creation overhead
- [x] Ensure thread safety: scene data is read-only during rendering (no shared mutable state)
- [x] Handle pixel buffer write synchronization (each thread writes to its own region)
- [x] Add a `--threads N` CLI option or auto-detect available CPU cores
- [x] Benchmark single-threaded vs multi-threaded rendering and document speedup
- [x] Consider GPU acceleration (OpenCL/Vulkan compute) as a future stretch goal

---

## 4. Add More Shapes Handling

### New Primitives
- [x] **Cone** — similar to cylinder but with tapering radius
- [x] **Disk** — circular planar shape (useful for cylinder caps)
- [x] **Torus** — donut shape (quartic intersection equation)
- [x] **Ellipsoid** — generalized sphere with non-uniform scaling
- [x] **Box/Cube** — axis-aligned or oriented bounding box (ray-AABB intersection)

### Composite & Advanced Shapes
- [x] **Mesh support** — load triangle meshes from OBJ files
- [x] **Constructive Solid Geometry (CSG)** — union, intersection, difference of shapes
- [x] **Quadric surfaces** — generalized second-degree surfaces (hyperboloid, paraboloid)

### Shape Enhancements
- [x] Add proper cylinder caps (disk intersection at top and bottom)
- [x] Support per-shape material properties (reflectivity, transparency, refractive index)
- [x] Add texture mapping (UV coordinates per shape type)
- [x] Add normal mapping / bump mapping for surface detail

---

## 5. Generate Configuration for Complex and Pretty Renders

### Scene Generator Tool
- [x] Build a CLI scene generator (`gen_scene`) that outputs valid `.rt` files
- [x] Support procedural placement: grids, spirals, random scatter with seed
- [x] Include preset scenes: Cornell box, spheres on checkerboard, solar system, etc.
- [x] Add material/color palettes for visually appealing outputs
- [x] Support parametric generation (number of objects, scene bounds, randomness)

### Enhanced Rendering Features (prerequisites for pretty output)
- [x] Implement specular highlights (complete Phong or Blinn-Phong model)
- [x] Add reflection rays (recursive ray tracing, configurable depth)
- [x] Add refraction/transparency (Snell's law, Fresnel equations)
- [x] Implement soft shadows (area lights with multiple sample rays)
- [x] Add anti-aliasing (supersampling or stochastic sampling per pixel)
- [x] Implement gamma correction for accurate color output

### Gallery & Showcase
- [x] Create a `gallery/` directory with curated high-quality scene files
- [x] Render reference images at high resolution for each gallery scene
- [x] Document each scene's features and rendering time

---

## 6. Fix Multi-Threaded Rendering Data Race

The rendering pipeline was analyzed and found to be mostly thread-safe already: normals are stored on stack-local `t_ray.hit_n`, colors are read from shapes without mutation, and each thread operates on its own ray. The only data race was dead `sh->in = 0` writes in intersection functions (`ft_intersect_ray_sphere`, `ft_cylinder_calc_one`) that mutated shared `t_shape` fields from multiple threads. The fix removed these writes, removed the unused `in` field from `t_shape`, and updated regression tests to run with the default thread count.

- [x] Refactor `ft_trace_ray_recursive()` to use stack-local `t_argb` color and `t_pt` normal instead of writing to shared `t_shape` fields
- [x] Thread the local color/normal through `ft_pre_light()`, `ft_albedo()`, `ft_apply_reflection()`, and `ft_apply_refraction()`
- [x] Remove the save/restore workaround in `src/ft_ray.c`
- [x] Verify multi-threaded determinism: same BMP output regardless of thread count
- [x] Update regression tests to run with default thread count (remove `--threads 1` constraint)

---

## 7. Code Quality & Refactoring

- [x] Add inline documentation / comments to all functions (especially math-heavy ones)
- [x] Replace hardcoded epsilon values (0.0001, 0.001) with named constants
- [x] Replace magic shape IDs ('s', 'p', 'q', 'y', 't') with an enum
- [x] Refactor nested ternary chains in `ft_parsing.c` into readable if/else blocks
- [x] Consolidate duplicated cylinder code (`ft_cylinder.c` + `ft_cylinder_2.c`)
- [x] Add proper error messages with context (file, line, what went wrong)
- [x] Separate parsing logic from shape initialization (single responsibility)

---

## 8. Cross-Platform & Build Improvements

- [x] Add CMake as an alternative build system for better IDE integration and dependency management
- [x] Fix compiler warnings with stricter flags (`-Wpedantic`, `-Wshadow`, `-Wconversion`)
- [x] Add a `make debug` target with sanitizers enabled (`-fsanitize=address,undefined`)
- [x] Add a `make release` target with full optimizations (`-O3 -DNDEBUG`)
- [x] Test and validate on multiple Linux distributions
- [x] Consider replacing minilibX with SDL2 or similar for broader platform support

---

## 9. User Experience Improvements

- [x] Add a loading/progress indicator during rendering (render timing measured per frame, displayed in stats overlay)
- [x] Implement real-time preview at low resolution, then progressive refinement (8x → 4x → 2x → 1x multi-stage pipeline in `ft_frame_update`)
- [x] Add mouse-based camera controls (left-click-drag to rotate view, scroll wheel to zoom in/out)
- [x] Display scene statistics on screen (press 'I': resolution, object/light count, render time, threads, camera position)
- [x] Add command-line flags for resolution override (`--width W --height H`), output path (`--output FILE`), verbose mode (`--verbose` / `-v`)
- [x] Support window resizing with automatic re-render (press 1/2/3/4 for preset resolutions: 320×240, 640×480, 1280×720, scene default — recreates window and re-renders)
- [x] Add a help overlay (press 'H' to show all controls including new mouse, stats, and resolution features)

---

## 10. Advanced Rendering Features

- [x] Implement a full Phong/Blinn-Phong lighting model (ambient + diffuse + specular)
- [x] Add recursive reflections with configurable max depth
- [x] Implement refractive transparency (glass, water effects)
- [x] Add texture mapping (image textures, procedural textures like checkerboard/noise)
- [x] Implement normal/bump mapping
- [x] Add depth of field (aperture simulation)
- [x] Add motion blur support
- [x] Implement path tracing for global illumination (Monte Carlo integration)

---

## Priority Suggestions

| Priority | Item | Rationale |
|----------|------|-----------|
| P0 | Memory safety fixes | Prevent crashes and undefined behavior |
| P0 | Unit tests + CI | Foundation for safe future changes |
| P1 | Multithreading | Major user-visible performance improvement |
| P1 | Code quality refactoring | Makes all other work easier |
| P2 | More shapes | Expands rendering capabilities |
| P2 | Specular + reflections | Prerequisite for pretty renders |
| P3 | Scene generator | Showcases the engine's capabilities |
| P3 | Advanced rendering | Long-term vision features |

---

## 11. RT Subject — Mandatory Compliance

Items required by the RT subject (v4.1) mandatory part that are not yet fully implemented.

### Expose Event Handling
- [ ] Add `mlx_hook(win, 12, ...)` expose event handler in `ft_mini_rt.c` to redraw the cached image buffer on window expose without re-rendering the scene
- [ ] Ensure `mlx_put_image_to_window` is called on expose to restore the display after window occlusion/resize

### Per-Shape Rotation Transforms
- [ ] Add a `t_mat rot` rotation matrix field to `t_shape` struct in `includes/mini_rt.h`
- [ ] Parse optional `rot:rx,ry,rz` Euler angle property on any shape line in `ft_parsing.c`
- [ ] Build rotation matrix from Euler angles at parse time in `ft_precompute.c`
- [ ] Transform ray to object-local space (apply inverse rotation) before intersection testing in `ft_ray.c`
- [ ] Transform hit normal back to world space after intersection
- [ ] Add unit tests for rotated shape intersections in `tests/test_intersections.c`

---

## 12. Additional Light Types

### Directional (Parallel) Light
- [ ] Add `e_light_type` enum with `LIGHT_POINT` and `LIGHT_DIRECTIONAL` values in `includes/mini_rt.h`
- [ ] Add `type` and `direction` fields to `t_light` struct
- [ ] Implement new scene directive `dl ox,oy,oz ratio R,G,B` for directional lights in `ft_parsing.c`
- [ ] Add parsing validation for directional light in `ft_check_parsing.c`
- [ ] Update `ft_light.c` shadow computation: directional lights cast parallel shadow rays (infinite distance, fixed direction)
- [ ] Update `ft_light.c` illumination: no distance attenuation for directional lights
- [ ] Add unit tests for directional light shadows and illumination in `tests/test_lighting.c`
- [ ] Create example scene `scenes/directional_light.rt` demonstrating parallel light

---

## 13. Additional Shape Types

### Parallelogram
- [ ] Add shape ID `pg` to `e_shape_id` enum
- [ ] Implement `ft_parallelogram.c`: intersection via plane test + edge-vector bounds check (corner + two edge vectors)
- [ ] Parse `pg corner edge1 edge2 R,G,B` in `ft_parsing.c`
- [ ] Compute AABB for BVH integration in `ft_precompute.c`
- [ ] Add to Makefile and `includes/mini_rt.h`

### Half-Sphere
- [ ] Add shape ID `hs` to `e_shape_id` enum
- [ ] Implement `ft_halfsphere.c`: sphere intersection + half-space clipping against orientation normal
- [ ] Handle normal at the equatorial disk cap (flat normal)
- [ ] Parse `hs center diameter orient R,G,B` in `ft_parsing.c`
- [ ] Add to Makefile and `includes/mini_rt.h`

### Tube (Open Cylinder)
- [ ] Add shape ID `tu` to `e_shape_id` enum
- [ ] Implement `ft_tube.c`: reuse cylinder body intersection logic, skip cap intersection tests
- [ ] Parse `tu center axis diameter height R,G,B` in `ft_parsing.c`
- [ ] Add to Makefile and `includes/mini_rt.h`

### Pyramid
- [ ] Add shape ID `py` to `e_shape_id` enum
- [ ] Implement `ft_pyramid.c`: decompose into square base + 4 triangular faces at parse time
- [ ] Parse `py center axis width height R,G,B` in `ft_parsing.c`
- [ ] Store as triangle list linked to parent shape for BVH
- [ ] Add to Makefile and `includes/mini_rt.h`

### Tetrahedron
- [ ] Add shape ID `th` to `e_shape_id` enum
- [ ] Implement `ft_tetrahedron.c`: decompose 4 vertices into 4 triangular faces at parse time
- [ ] Parse `th v1 v2 v3 v4 R,G,B` in `ft_parsing.c`
- [ ] Add to Makefile and `includes/mini_rt.h`

### Tablecloth (Parametric Wave Surface)
- [ ] Add shape ID `tc` to `e_shape_id` enum
- [ ] Implement `ft_tablecloth.c`: generate triangle mesh from `y = A * sin(fx*x) * sin(fz*z)` parametric surface
- [ ] Parse `tc center amplitude frequency extent R,G,B` in `ft_parsing.c`
- [ ] Tessellate into triangle grid at parse time, integrate into BVH
- [ ] Add to Makefile and `includes/mini_rt.h`

---

## 14. Procedural Textures & Colour Disruption

### Procedural Textures
- [ ] Add `e_proc_type` enum: `PROC_NONE`, `PROC_CHECKER`, `PROC_STRIPE`, `PROC_NOISE`
- [ ] Add `proc_type` field to `t_material` struct
- [ ] Parse `proc:checker`, `proc:stripe`, `proc:noise` material property in `ft_parsing.c`
- [ ] Implement checkerboard pattern in `ft_texture.c`: alternate colors based on UV floor parity
- [ ] Implement stripe pattern: alternate based on single UV axis
- [ ] Implement Perlin noise: gradient noise for organic patterns
- [ ] Apply procedural color as albedo modulation in lighting pipeline

### Texture-Driven Material Maps
- [ ] Add `refl_map` and `trans_map` fields (texture pointers) to `t_material` struct
- [ ] Parse `refl_map:file.xpm` and `trans_map:file.xpm` material properties
- [ ] Sample reflection map at hit UV to modulate `refl` value per-pixel in `ft_ray.c`
- [ ] Sample transparency map at hit UV to modulate `trans` value per-pixel
- [ ] Support grayscale XPM maps (luminance → scalar value)

---

## 15. Transparent Shadows

- [ ] Modify shadow ray traversal in `ft_light.c`: instead of returning binary blocked/not-blocked, accumulate shadow attenuation
- [ ] When shadow ray hits a transparent object, multiply shadow factor by `(1 - transparency)` and tint by object color
- [ ] Continue shadow ray through transparent objects (iterate until fully opaque or reaching light)
- [ ] Apply accumulated attenuation and color tint to the light contribution
- [ ] Add test scene `scenes/transparent_shadows.rt` with colored glass casting tinted shadows

---

## 16. Live Scene Manipulation

### Object Selection
- [ ] Implement mouse-click ray-casting to select the nearest shape under cursor
- [ ] Store selected shape pointer in `t_window` struct
- [ ] Display selection highlight in HUD: shape type, index, position, material properties

### Object Translation
- [ ] Shift + W/A/S/D to translate selected object in camera-relative XZ plane
- [ ] Shift + PgUp/PgDn to translate along world Y axis
- [ ] Update shape position, rebuild BVH, and re-render at draft resolution

### Object Rotation
- [ ] Ctrl + Arrow keys to rotate selected object around its local axes
- [ ] Update shape rotation matrix and re-render at draft resolution

### Object Copy / Paste / Delete
- [ ] Ctrl+C to copy selected object (store deep clone)
- [ ] Ctrl+V to paste copied object at camera target position
- [ ] Delete/Backspace to remove selected object from scene
- [ ] Rebuild BVH and re-render after add/remove operations

### Real-Time Preview
- [ ] After each manipulation, trigger progressive re-render (8x → full) for responsive feedback
- [ ] Debounce rapid input to avoid excessive re-renders

### Files
- `src/ft_event.c` — input handling for selection/translation/rotation
- `src/ft_hud.c` — selection indicator display
- `src/ft_ray.c` — ray-cast for mouse picking
- `src/ft_bvh.c` — BVH rebuild after object modification
- `includes/mini_rt.h` — selected shape pointer, new key bindings

---

## 17. Animation / Video Export

### Frame Sequence Rendering
- [ ] Add `--animate N` CLI flag to render N frames as numbered BMPs (`frame_0001.bmp`, `frame_0002.bmp`, ...)
- [ ] Output to `saves/` directory with frame numbering

### Camera Path Interpolation
- [ ] Support multiple `c` camera definitions as keyframes
- [ ] Linearly interpolate position, orientation, and FOV across frames between keyframes

### Object Animation
- [ ] Apply per-shape `vel:` vectors across frames (position += velocity * frame_dt)
- [ ] Support rotation velocity for animated spinning objects

### Files
- `src/ft_mini_rt.c` — CLI parsing for `--animate`
- `src/ft_save.c` — numbered frame output
- New `src/ft_animate.c` — interpolation and animation loop logic

---

## 18. Spotlight

*Inspired by Gpinchon/RT42 and David-GC/RT — both implement cone-shaped spotlights.*

- [ ] Add `LIGHT_SPOT` to `e_light_type` enum
- [ ] Add `direction`, `spot_angle` (cone half-angle), and `spot_falloff` fields to `t_light`
- [ ] Implement new scene directive `sl pos dir angle ratio R,G,B` in `ft_parsing.c`
- [ ] In `ft_light.c`, compute cone attenuation: `cos(angle_to_light_axis)` vs `cos(spot_angle)`, smooth falloff at edges
- [ ] Spotlights cast shadows only within their cone
- [ ] Add example scene `scenes/spotlight.rt` with dramatic spotlight pool effect
- [ ] Files: `includes/mini_rt.h`, `src/ft_light.c`, `src/ft_parsing.c`, `src/ft_check_parsing.c`

---

## 19. Post-Processing Effects

*Inspired by Gpinchon/RT42 (bloom, SSAO, gamma) and Matt-Hurd/42-RT (sepia, smoothing).*

### Bloom (HDR Glow)
- [ ] Add HDR float color buffer alongside the existing 8-bit framebuffer
- [ ] After rendering, extract pixels exceeding a brightness threshold
- [ ] Apply Gaussian blur to bright pixels and composite back (additive blend)
- [ ] Parse `bloom:threshold,intensity,radius` as scene-level setting
- [ ] Files: new `src/ft_postprocess.c`, `includes/mini_rt.h`

### Screen-Space Ambient Occlusion (SSAO)
- [ ] Store depth buffer during rendering (distance from camera per pixel)
- [ ] Sample Poisson disc pattern around each pixel, compare depths for occlusion factor
- [ ] Darken corners/crevices where geometry is close together
- [ ] Parse `ssao:radius,samples` as scene-level setting
- [ ] Files: new `src/ft_ssao.c`, `includes/mini_rt.h`

### Visual Filters
- [ ] Add post-processing filter toggle (key: F or number keys)
- [ ] Implement sepia tone, grayscale, inverted colors, edge detection (Sobel), cel-shading outline
- [ ] Apply as screen-space pass after rendering, before display
- [ ] Files: `src/ft_postprocess.c`, `src/ft_event.c`

---

## 20. Skybox / Environment Map

*Inspired by David-GC/RT — skybox with cubemap textures for environment reflections.*

- [ ] Load 6 XPM/PNG face images as a cubemap (or 1 equirectangular HDR map)
- [ ] Parse `skybox front.xpm back.xpm left.xpm right.xpm top.xpm bottom.xpm` scene directive
- [ ] When a ray misses all objects, sample the skybox instead of the flat sky gradient
- [ ] Use skybox for environment reflections on reflective surfaces
- [ ] Replace current `ft_env.c` sky gradient as fallback when no skybox is defined
- [ ] Files: new `src/ft_skybox.c`, `src/ft_env.c`, `src/ft_ray.c`, `includes/mini_rt.h`

---

## 21. Scene Export / Save Modified Scene

*Inspired by David-GC/RT (scene saving) and Chr0nos/rt (format conversion).*

- [ ] Add `--export FILE` CLI flag to write current scene state to a `.rt` file
- [ ] Serialize all cameras, lights, shapes (with materials) back to `.rt` format
- [ ] Bind to a key (e.g., Ctrl+S) for saving after live manipulation (§16)
- [ ] Include comments with generation metadata (date, camera position)
- [ ] Files: new `src/ft_export.c`, `src/ft_event.c`, `includes/mini_rt.h`

---

## 22. Additional PBR Texture Maps

*Inspired by Gpinchon/RT42 — full PBR texture map pipeline (roughness, metallic, AO, normal, height).*

- [ ] Add `rough_map`, `metal_map`, `ao_map` texture fields to `t_material`
- [ ] Parse `rough_map:file.xpm`, `metal_map:file.xpm`, `ao_map:file.xpm` material properties
- [ ] Sample roughness map to modulate PBR roughness per-pixel in `ft_pbr.c`
- [ ] Sample metallic map to modulate metallic factor per-pixel
- [ ] Sample AO map to darken ambient contribution in crevices
- [ ] Files: `includes/mini_rt.h`, `src/ft_pbr.c`, `src/ft_material.c`, `src/ft_parsing.c`

---

## 23. Cel-Shading / Toon Rendering

*Inspired by David-GC/RT — cartoon-style rendering with quantized lighting and outlines.*

- [ ] Add `cel:N` material property to quantize diffuse lighting into N bands
- [ ] Implement edge detection (Sobel on normal/depth buffer) for black outlines
- [ ] Toggle cel-shading as a render mode or per-material property
- [ ] Files: `src/ft_light.c`, `src/ft_postprocess.c`, `includes/mini_rt.h`

---

## 24. Showcase Scene Collection

*Inspired by all 4 reference implementations — themed scenes to demonstrate features at defence.*

### Mandatory Demonstration Scenes (reproduce subject figures)
- [ ] `scenes/four_basics.rt` — The 4 basic objects (sphere, plane, cylinder, cone) with 2 spots, shadows, and shine (Figure VI.1)
- [ ] `scenes/four_basics_alt.rt` — Same scene from another viewpoint (Figure VI.2)
- [ ] `scenes/shadow_mixing.rt` — Multiple lights casting overlapping shadows (Figure VI.3)

### Feature Showcase Scenes
- [ ] `scenes/glass_refraction.rt` — Glass spheres/cubes with IOR, Fresnel, colored transparent shadows
- [ ] `scenes/mirror_room.rt` — Enclosed room with reflective walls showing recursive reflections
- [ ] `scenes/materials_showcase.rt` — Grid of spheres: matte, glossy, metallic, glass, emissive, textured
- [ ] `scenes/procedural_textures.rt` — Checkerboard floor, noise-textured walls, striped objects
- [ ] `scenes/all_shapes.rt` — One of every shape type arranged for visibility
- [ ] `scenes/spotlight_drama.rt` — Dark scene with spotlights creating dramatic pools of light
- [ ] `scenes/skybox_outdoor.rt` — Reflective objects in a skybox environment

### Themed / Artistic Scenes
- [ ] `scenes/christmas_tree.rt` — Tree made of cones + sphere ornaments + star (inspired by Chr0nos/rt arbre_de_noel)
- [ ] `scenes/solar_system.rt` — Sun (emissive) + planets with textures (inspired by David-GC/RT)
- [ ] `scenes/chess_board.rt` — Chess pieces on checkerboard (inspired by Chr0nos/rt echiquier)
- [ ] `scenes/stained_glass.rt` — Transparent colored panels casting tinted shadows (inspired by Gpinchon/RT42 vitrail)
- [ ] `scenes/underwater.rt` — Blue-tinted lighting, caustic-like patterns, fish-like shapes

---

## Priority Suggestions (Updated)

| Priority | Item | Rationale |
|----------|------|-----------|
| P0 | Expose event + shape rotations (§11) | Mandatory part compliance |
| P1 | Directional light (§12) | High-value option, straightforward |
| P1 | Spotlight (§18) | High-value, implemented by most reference projects |
| P1 | Transparent shadows (§15) | High-value option, single-file change |
| P1 | Procedural textures (§14) | Subject explicitly lists "colour disruption" |
| P1 | Mandatory demo scenes (§24) | Required for defence — reproduce subject figures |
| P2 | New shapes: parallelogram, half-sphere, tube (§13) | Listed as "limited objects" options |
| P2 | Composed shapes: pyramid, tetrahedron (§13) | Listed as "composed elements" options |
| P2 | Live scene manipulation (§16) | Subject requires "live manipulation tools" |
| P2 | Scene export/save (§21) | Pairs with live manipulation for defence |
| P2 | Skybox / environment map (§20) | Big visual impact, used by reference impls |
| P2 | Post-processing: bloom + filters (§19) | Visual polish, easy to demo |
| P2 | Texture-driven material maps (§14) | Listed option: limit properties by texture |
| P2 | Showcase scenes (§24) | Defence preparation, demonstrates all features |
| P3 | SSAO (§19) | Advanced screen-space effect |
| P3 | PBR texture maps (§22) | Already have PBR shading, maps add realism |
| P3 | Cel-shading (§23) | Unique visual style, low effort |
| P3 | Tablecloth surface (§13) | Exotic parametric surface |
| P3 | Animation / video export (§17) | Listed option: "video clips from images" |
