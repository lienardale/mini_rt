# SDL2 Migration Consideration

## Motivation

minilibX is a 42-school-specific library with several limitations:

- No package manager distribution (must be vendored)
- Limited documentation and community support
- X11-only on Linux (no Wayland support)
- No Windows support
- No hardware-accelerated blitting

## SDL2 Advantages

- Cross-platform: Linux, macOS, Windows, and WebAssembly
- Available via system package managers (`apt`, `brew`, `dnf`)
- Native Wayland support
- Hardware-accelerated rendering via `SDL_Renderer`
- Well-documented and actively maintained
- Rich ecosystem (`SDL_image`, `SDL_ttf`)

## mlx API Surface Mapping

The miniRT codebase uses a small subset of the mlx API. Below is the
mapping to SDL2 equivalents:

| mlx function | SDL2 equivalent |
|---|---|
| `mlx_init()` | `SDL_Init()` + `SDL_CreateWindow()` + `SDL_CreateRenderer()` |
| `mlx_new_window()` | Handled during init |
| `mlx_new_image()` / `mlx_get_data_addr()` | `SDL_CreateTexture()` + direct pixel buffer |
| `mlx_put_image_to_window()` | `SDL_UpdateTexture()` + `SDL_RenderCopy()` + `SDL_RenderPresent()` |
| `mlx_hook()` / `mlx_loop()` / `mlx_loop_hook()` | `SDL_PollEvent()` loop |
| `mlx_destroy_image()` / `mlx_destroy_window()` | `SDL_DestroyTexture()` / `SDL_DestroyWindow()` |
| `mlx_xpm_file_to_image()` | `IMG_Load()` (SDL_image) or `stb_image` |
| `mlx_do_key_autorepeatoff()` | SDL handles key repeat natively |

## Files Affected

Only 5 source files call mlx functions directly:

| File | mlx calls | Role |
|---|---|---|
| `ft_mini_rt.c` | 15 | Init, window creation, event hooks, loop |
| `ft_close.c` | 11 | Cleanup and destroy |
| `ft_texture.c` | 6 | XPM image loading |
| `ft_threads.c` | 4 | Image creation in threaded render |
| `ft_bzero_struct.c` | 1 | mlx pointer init |

## Recommended Approach

1. Create a `gfx_backend.h` abstraction with function pointers or
   compile-time `#ifdef` guards for MLX vs SDL2
2. Implement both backends behind the abstraction
3. Select backend at build time via a Makefile/CMake flag
4. Use `pkg-config` or CMake `find_package(SDL2)` for discovery

## Build System Impact

SDL2 integrates cleanly with CMake:

```cmake
find_package(SDL2 REQUIRED)
target_link_libraries(miniRT PRIVATE SDL2::SDL2)
```

This is significantly cleaner than the vendored mlx approach and
eliminates the need to ship the mlx source tree.
