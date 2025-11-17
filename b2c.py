import os

# --- CONFIGURATION ---
FILES_TO_CONVERT = {
    "tileset.bin": "title_tiles_data",
    "tile_map.bin": "title_map_data",
}
BYTES_PER_LINE = 16  # Standard GBDK formatting

def convert_bin_to_c(input_filename, array_name):
    """Reads a binary file and writes the content to a .c file as a hex array."""
    try:
        with open(input_filename, 'rb') as f:
            data = f.read()
    except FileNotFoundError:
        print(f"Error: File not found: {input_filename}")
        return

    output_c_filename = array_name + ".c"
    output_h_filename = array_name + ".h"

    # --- Write .h file ---
    with open(output_h_filename, 'w') as f_h:
        f_h.write(f'#ifndef {array_name.upper()}_H\n')
        f_h.write(f'#define {array_name.upper()}_H\n\n')
        f_h.write(f'extern const unsigned char {array_name}[];\n')
        f_h.write(f'#define {array_name.upper()}_LENGTH {len(data)}\n\n')
        
        # Add map size defines for the map data
        if array_name == "title_map_data":
            f_h.write(f'#define {array_name.upper()}_WIDTH 20\n')
            f_h.write(f'#define {array_name.upper()}_HEIGHT 18\n\n')

        # Add tile count for the tileset data
        if array_name == "title_tiles_data":
            f_h.write(f'#define {array_name.upper()}_COUNT {len(data) // 16}\n\n')

        f_h.write(f'#endif // {array_name.upper()}_H\n')

    # --- Write .c file ---
    with open(output_c_filename, 'w') as f_c:
        f_c.write(f'// Generated from {input_filename} by bin_to_c.py\n\n')
        f_c.write(f'const unsigned char {array_name}[] = {{\n    ')
        
        byte_count = 0
        for byte in data:
            # Format byte as 0x00 hex string
            hex_str = f"0x{byte:02X}"
            f_c.write(hex_str + ',')
            byte_count += 1
            
            if byte_count % BYTES_PER_LINE == 0:
                f_c.write('\n    ')
            elif byte_count < len(data):
                f_c.write(' ')
                
        f_c.write('\n};\n')

    print(f"Successfully converted {input_filename} to {output_c_filename} and {output_h_filename}")


if __name__ == "__main__":
    for bin_file, array_name in FILES_TO_CONVERT.items():
        convert_bin_to_c(bin_file, array_name)