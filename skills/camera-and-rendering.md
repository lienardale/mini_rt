# Camera System & Rendering Internals

## Camera Orientation

The camera orientation field in `.rt` files is **Euler rotation angles in units of PI radians**, applied in X → Y → Z order. It is NOT a direction vector.

```
c  <pos_x,pos_y,pos_z>  <rot_x,rot_y,rot_z>  <fov>
```

- `rot_x * PI` = rotation around X axis (pitch: positive tilts camera downward)
- `rot_y * PI` = rotation around Y axis (yaw: turns camera left/right)
- `rot_z * PI` = rotation around Z axis (roll: rotates the image plane)

### Default camera direction

With orientation `0,0,0`, the camera looks in the **-Z direction**. The pixel-to-ray mapping in `ft_ray.c:ft_ray_single()`:

```c
cam->rij.x = (i - (win->x / 2));      // screen X → ray X
cam->rij.y = (j - (win->y / 2));      // screen Y → ray Y
cam->rij.z = -(win->x / (2 * size));  // focal length, points at -Z
cam->rij = ft_rotation_vect(cam->rij, cam->ori);  // apply rotations
```

### Y-axis convention

The render loop in `ft_mini_rt.c` iterates j from 0 to height. In `ft_argb.c:ft_pix()`, the buffer write uses `(w->y - y - j - 1)` which maps j=0 to the **top** of the displayed image. Since j=0 → `rij.y = -height/2` (pointing down), the top of the image looks downward by default.

This means **without any rotation**, the image Y-axis is already correct: top→down, bottom→up.

### Common pitfall: Z rotation of ±1

Setting `rot_z = -1` (or `1`) applies a 180° rotation around Z, which negates both X and Y ray components. This **flips the image vertically and horizontally**. Many legacy scenes in `scenes/` use this, but for gallery scenes it caused the floor plane to appear at the top.

### Rotation application (`ft_cam.c`)

```c
t_pt ft_rotation_vect(t_pt p, t_pt ori)
{
    tmp = ft_rotate_arr_x(p, ori.x * M_PI);  // pitch
    tmp = ft_rotate_arr_y(tmp, ori.y * M_PI); // yaw
    tmp = ft_rotate_arr_z(tmp, ori.z * M_PI); // roll
    return (tmp);
}
```

### Practical examples

| Orientation | Effect |
|-------------|--------|
| `0,0,0` | Look at -Z, no tilt |
| `0.1,0,0` | Look at -Z, tilt down ~18° |
| `0,0.5,0` | Turn 90° around Y (look at -X) |
| `0,0,-1` | 180° roll — flips image vertically and horizontally |
| `-0.1,0,-1` | Tilt up 18° + full flip — floor appears on top |

## FPS-Style Camera Controls

### Movement Model

Camera movement uses FPS-style controls where WASD moves relative to the camera's facing direction, projected onto the XZ ground plane.

The forward and right vectors are derived from the camera's yaw angle (`ori.y * M_PI`):

```c
// Forward direction on XZ plane (ignoring pitch)
forward.x = -sin(yaw);
forward.z = -cos(yaw);

// Right strafe direction (90° clockwise from forward)
right.x = cos(yaw);
right.z = -sin(yaw);
```

Y movement (PgUp/PgDn) remains world-relative — standard FPS convention.

Implementation: `src/ft_cam_move.c` — `ft_cam_forward_xz()`, `ft_cam_right_xz()`, `ft_cam_apply_movement()`.

### Continuous Input Architecture

The event system uses KeyPress/KeyRelease events with a frame loop for smooth continuous motion:

1. **KeyPress** (`X11 event 2`): Sets bit in `t_window.keys_held` bitfield
2. **KeyRelease** (`X11 event 3`): Clears bit in `t_window.keys_held`
3. **Frame loop** (`mlx_loop_hook`): Each iteration checks `keys_held`; if any movement/look bits are set, applies incremental movement and re-renders

Key-to-bit mapping is defined in `includes/keybinds.h` (`KEY_BIT_FWD`, `KEY_BIT_BKW`, etc.) and converted via `ft_keycode_to_bit()` in `src/ft_cam_move.c`.

One-shot keys (ESC, Space, R, FOV +/-) are handled immediately in the KeyPress handler and do not use the bitfield.

### Rendering During Movement

While keys are held, the scene renders at draft resolution (4x pixel stepping) for responsiveness. When all keys are released, a final full-resolution render is triggered. This matches the existing dual-render pattern.

### Step Sizes

| Parameter | Value | Defined in |
|-----------|-------|------------|
| `MOVE_STEP` | 0.5 | `keybinds.h` |
| `LOOK_STEP` | 0.02 | `keybinds.h` |

### X11 Auto-Repeat

`mlx_do_key_autorepeatoff()` is called at startup to prevent X11 from generating spurious KeyPress/KeyRelease pairs when a key is held. `mlx_do_key_autorepeaton()` is called in `ft_close()` to restore normal behavior on exit.

## Pixel Writing

`ft_argb.c:ft_pix()` writes to the mlx image buffer with a Y-flip:

```c
w->data[((w->y - y - j - 1) * w->x + x + i) * 4 + channel]
```

In minilibX, buffer row 0 = top of window. This formula maps render y=0 → top of display.

## BMP Save

`ft_save.c:ft_pixel_data()` reads from the mlx buffer with another Y-flip (`win->y - y - 1`), producing a standard bottom-up BMP. The save always writes to `miniRT.bmp` in the working directory.

`-save` requires mlx/X11 initialization (mlx_init + mlx_new_window are called unconditionally before the save flag is checked). Use `Xvfb` for headless rendering:

```bash
Xvfb :42 -screen 0 1920x1080x24 -ac &
DISPLAY=:42 ./miniRT scene.rt -save
```

## Scene File Authoring Guidelines

### Camera setup

- Use `rot_z = 0` (not -1) for correct image orientation
- Use positive `rot_x` for downward tilt (bird's-eye views)
- Camera position Y should be above objects; `rot_x` of 0.05–0.15 covers most cases

### Lighting and contrast

- Ambient ratio above 0.2 washes out scenes. Use 0.05–0.15 for gallery scenes.
- Neutral wall/plane colors of 150–200 work better than 220–240 for scenes with white/reflective objects
- At least 2 lights from different angles improves depth perception

## Post-42 Scene Generation Rules

All scenes created after the initial 42 project **MUST** follow these rules. Original 42 scenes (cylinder, exemple, inside_cy, inside_cy_l, inside_sp, inside_sp_l, lights, objects, square, tiny, triangle, error) are left as-is.

### Drone-view camera placement

Every showcase scene uses an elevated "drone view" where **all objects are fully visible at first glance**:

| Parameter | Rule | Typical range |
|-----------|------|---------------|
| Camera Y | max_object_height + 8–12 | 12–16 |
| Camera Z | furthest_object_z + scene_x_span × 0.8 | 20–40 |
| rot_x (pitch) | 0.12–0.18 (22°–32° downward tilt) | 0.15 |
| rot_y (yaw) | 0 always | 0 |
| rot_z (roll) | 0 always (**never** ±1) | 0 |
| FOV | 65–70° | 70 |

**Checklist for row-based layouts** (e.g. materials, pbr_showcase):
- All objects in every row must be fully visible — no clipping at frame edges
- Clear ground visible between rows (at least 4–6 units of Z separation)
- Camera high enough that rear rows are not occluded by front rows

### Sky background

Add the `E` directive (on its own line, after `A`) to enable the gradient sky. This replaces the black void with a blue-to-white horizon gradient.

```
R   1920  1080
A   0.15   255,255,255
E
c   0,14,28   0.15,0,0   70
```

### Object layout

- Spread objects along X with at least 1 diameter of spacing between them
- Use 2+ lights from different angles for depth/shadow separation
- Ground plane color: light gray (220–235) to contrast with objects and sky

### Material properties (parsed in `ft_material.c`)

Appended to shape lines as `key:value` pairs:

| Property | Range | Effect |
|----------|-------|--------|
| `spec:` | 0–1 | Specular highlight intensity |
| `refl:` | 0–1 | Reflectivity (recursive ray tracing, max depth 4) |
| `trans:` | 0–1 | Transparency |
| `ior:` | >0 | Index of refraction (used with trans) |
| `metal:` | 0–1 | Metallic factor (PBR metallic-roughness workflow) |
| `rough:` | 0–1 | Surface roughness (0 = mirror, 1 = matte) |
| `emit:` | R,G,B | Emissive light (HDR values allowed, e.g. `emit:500,400,300`) |

### Supported shape IDs

| ID | Shape | File |
|----|-------|------|
| `sp` | Sphere | `ft_sphere.c` |
| `pl` | Plane | `ft_plane.c` |
| `sq` | Square | `ft_square.c` |
| `cy` | Cylinder | `ft_cylinder.c` |
| `tr` | Triangle | `ft_triangle.c` |
| `co` | Cone | `ft_cone.c` |
| `dk` | Disk | `ft_disk.c` |
| `to` | Torus | `ft_torus.c` |
| `el` | Ellipsoid | `ft_ellipsoid.c` |
| `bx` | Box | `ft_box.c` |

## Scene Generator

`tools/gen_scene.c` generates `.rt` files for 8 presets:

```bash
cc -lm tools/gen_scene.c -o gen_scene
./gen_scene --preset cornell --output gallery/cornell_box.rt
./gen_scene --list  # show all presets
```

The `write_camera` function accepts an intuitive direction vector and internally converts it to Euler angles via `dir_to_euler()` using `-asin(dy)/PI` for pitch and `atan2(-dx,-dz)/PI` for yaw.
