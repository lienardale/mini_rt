#!/usr/bin/env python3
"""Convert BMP files to PNG format."""
import sys
from PIL import Image

def convert(src, dst=None):
    if dst is None:
        dst = src.rsplit('.', 1)[0] + '.png'
    img = Image.open(src)
    img.save(dst, 'PNG')
    print(f"{src} -> {dst} ({img.width}x{img.height})")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: bmp2png.py <input.bmp> [output.png]")
        sys.exit(1)
    convert(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else None)
