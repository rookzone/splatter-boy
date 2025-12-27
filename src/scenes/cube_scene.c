#include "cube_scene.h"
#include "../_types.h"
#include "../graphics.h"
#include <gb/drawing.h>
#include <gb/gb.h>
#include <stdio.h>

#include <gbdk/emu_debug.h>

#include "../tiles/menuFont.h"

#define CUBE_ROTATION_SPEED 4

#define HUNT_PATTERN(idx, var) EMU_printf("%s: [%hu] -> %d", #var, (uint16_t)idx, (int16_t)var)

// 64-entry Sine table (scaled by 256 for fixed_t)
// Covers 0 to 360 degrees in 64 steps
const int16_t SIN_TABLE[64] = {
    0, 25, 50, 74, 98, 120, 142, 162, 181, 197, 212, 225, 236, 244, 250, 254,
    256, 254, 250, 244, 236, 225, 212, 197, 181, 162, 142, 120, 98, 74, 50, 25,
    0, -25, -50, -74, -98, -120, -142, -162, -181, -197, -212, -225, -236, -244, -250, -254,
    -256, -254, -250, -244, -236, -225, -212, -197, -181, -162, -142, -120, -98, -74, -50, -25
};

// Helper to get Cosine from Sine table
inline fixed_t get_sin(uint8_t angle) { return SIN_TABLE[angle & 63]; }
inline fixed_t get_cos(uint8_t angle) { return SIN_TABLE[(angle + 16) & 63]; }

typedef struct {
    fixed_t x, y, z;
} Vec3;

// Cube vertices scaled to fixed point (0.25 * 256 = 64)
const Vec3 cube_vertices[8] = {
    { 64,  64,  64}, {-64,  64,  64}, {-64, -64,  64}, { 64, -64,  64},
    { 64,  64, -64}, {-64,  64, -64}, {-64, -64, -64}, { 64, -64, -64}
};

// 12 edges of a cube (pairs of vertex indices)
const uint8_t cube_edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting lines
};

uint8_t cube_angle = 0;
fixed_t cube_dz = 400; // Distance from camera (Z translation)
// Store previous frame coordinates to erase them
int16_t old_sx[8], old_sy[8];
uint8_t first_frame = 1;
uint8_t has_old = 0;

const uint16_t inv_z_table[256] = {
    65535, 32768, 21845, 16384, 13107, 10922, 9362, 8192, 7281, 6553, 5957, 5461, 5041, 4681, 4369, 4096,
    3855, 3640, 3449, 3276, 3120, 2978, 2849, 2730, 2621, 2520, 2427, 2340, 2259, 2184, 2114, 2048,
    1985, 1927, 1872, 1820, 1771, 1724, 1680, 1638, 1598, 1560, 1524, 1489, 1456, 1424, 1394, 1365,
    1337, 1310, 1285, 1260, 1236, 1213, 1191, 1170, 1149, 1129, 1110, 1092, 1074, 1057, 1040, 1024,
    1008, 992, 978, 963, 949, 936, 923, 910, 897, 885, 873, 862, 851, 840, 829, 819,
    809, 799, 789, 780, 771, 762, 753, 744, 736, 728, 720, 712, 704, 697, 690, 682,
    675, 668, 661, 655, 648, 642, 636, 630, 624, 618, 612, 606, 601, 595, 590, 585,
    580, 574, 569, 564, 560, 555, 550, 546, 541, 537, 532, 528, 524, 520, 516, 512,
    508, 504, 500, 496, 492, 489, 485, 481, 478, 474, 471, 468, 464, 461, 458, 455,
    451, 448, 445, 442, 439, 436, 434, 431, 428, 425, 422, 420, 417, 414, 412, 409,
    407, 404, 402, 399, 397, 394, 392, 389, 387, 385, 383, 380, 378, 376, 374, 372,
    370, 368, 366, 364, 362, 360, 358, 356, 354, 352, 350, 348, 346, 344, 343, 341,
    339, 337, 336, 334, 332, 330, 329, 327, 326, 324, 322, 321, 319, 318, 316, 315,
    313, 312, 310, 309, 307, 306, 304, 303, 301, 300, 299, 297, 296, 295, 293, 292,
    291, 290, 288, 287, 286, 284, 283, 282, 281, 280, 278, 277, 276, 275, 274, 273,
    271, 270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256
};

void init_cube_scene(void)
{


}


void update_cube_scene(void) {
    uint8_t v_idx; // Vertex index
    uint8_t screen_x[8], screen_y[8]; 

    // 1. UPDATE ROTATION
    // Increase speed by adding a higher value (e.g., +2 or +4)
    cube_angle = (cube_angle + CUBE_ROTATION_SPEED) & 63; 
    
    fixed_t sin_val = get_sin(cube_angle);
    fixed_t cos_val = get_cos(cube_angle);
    
    // Scale trigonometric values for fixed-point rotation math
    fixed_t cos_scaled = cos_val >> 2; 
    fixed_t sin_scaled = sin_val >> 2;

    // 2. TRANSFORM AND PROJECT VERTICES
    for (v_idx = 0; v_idx < 8; v_idx++) {
        // Logic to determine cube corner coordinates (±1)
        int16_t x_offset = (v_idx == 1 || v_idx == 2 || v_idx == 5 || v_idx == 6) ? -cos_scaled : cos_scaled;
        int16_t z_offset = (v_idx >= 4) ? -sin_scaled : sin_scaled;
        
        // Apply Y-axis rotation matrix
        int16_t world_x = x_offset - z_offset;
        int16_t world_z = ((v_idx == 1 || v_idx == 2 || v_idx == 5 || v_idx == 6) ? -sin_scaled : sin_scaled) + 
                          ((v_idx >= 4) ? -cos_scaled : cos_scaled);
        
        // Define height (Y) for top and bottom faces
        int16_t world_y = (v_idx == 2 || v_idx == 3 || v_idx == 6 || v_idx == 7) ? -64 : 64;
        
        // Apply depth translation
        int16_t depth = world_z + cube_dz;

        screen_x[v_idx] = (((world_x << 6) + (world_x << 4)) / depth) + 80;
        screen_y[v_idx] = 72 - (((world_y << 6) + (world_y << 2) + (world_y << 1)) / depth);


    }

    vsync();
    
    // 3. ERASE PREVIOUS FRAME
    if (!first_frame) {
        for (uint8_t e_idx = 0; e_idx < 12; e_idx++) {
            uint8_t v1 = cube_edges[e_idx][0];
            uint8_t v2 = cube_edges[e_idx][1];
            
            bresenham_line(old_sx[v1], old_sy[v1], old_sx[v2], old_sy[v2], WHITE);
        }
    }

    first_frame = 0;

    // 4. DRAW NEW FRAME
    for (uint8_t e_idx = 0; e_idx < 12; e_idx++) {
        uint8_t v1 = cube_edges[e_idx][0];
        uint8_t v2 = cube_edges[e_idx][1];

        bresenham_line(screen_x[v1], screen_y[v1], screen_x[v2], screen_y[v2], BLACK);
    }

    // 5. SAVE STATE FOR NEXT UPDATE
    for (v_idx = 0; v_idx < 8; v_idx++) {
        old_sx[v_idx] = screen_x[v_idx];
        old_sy[v_idx] = screen_y[v_idx];
    }
}

void cleanup_cube_scene(void)
{

}
