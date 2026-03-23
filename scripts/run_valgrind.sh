#!/bin/bash
# Run Valgrind memory analysis on all miniRT scene files
# Uses -save flag for headless BMP output (no X window needed)

set -euo pipefail

BINARY="./miniRT"
SCENES_DIR="./scenes"
SUPP_FILE="./valgrind.supp"
LOG_DIR="./valgrind_logs"
EXIT_CODE=0

mkdir -p "$LOG_DIR"

if [ ! -f "$BINARY" ]; then
    echo "Binary not found. Run 'make re' first."
    exit 1
fi

for scene in "$SCENES_DIR"/*.rt; do
    scene_name=$(basename "$scene" .rt)
    log_file="$LOG_DIR/${scene_name}.log"

    echo "=== Testing: $scene ==="
    # Run valgrind with virtual framebuffer; miniRT may exit non-zero on invalid scenes
    xvfb-run -a valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --suppressions="$SUPP_FILE" \
        --log-file="$log_file" \
        "$BINARY" "$scene" -save 2>&1 || true

    # Check for definite leaks
    if grep -q "definitely lost: 0 bytes" "$log_file"; then
        echo "  PASS: No definite leaks"
    else
        echo "  WARN: Possible definite leaks (see $log_file)"
        EXIT_CODE=1
    fi

    # Check for indirect leaks
    if grep -q "indirectly lost: 0 bytes" "$log_file"; then
        echo "  PASS: No indirect leaks"
    else
        echo "  WARN: Possible indirect leaks (see $log_file)"
        EXIT_CODE=1
    fi

    # Check for errors
    if grep -q "ERROR SUMMARY: 0 errors" "$log_file"; then
        echo "  PASS: No errors"
    else
        echo "  WARN: Errors detected (see $log_file)"
        EXIT_CODE=1
    fi

    # Clean up generated BMP
    rm -f miniRT.bmp

    echo ""
done

echo "================================"
if [ $EXIT_CODE -eq 0 ]; then
    echo "All scenes passed Valgrind checks."
else
    echo "Some scenes had issues. Check logs in $LOG_DIR/"
fi
exit $EXIT_CODE
