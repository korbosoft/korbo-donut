#!/usr/bin/env python3
import argparse
import struct
from PIL import Image

def main(input_path, output_path):
    img = Image.open(input_path)
    if img.mode != 'P':
        raise ValueError(f"Image must be in palette mode (P). Current mode: {img.mode}")

    width, height = img.size

    palette_rgb = img.getpalette()
    palette_bytes = []

    transparency = img.info.get('transparency')
    if transparency is not None:
        for i in range(256):
            r = palette_rgb[i*3] if i*3 < len(palette_rgb) else 0
            g = palette_rgb[i*3+1] if i*3+1 < len(palette_rgb) else 0
            b = palette_rgb[i*3+2] if i*3+2 < len(palette_rgb) else 0

            a = 255
            if isinstance(transparency, bytes) and i < len(transparency):
                a = transparency[i]
            elif isinstance(transparency, int) and i == transparency:
                a = 0

            palette_bytes.extend([r, g, b, a])
        palette_size = 1024
        alpha_flag = 1
    else:
        if len(palette_rgb) < 768:
            palette_rgb += [0] * (768 - len(palette_rgb))
        palette_bytes = palette_rgb[:768]
        palette_size = 768
        alpha_flag = 0

    # Modernized for Pillow 14 compatibility
    pixel_data = list(img.get_flattened_data())

    with open(output_path, 'wb') as f:
        f.write(struct.pack('<II', width, height))
        f.write(struct.pack('B', alpha_flag))
        f.write(bytes(palette_bytes))
        f.write(bytes(pixel_data))

    print(f"successfully converted '{input_path}'!!!1")
    print(f"total file size is {8 + 1 + palette_size + len(pixel_data)} bytes")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="convert paletted pngs into the format my program uses")
    parser.add_argument('input', help="path to input PNG")
    parser.add_argument('output', help="path to output file")

    args = parser.parse_args()
    main(args.input, args.output)
