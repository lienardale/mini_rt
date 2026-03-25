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

- [ ] Add a loading/progress indicator during rendering (percentage or scanline counter)
- [ ] Implement real-time preview at low resolution, then progressive refinement
- [ ] Add mouse-based camera controls (click-drag to rotate, scroll to zoom)
- [ ] Display scene statistics on screen (object count, render time, FPS)
- [ ] Add command-line flags for resolution override, output format, verbose mode
- [ ] Support window resizing with automatic re-render
- [ ] Add a help overlay (press 'h' to show controls)

---

## 10. Advanced Rendering Features

- [ ] Implement a full Phong/Blinn-Phong lighting model (ambient + diffuse + specular)
- [ ] Add recursive reflections with configurable max depth
- [ ] Implement refractive transparency (glass, water effects)
- [ ] Add texture mapping (image textures, procedural textures like checkerboard/noise)
- [ ] Implement normal/bump mapping
- [ ] Add depth of field (aperture simulation)
- [ ] Add motion blur support
- [ ] Consider path tracing for global illumination (Monte Carlo integration)

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
