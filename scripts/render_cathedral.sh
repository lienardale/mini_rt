#!/bin/bash
# Render the cathedral ruins scene
# Generates the .rt file via gen_scene and renders it with miniRT
#
# Usage: scripts/render_cathedral.sh [OPTIONS]
#   --preview         Quick render at 640x360
#   --full            Full resolution render (2560x1440)
#   --generate-only   Create .rt file without rendering
#   --png             Convert output BMP to PNG (requires Pillow)
#   --width N         Custom width (default: 1280)
#   --height N        Custom height (default: 720)
#   --threads N       Thread count (default: auto)
#   --seed N          RNG seed for debris placement (default: 42)
#   --output PATH     Output BMP path (default: saves/cathedral_ruins.bmp)
#   --scene PATH      Use existing .rt file instead of generating

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

WIDTH=1280
HEIGHT=720
THREADS="auto"
SEED=42
OUTPUT="$PROJECT_DIR/saves/cathedral_ruins.bmp"
SCENE_FILE=""
GENERATE_ONLY=0
USE_EXISTING=0
CONVERT_PNG=0

while [ $# -gt 0 ]; do
    case "$1" in
        --preview)
            WIDTH=640; HEIGHT=360 ;;
        --full)
            WIDTH=2560; HEIGHT=1440 ;;
        --generate-only)
            GENERATE_ONLY=1 ;;
        --png)
            CONVERT_PNG=1 ;;
        --width)
            WIDTH="$2"; shift ;;
        --height)
            HEIGHT="$2"; shift ;;
        --threads)
            THREADS="$2"; shift ;;
        --seed)
            SEED="$2"; shift ;;
        --output)
            OUTPUT="$2"; shift ;;
        --scene)
            SCENE_FILE="$2"; USE_EXISTING=1; shift ;;
        --help|-h)
            head -16 "$0" | tail -14
            exit 0 ;;
        *)
            echo "Unknown option: $1" >&2
            exit 1 ;;
    esac
    shift
done

# Build miniRT if missing
if [ ! -f "$PROJECT_DIR/miniRT" ]; then
    echo "Building miniRT..."
    NJOBS=$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)
    make -C "$PROJECT_DIR" -j"$NJOBS" 2>&1 | tail -1
fi

# Build gen_scene if missing
if [ ! -f "$PROJECT_DIR/gen_scene" ]; then
    echo "Building gen_scene..."
    make -C "$PROJECT_DIR" gen_scene 2>&1
fi

# Generate scene file
if [ "$USE_EXISTING" -eq 0 ]; then
    SCENE_FILE="$PROJECT_DIR/scenes/cathedral_ruins_gen.rt"
    echo "Generating cathedral scene (${WIDTH}x${HEIGHT}, seed=${SEED})..."
    "$PROJECT_DIR/gen_scene" --preset cathedral \
        --width "$WIDTH" --height "$HEIGHT" \
        --seed "$SEED" \
        --output "$SCENE_FILE"
    echo "Scene file: $SCENE_FILE"
    SHAPE_COUNT=$(grep -c "^[a-z]" "$SCENE_FILE" || true)
    echo "Shape/element count: $SHAPE_COUNT"
fi

if [ "$GENERATE_ONLY" -eq 1 ]; then
    echo "Done (generate-only mode)."
    exit 0
fi

# Render
mkdir -p "$(dirname "$OUTPUT")"
echo ""
echo "Rendering ${WIDTH}x${HEIGHT} with --threads=${THREADS}..."
echo "Output: $OUTPUT"
echo "This may take a while for large resolutions."
echo ""

START=$(date +%s)

cd "$PROJECT_DIR"
if command -v xvfb-run >/dev/null 2>&1; then
    RUNNER="xvfb-run -a"
else
    RUNNER=""
fi

$RUNNER "$PROJECT_DIR/miniRT" "$SCENE_FILE" -save \
    --output "$OUTPUT" --threads="$THREADS" || {
    echo "Retrying with default output path..."
    $RUNNER "$PROJECT_DIR/miniRT" "$SCENE_FILE" -save \
        --threads="$THREADS"
    mv -f "$PROJECT_DIR/miniRT.bmp" "$OUTPUT"
}

END=$(date +%s)
ELAPSED=$((END - START))
MINUTES=$((ELAPSED / 60))
SECONDS=$((ELAPSED % 60))

echo ""
echo "=== Render Complete ==="
echo "Time: ${MINUTES}m ${SECONDS}s"
ls -lh "$OUTPUT" | awk '{print "Size:", $5}'
file "$OUTPUT" | sed 's/.*: /Format: /'

# Convert to PNG if requested
if [ "$CONVERT_PNG" -eq 1 ]; then
    PNG_OUTPUT="${OUTPUT%.bmp}.png"
    echo ""
    echo "Converting to PNG..."
    python3 "$PROJECT_DIR/scripts/bmp2png.py" "$OUTPUT" "$PNG_OUTPUT"
fi
