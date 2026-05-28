#!/usr/bin/env python3
import re
import numpy
import struct

def get_signatures(font):
    signatures = []
    for code in range(32, 126):
        if code >= len(font):
            break

        char_data = font[code]
        # 16 bytes -> 8x16 bit matrix
        bits = numpy.unpackbits(numpy.array(char_data, dtype=numpy.uint8)).reshape(16, 8)

        sig_vector = []
        for r_block in numpy.array_split(bits, 4, axis=0):
            for c_block in numpy.array_split(r_block, 2, axis=1):
                val = int(round(numpy.mean(c_block) * 255))
                sig_vector.append(val)

        signatures.append((code, sig_vector))
    return signatures

def create_ascii_lut(signatures):
    lut = [0] * 65536

    for pattern in range(65536):
        target = [((pattern >> (i * 2)) & 0x03) * 85 for i in range(8)]

        best_char = 32
        min_diff = float('inf')

        for char_code, sig in signatures:
            # manhattan distance (SAD)
            diff = sum(abs(target[k] - sig[k]) for k in range(8))

            if diff < min_diff:
                min_diff = diff
                best_char = char_code
                if diff == 0: break

        lut[pattern] = best_char
    return lut

if __name__ == "__main__":
    font = []

    try:
        with open("font.h", 'r', encoding='cp437') as f:
            current_bytes = []
            for line in f:
                clean = re.sub(r'/\*.*?\*/', '', line)
                finds = re.findall(r'0x[0-9a-fA-F]{2}', clean)
                for h in finds:
                    current_bytes.append(int(h, 16))
                    if len(current_bytes) == 16:
                        font.append(list(current_bytes))
                        current_bytes = []

        print(f'successfully read {len(font)} characters from font. signature time')
    except FileNotFoundError:
        print("font.h not found :(")
        exit()

    sigs = get_signatures(font)
    print('font signatures are done, making LUT now...')

    lut = create_ascii_lut(sigs)
    print('saving LUT...')

    with open("shape_lut.bin", "wb") as f:
        binary_data = struct.pack(f'{len(lut)}B', *lut)
        f.write(binary_data)

    print('LUT saved to shape_lut.bin, making character ramp...')
    densities = []
    for i in range(32, 126):
        on_pixels = sum(bin(byte).count('1') for byte in font[i])
        densities.append({'char': chr(i), 'density': on_pixels})

    densities.sort(key=lambda x: x['density'])

    ramp = []
    num_chars = len(densities)
    for i in range(32):
        index = int(i * (num_chars - 1) / 31)
        ramp.append(densities[index]['char'])

    print('const char ramp[] = "' + "".join(ramp) + '"; // generated with tools/gen.py')
    print('boobulation complete. Bye bye :)')
