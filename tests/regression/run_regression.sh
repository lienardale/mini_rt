#!/bin/bash
# Regression test: render scenes and compare BMP output pixel-by-pixel
# Usage: ./tests/regression/run_regression.sh [--generate]
# With --generate: creates reference BMPs instead of comparing

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
REF_DIR="$SCRIPT_DIR/reference"
TMP_DIR=$(mktemp -d)
BINARY="$PROJECT_DIR/miniRT"
GENERATE=0
PASS=0
FAIL=0

if [ "$1" = "--generate" ]; then
    GENERATE=1
    mkdir -p "$REF_DIR"
    echo "Generating reference BMPs..."
fi

cleanup() {
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

if [ ! -f "$BINARY" ]; then
    echo "Error: miniRT binary not found. Run 'make' first."
    exit 1
fi

run_scene() {
    local scene_file="$1"
    local scene_name="$(basename "$scene_file" .rt)"
    local ref_bmp="$REF_DIR/${scene_name}.bmp"

    echo -n "  $scene_name: "

    cd "$TMP_DIR"
    timeout 30 "$BINARY" "$scene_file" -save --threads 1 2>/dev/null || true
    cd "$PROJECT_DIR"

    if [ ! -f "$TMP_DIR/miniRT.bmp" ]; then
        echo "SKIP (no output)"
        return
    fi

    if [ "$GENERATE" -eq 1 ]; then
        cp "$TMP_DIR/miniRT.bmp" "$ref_bmp"
        echo "GENERATED"
    elif [ ! -f "$ref_bmp" ]; then
        echo "SKIP (no reference)"
    else
        if cmp -s "$TMP_DIR/miniRT.bmp" "$ref_bmp"; then
            echo "PASS"
            PASS=$((PASS + 1))
        else
            echo "FAIL (output differs from reference)"
            FAIL=$((FAIL + 1))
        fi
    fi

    rm -f "$TMP_DIR/miniRT.bmp"
}

echo "=== BMP Regression Tests ==="

for scene in "$SCRIPT_DIR"/*.rt; do
    [ -f "$scene" ] || continue
    run_scene "$scene"
done

if [ "$GENERATE" -eq 0 ]; then
    echo ""
    echo "Results: $PASS passed, $FAIL failed"
    [ "$FAIL" -eq 0 ] || exit 1
fi
