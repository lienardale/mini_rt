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
- [ ] Consider SIMD intrinsics for vector math operations (`ft_vectors.c`, `ft_scalar.c`)
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

- [ ] Evaluate threading approaches: pthreads vs OpenMP for pixel-level parallelism
- [ ] Partition the image into horizontal scanline bands (or tiles) for parallel rendering
- [ ] Implement thread pool to avoid per-frame thread creation overhead
- [ ] Ensure thread safety: scene data is read-only during rendering (no shared mutable state)
- [ ] Handle pixel buffer write synchronization (each thread writes to its own region)
- [ ] Add a `--threads N` CLI option or auto-detect available CPU cores
- [ ] Benchmark single-threaded vs multi-threaded rendering and document speedup
- [ ] Consider GPU acceleration (OpenCL/Vulkan compute) as a future stretch goal

---

## 4. Add More Shapes Handling

### New Primitives
- [ ] **Cone** — similar to cylinder but with tapering radius
- [ ] **Disk** — circular planar shape (useful for cylinder caps)
- [ ] **Torus** — donut shape (quartic intersection equation)
- [ ] **Ellipsoid** — generalized sphere with non-uniform scaling
- [ ] **Box/Cube** — axis-aligned or oriented bounding box (ray-AABB intersection)

### Composite & Advanced Shapes
- [ ] **Mesh support** — load triangle meshes from OBJ/PLY files
- [ ] **Constructive Solid Geometry (CSG)** — union, intersection, difference of shapes
- [ ] **Quadric surfaces** — generalized second-degree surfaces (hyperboloid, paraboloid)

### Shape Enhancements
- [ ] Add proper cylinder caps (disk intersection at top and bottom)
- [ ] Support per-shape material properties (reflectivity, transparency, refractive index)
- [ ] Add texture mapping (UV coordinates per shape type)
- [ ] Add normal mapping / bump mapping for surface detail

---

## 5. Generate Configuration for Complex and Pretty Renders

### Scene Generator Tool
- [ ] Build a CLI scene generator (`gen_scene`) that outputs valid `.rt` files
- [ ] Support procedural placement: grids, spirals, random scatter with seed
- [ ] Include preset scenes: Cornell box, spheres on checkerboard, solar system, etc.
- [ ] Add material/color palettes for visually appealing outputs
- [ ] Support parametric generation (number of objects, scene bounds, randomness)

### Enhanced Rendering Features (prerequisites for pretty output)
- [ ] Implement specular highlights (complete Phong or Blinn-Phong model)
- [ ] Add reflection rays (recursive ray tracing, configurable depth)
- [ ] Add refraction/transparency (Snell's law, Fresnel equations)
- [ ] Implement soft shadows (area lights with multiple sample rays)
- [ ] Add anti-aliasing (supersampling or stochastic sampling per pixel)
- [ ] Implement gamma correction for accurate color output

### Gallery & Showcase
- [ ] Create a `gallery/` directory with curated high-quality scene files
- [ ] Render reference images at high resolution for each gallery scene
- [ ] Document each scene's features and rendering time

---

## 6. Code Quality & Refactoring

- [ ] Add inline documentation / comments to all functions (especially math-heavy ones)
- [ ] Replace hardcoded epsilon values (0.0001, 0.001) with named constants
- [ ] Replace magic shape IDs ('s', 'p', 'q', 'y', 't') with an enum
- [ ] Refactor nested ternary chains in `ft_parsing.c` into readable if/else blocks
- [ ] Consolidate duplicated cylinder code (`ft_cylinder.c` + `ft_cylinder_2.c`)
- [ ] Add proper error messages with context (file, line, what went wrong)
- [ ] Separate parsing logic from shape initialization (single responsibility)

---

## 7. Cross-Platform & Build Improvements

- [ ] Add CMake as an alternative build system for better IDE integration and dependency management
- [ ] Fix compiler warnings with stricter flags (`-Wpedantic`, `-Wshadow`, `-Wconversion`)
- [ ] Add a `make debug` target with sanitizers enabled (`-fsanitize=address,undefined`)
- [ ] Add a `make release` target with full optimizations (`-O3 -DNDEBUG`)
- [ ] Test and validate on multiple Linux distributions
- [ ] Consider replacing minilibX with SDL2 or similar for broader platform support

---

## 8. User Experience Improvements

- [ ] Add a loading/progress indicator during rendering (percentage or scanline counter)
- [ ] Implement real-time preview at low resolution, then progressive refinement
- [ ] Add mouse-based camera controls (click-drag to rotate, scroll to zoom)
- [ ] Display scene statistics on screen (object count, render time, FPS)
- [ ] Add command-line flags for resolution override, output format, verbose mode
- [ ] Support window resizing with automatic re-render
- [ ] Add a help overlay (press 'h' to show controls)

---

## 9. Advanced Rendering Features

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
