#!/usr/bin/env python3
import sys
import re
import math
from PIL import Image

CHAR_WIDTH = 8
CHAR_HEIGHT = 16
BYTES_PER_CHAR = CHAR_HEIGHT
CHARS_PER_ROW = 16

def parse_c_header(header_path):
	with open(header_path, 'r', encoding='cp437', errors='ignore') as f:
		content = f.read()

	array_name_match = re.search(r'(?:unsigned\s+char|uint8_t)\s+(\w+)\s*\[\]', content)
	array_name = array_name_match.group(1) if array_name_match else "devkitpro_font"

	start_idx = content.find('{', content.find(array_name))
	if start_idx == -1:
		print("Error: Could not find opening brace '{' for the font array.")
		sys.exit(1)

	array_data = content[start_idx + 1:]

	array_data = re.sub(r'/\*.*?\*/', '', array_data, flags=re.DOTALL)

	array_data = re.sub(r'//.*', '', array_data)

	hex_values = re.findall(r'0x[0-9a-fA-F]{2}', array_data)
	byte_data = bytes(int(val, 16) for val in hex_values)

	return array_name, byte_data

def font_to_png(header_path, png_path):
	array_name, font_bytes = parse_c_header(header_path)
	total_chars = len(font_bytes) // BYTES_PER_CHAR

	if total_chars == 0:
		print("Error: Extracted 0 valid font bytes from the file.")
		return

	rows = math.ceil(total_chars / CHARS_PER_ROW)
	img_width = CHARS_PER_ROW * CHAR_WIDTH
	img_height = rows * CHAR_HEIGHT

	img = Image.new('RGB', (img_width, img_height), color=(0, 0, 0))
	pixels = img.load()

	for ch_idx in range(total_chars):
		grid_x = ch_idx % CHARS_PER_ROW
		grid_y = ch_idx // CHARS_PER_ROW

		pixel_offset_x = grid_x * CHAR_WIDTH
		pixel_offset_y = grid_y * CHAR_HEIGHT

		for row_y in range(CHAR_HEIGHT):
			byte_idx = (ch_idx * BYTES_PER_CHAR) + row_y
			row_byte = font_bytes[byte_idx]

			for bit_x in range(CHAR_WIDTH):
				is_active = (row_byte >> (7 - bit_x)) & 1
				if is_active:
					pixels[pixel_offset_x + bit_x, pixel_offset_y + row_y] = (255, 255, 255)

	img.save(png_path)
	print(f"Successfully exported {total_chars} glyphs from '{array_name}' into '{png_path}' ({img_width}x{img_height})")

def cp437(b: int) -> str:
    if 0 <= b <= 31:
        return f"^{chr(b + 64)}"

    else:
        return bytes([b]).decode('cp437')

def png_to_font(png_path, header_path, array_name="donut_font_8x16"):
	try:
		img = Image.open(png_path).convert('RGB')
	except Exception as e:
		print(f"Error loading image: {e}")
		return

	img_width, img_height = img.size

	grid_cols = img_width // CHAR_WIDTH
	grid_rows = img_height // CHAR_HEIGHT
	total_chars = grid_cols * grid_rows

	pixels = img.load()
	output_lines = [
		"// generated with tools/fontgen.py",
		f"unsigned char {array_name}[] = {{"
	]

	for ch_idx in range(total_chars):
		grid_x = ch_idx % grid_cols
		grid_y = ch_idx // grid_cols

		pixel_offset_x = grid_x * CHAR_WIDTH
		pixel_offset_y = grid_y * CHAR_HEIGHT

		output_lines.append(f"\n\t/* {ch_idx} 0x{ch_idx:02x} '{cp437(ch_idx)}' */")

		for row_y in range(CHAR_HEIGHT):
			row_byte = 0
			binary_str = ""
			for bit_x in range(CHAR_WIDTH):
				r, g, b = pixels[pixel_offset_x + bit_x, pixel_offset_y + row_y][:3]
				if (r + g + b) / 3 > 127:
					row_byte |= (1 << (7 - bit_x))
					binary_str += "1"
				else:
					binary_str += "0"

			output_lines.append(f"\t0x{row_byte:02x}, /* {binary_str} */")

	output_lines.append("};")

	with open(header_path, 'w', encoding='cp437') as f:
		f.write("\n".join(output_lines) + "\n")

def print_usage():
	print("Usage:")
	print("font_converter.py to-png <input.h> <output.png>")
	print("font_converter.py to-header <input.png> <output.h> [array_name]")

if __name__ == "__main__":
	if len(sys.argv) < 4:
		print_usage()
		sys.exit(1)

	mode = sys.argv[1].lower()
	if mode == "to-png":
		font_to_png(sys.argv[2], sys.argv[3])
	elif mode == "to-header":
		arr_name = sys.argv[4] if len(sys.argv) > 4 else "donut_font_8x16"
		png_to_font(sys.argv[2], sys.argv[3], arr_name)
	else:
		print_usage()
