# Scene Design & Rendering Workflows

## Architectural Scene Design Principles

### Building from primitives

Complex architectural scenes are built by composing simple shapes:

| Architectural Element | Primary Shape | Supporting Shapes |
|----------------------|---------------|-------------------|
| Walls | `bx` (box) | Multiple boxes with gaps for ruin effect |
| Columns/pillars | `cy` (cylinder) | `dk` (disk) base + `sp` (sphere) capital |
| Gothic arches | `to` (torus) | Half-torus between column tops |
| Floors | `pl` (plane) | `sq` (square) overlays for tile variation |
| Roofs/vaults | `tr` (triangle) | Angled triangles + `sq` for panels |
| Windows | `sq` with `trans:` | `sp` for round window tops (oculi) |
| Towers | `cy` base | `co` (cone) roof |
| Debris | `bx`, `sp`, `tr` | Mixed small shapes at ground level |
| Vegetation | `el` (ellipsoid) | Organic shapes for ivy/moss |

### Layered composition

Build scenes in layers, from structural to atmospheric:

1. **Structural shell** -- walls, floor plane, ceiling/roof
2. **Architectural features** -- columns, arches, windows, altar
3. **Decorative elements** -- crosses, candelabra, fonts, tomb slabs
4. **Decay/wear** -- broken columns, rubble, fallen roof pieces
5. **Atmospheric effects** -- dust motes, water puddles, stained glass light patches

### Depth through repetition

Colonnades (rows of repeated columns) and arcades (rows of arches) create visual depth and rhythm. Use loops in the scene generator:

```c
// 8 columns spaced 5 units apart along Z axis
i = 0;
while (i < 8) {
    col_z = 10.0 - (double)i * 5.0;
    cathedral_column(f, -6.0, col_z, heights[i]);
    i++;
}
```

Varying individual column heights within the row breaks monotony and suggests ruin/decay.

## Cathedral Ruins Case Study

### Scene overview

- **File:** `scenes/cathedral_ruins.rt` (307 lines)
- **Shapes:** ~236 geometric objects
- **Resolution:** 2560x1440
- **Render time:** ~20 minutes (multi-threaded, depends on hardware)

### Shape census

| Type | Count | Primary Use |
|------|-------|-------------|
| `sp` (sphere) | 68 | Column capitals, rubble, rose window glass, dust motes, candle flames |
| `bx` (box) | 49 | Walls, rubble blocks, altar, tower battlements, tomb slab |
| `cy` (cylinder) | 31 | Column shafts, candlesticks, cross, fallen columns, roof rafters |
| `dk` (disk) | 26 | Column bases, water puddles, candelabra top, font water |
| `to` (torus) | 20 | Gothic arches, rose window frame, fallen arch fragments |
| `tr` (triangle) | 16 | Vault fragments, rubble, fallen roof pieces |
| `sq` (square) | 13 | Floor tiles, stained glass windows, tilted roof panels |
| `el` (ellipsoid) | 10 | Ivy/vegetation, fallen glass shards |
| `pl` (plane) | 1 | Ground floor |
| `pa` (paraboloid) | 1 | Stone font/basin |
| `co` (cone) | 1 | Tower roof |

### Structural breakdown

| Section | Shapes | Z Range | Description |
|---------|--------|---------|-------------|
| Floor & tiles | 5 | all | Ground plane + 4 stone tile squares |
| Walls | 19 | -30 to 15 | Left/right walls, apse, entrance, transept stubs, buttresses |
| Left colonnade | 23 | -25 to 10 | 8 columns (cy+dk+sp), 2 broken |
| Right colonnade | 23 | -25 to 10 | 8 columns, 2 broken differently |
| Gothic arches | 18 | -28 to 7.5 | 12 side arches + 3 transverse + 1 apse + 2 fallen |
| Rose window | 22 | -29.3 | 2 torus frames + 12 outer + 6 inner + 1 center glass spheres |
| Altar area | 12 | -27 to -24 | Table, base, candles, cross, steps |
| Debris | ~45 | scattered | Fallen columns, rubble boxes/spheres, fragment triangles, capitals |
| Roof fragments | 18 | 0 to 17 | Vault triangles, tilted panels, floor debris, ridge beams, rafters |
| Bell tower | 15 | 11 to 23 | Base cylinder, upper section, cone roof, windows, battlements |
| Dust motes | 5 | 7 to 11 | Tiny transparent spheres in light beams |
| Water puddles | 8 | 0.02 | Reflective disks on floor |
| Stained glass | 15 | walls + floor | Transparent windows, floor glass shards, accent spheres |
| Decorations | 15 | various | Font, tomb, candelabra, ivy |

### Design decisions

**Sandstone palette (R: 170-200, G: 155-185, B: 130-160):** Warm earth tones with a consistent R > G > B bias. Each element varies by 5-10 per channel to prevent flat appearance while maintaining cohesion.

**Column height variation:** Columns at z=-10 (left) and z=0 (right) are broken short. This is controlled by the `left_h[]` and `right_h[]` arrays in the generator. Broken columns omit the capital sphere and have fallen capital spheres placed on the ground nearby.

**Torus for arches:** The torus is the closest available primitive to a semicircular arch. Placed at column-top height (y=14) with axis along Z, each torus spans the gap between adjacent columns. The 20 tori are the scene's biggest performance cost.

**Transparent spheres for rose window:** 12 outer spheres in a circular pattern (computed with sin/cos) with `trans:0.7 ior:1.3` simulate stained glass. Saturated colors (reds, blues, greens, golds) create the jewel-like effect.

## Material Strategy

| Goal | Properties | Example |
|------|-----------|---------|
| Polished stone | `spec:0.2` | Column shafts -- subtle sheen on weathered stone |
| Wet stone / water | `refl:0.5-0.7 spec:0.7-0.9` | Floor puddles -- mirror-like reflections |
| Stained glass | `trans:0.7-0.85 ior:1.3-1.5` | Rose window, wall windows -- colored light |
| Metal / gold | `spec:0.6-0.9 refl:0.2-0.3` | Cross, candlesticks -- metallic shine |
| Pure diffuse | (none) | Rubble, ivy -- no special surface treatment |
| Atmospheric dust | `trans:0.9 ior:1.0` | Dust motes -- nearly invisible, catches light |

**Guidelines:**
- Use `spec:` liberally (0.1-0.3) on stone for a wet/polished look
- Use `refl:` sparingly -- only on puddles and metal. Each reflective surface triggers recursive ray tracing (max depth 4)
- Combine `trans:` with `ior:` for glass effects. Higher `ior` = more distortion (1.0=air, 1.3=water, 1.5=glass, 1.7=crystal)
- Don't use materials on rubble/debris -- pure diffuse is faster and looks natural

## Lighting Design

### Warm/cool contrast

The cathedral uses 6 lights with deliberate warm/cool separation:

| Light | Position | Intensity | Color | Purpose |
|-------|----------|-----------|-------|---------|
| Main sun | (-3, 25, -5) | 0.9 | 255,240,200 | Primary warm illumination through roof gaps |
| Secondary sun | (5, 20, 5) | 0.6 | 255,235,190 | Fill warm light from side |
| Altar glow | (-8, 15, -15) | 0.5 | 255,220,180 | Warm backlight behind altar |
| Deep apse | (0, 12, -25) | 0.4 | 255,200,150 | Deep warm glow at back wall |
| Cool fill | (10, 18, -10) | 0.3 | 200,210,255 | Sky-blue through broken wall (contrast) |
| Ground bounce | (0, 3, 10) | 0.2 | 180,190,210 | Subtle cool ambient from floor |

### Intensity cascade

Lights decrease in intensity from 0.9 to 0.2. The key (brightest) light dominates shadow direction, while fill lights soften shadows without washing them out.

### Ambient ratio

Ambient is set very low (0.08) to preserve dramatic shadows. For gallery scenes, keep ambient at 0.05-0.15. Above 0.2 washes out the scene.

## Color Palette Design

### Base sandstone range

```
R: 170-200  (warm)
G: 155-185  (mid)
B: 130-160  (cool)
Offset: R > G by ~15, G > B by ~25
```

### Variation strategy

- **Per-element variation:** Each wall/column/rubble piece varies by 5-10 per channel from neighbors
- **Height gradient:** Upper elements slightly lighter than ground-level ones
- **Debris darkening:** Rubble is 10-20 units darker than intact structures
- **Accent colors:** Only on stained glass (saturated primaries), vegetation (40-80 green range), and metallic objects (gold: 150,120,40)

## Performance Considerations

### Torus cost

The torus intersection requires solving a **quartic equation** (degree 4 polynomial) per ray-torus test. With 20 tori, 6 lights (each casting shadow rays), and 3.7M pixels at 2560x1440, torus shapes dominate render time.

**Optimization options:**
- Replace decorative tori with cylinder approximations (save ~40% render time)
- Reduce torus count by removing some side arches
- Use `--width 1920 --height 1080` for faster iteration

### Shape count impact

| Shapes | Resolution | Approximate Time |
|--------|-----------|-----------------|
| ~236 (full) | 2560x1440 | 20-30 min |
| ~236 (full) | 1920x1080 | 10-15 min |
| ~236 (full) | 640x360 | 1-3 min |

The BVH acceleration structure helps with large shape counts, but per-intersection cost (especially torus) dominates.

### Shadow rays

Each light source fires a shadow ray per intersection point. With 6 lights, shadow computation is 6x the cost of a single-light scene. Consider reducing to 3-4 lights for faster iteration.

## Scene Improvement Tips

### Adding detail
- More debris variation: smaller rubble at column bases
- Moss/lichen: tiny green ellipsoids on north-facing surfaces (wall x=-12)
- Cobwebs: very thin triangles spanning column gaps
- Broken furniture: small boxes with wood color (80,50,30) near altar

### Camera angles
- **Low angle** (y=2): dramatic, emphasizes column height and vault
- **Bird's eye** (y=25, rot_x=0.2): shows floor plan layout
- **From altar** looking toward entrance: reverse perspective, entrance light as backlight
- **Close-up** on rose window (z=-28, y=12): detailed glass pattern

### Material tuning
- Increase `spec:` on all stone to 0.3 for a rain-wet look
- Add `refl:0.05` to floor plane for subtle ground reflection
- Increase rose window `trans:` to 0.9 for more light-passing effect

### Atmospheric effects
- Add more dust mote spheres (20-30) for visible light shafts
- Place a semi-transparent plane at y=2 with `trans:0.95` for ground fog

## Using the Scene Generator

### Available presets

| Preset | Description | Key Features |
|--------|-------------|-------------|
| `cornell` | Cornell box | Reflective/refractive spheres in enclosed room |
| `checkerboard` | 5x5 sphere grid | Color palette showcase |
| `solar` | Solar system | 8 planets orbiting a sun |
| `spiral` | Spiral spheres | Parametric arrangement |
| `grid` | Mixed shape grid | Spheres, boxes, cylinders in grid |
| `scatter` | Random scatter | Mixed shapes, random placement |
| `reflections` | Reflection showcase | Mirror and colored spheres |
| `glass` | Glass spheres | Transparent/refractive materials |
| `cathedral` | Cathedral ruins | 236-shape architectural scene |

### CLI reference

```bash
# Build the generator
make gen_scene

# Generate cathedral at default resolution (1920x1080)
./gen_scene --preset cathedral --output scenes/my_cathedral.rt

# Generate at full resolution with custom seed
./gen_scene --preset cathedral --width 2560 --height 1440 --seed 42 \
    --output scenes/cathedral_full.rt

# List all presets
./gen_scene --list
```

### Seed behavior

The `--seed` parameter controls randomized debris placement. Structural elements (walls, columns, arches, altar) are deterministic regardless of seed. Only the 30 debris items (15 boxes + 15 spheres) vary with different seeds.

### Adding new presets

1. Add writer helpers for any new shape types needed (see `write_disk`, `write_torus`, etc.)
2. Write a `gen_yourscene(FILE *f, t_config *cfg)` function
3. Register in `generate_scene()` dispatcher and `list_presets()`
4. Rebuild: `make gen_scene`

## Rendering Workflow

### Preview -> Iterate -> Full Render

```bash
# 1. Quick preview (~1-3 min)
scripts/render_cathedral.sh --preview

# 2. Edit scene file or adjust generator parameters
vim scenes/cathedral_ruins_gen.rt

# 3. Medium quality check (~5-10 min)
scripts/render_cathedral.sh

# 4. Full resolution render (~20-30 min)
scripts/render_cathedral.sh --full

# 5. Use existing .rt file (skip generation)
scripts/render_cathedral.sh --scene scenes/cathedral_ruins.rt --full
```

### Headless rendering

miniRT requires X11 initialization even for `-save` mode. The render script uses `xvfb-run` automatically. For manual headless rendering:

```bash
Xvfb :99 -screen 0 2560x1440x24 &
DISPLAY=:99 ./miniRT scene.rt -save --output out.bmp --threads=auto
```

### Makefile integration

```bash
# Render with default settings (1280x720)
make render-cathedral

# Pass custom arguments
make render-cathedral ARGS="--full"
make render-cathedral ARGS="--preview --seed 123"
```

### CLI flags reference

| Flag | Purpose | Default |
|------|---------|---------|
| `-save` | Render and save BMP | Required for headless |
| `--output PATH` | BMP save location | `miniRT.bmp` in CWD |
| `--threads=auto` | Use all CPU cores | Single-threaded |
| `--threads N` | Use N threads (max 64) | Single-threaded |
| `--width N` | Override resolution width | From .rt file |
| `--height N` | Override resolution height | From .rt file |
| `--verbose` | Print scene info | Off |
