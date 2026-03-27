#!/usr/bin/env python3
"""Convert BMP renders to PNG for visualization in PRs and documentation.

Usage:
    python3 tools/bmp_to_png.py                    # Convert all regression refs
    python3 tools/bmp_to_png.py saves/              # Convert all BMPs in a folder
    python3 tools/bmp_to_png.py file.bmp            # Convert a single file
    python3 tools/bmp_to_png.py --output gallery/   # Specify output directory
"""

import os
import sys
import glob
import subprocess


def convert_bmp_to_png(bmp_path, output_dir):
    """Convert a single BMP file to PNG using macOS sips."""
    name = os.path.splitext(os.path.basename(bmp_path))[0]
    png_path = os.path.join(output_dir, name + ".png")
    try:
        subprocess.run(
            ["sips", "-s", "format", "png", bmp_path, "--out", png_path],
            check=True, capture_output=True,
        )
        size_kb = os.path.getsize(png_path) / 1024
        print(f"  {name}.bmp -> {name}.png ({size_kb:.0f} KB)")
        return True
    except Exception as e:
        print(f"  {name}.bmp -> FAILED: {e}")
        return False


def main():
    output_dir = None
    sources = []

    args = sys.argv[1:]
    i = 0
    while i < len(args):
        if args[i] == "--output" and i + 1 < len(args):
            output_dir = args[i + 1]
            i += 2
        else:
            sources.append(args[i])
            i += 1

    # Default: convert regression reference BMPs
    if not sources:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        project_dir = os.path.dirname(script_dir)
        sources = [os.path.join(project_dir, "tests", "regression", "reference")]

    bmp_files = []
    for src in sources:
        if os.path.isdir(src):
            bmp_files.extend(sorted(glob.glob(os.path.join(src, "*.bmp"))))
        elif os.path.isfile(src) and src.lower().endswith(".bmp"):
            bmp_files.append(src)
        else:
            print(f"Warning: skipping '{src}' (not a BMP file or directory)")

    if not bmp_files:
        print("No BMP files found.")
        sys.exit(1)

    # Default output: renders_preview/ in project root
    if output_dir is None:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        project_dir = os.path.dirname(script_dir)
        output_dir = os.path.join(project_dir, "renders_preview")

    os.makedirs(output_dir, exist_ok=True)

    print(f"Converting {len(bmp_files)} BMP(s) -> PNG in {output_dir}/")
    ok = sum(convert_bmp_to_png(f, output_dir) for f in bmp_files)
    print(f"\nDone: {ok}/{len(bmp_files)} converted.")


if __name__ == "__main__":
    main()
