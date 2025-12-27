#include "cube_scene.h"
#include "../_types.h"
#include <gb/drawing.h>
#include <gb/gb.h>
#include <stdio.h>

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
fixed_t cube_dz = 300; // Distance from camera (Z translation)
// Store previous frame coordinates to erase them
int16_t old_sx[8], old_sy[8];
uint8_t first_frame = 1;
uint8_t has_old = 0;


void init_cube_scene(void)
{

}

void update_cube_scene(void)
{
    uint8_t i;
    int16_t sx[8], sy[8]; 

    cube_angle = (cube_angle + 1) & 63; 
    fixed_t s = get_sin(cube_angle);
    fixed_t c = get_cos(cube_angle);
    fixed_t sc = c >> 2; 
    fixed_t ss = s >> 2;

    for (i = 0; i < 8; i++) {
        int16_t x_comp = (i == 1 || i == 2 || i == 5 || i == 6) ? -sc : sc;
        int16_t z_comp = (i >= 4) ? -ss : ss;
        int16_t rx = x_comp - z_comp;
        int16_t rz = ((i == 1 || i == 2 || i == 5 || i == 6) ? -ss : ss) + ((i >= 4) ? -sc : sc);
        int16_t ry = (i == 2 || i == 3 || i == 6 || i == 7) ? -64 : 64;
        int16_t pz = rz + cube_dz;

        sx[i] = (((rx << 6) + (rx << 4)) / pz) + 80;
        sy[i] = 72 - (((ry << 6) + (ry << 2) + (ry << 1)) / pz);
    }

    if (!first_frame) {
        color(WHITE, WHITE, SOLID);
        for (i = 0; i < 12; i++) {
            line(old_sx[cube_edges[i][0]], old_sy[cube_edges[i][0]],
                old_sx[cube_edges[i][1]], old_sy[cube_edges[i][1]]);
        }
    }

    color(BLACK, WHITE, SOLID);
    for (i = 0; i < 12; i++) {
        line(sx[cube_edges[i][0]], sy[cube_edges[i][0]],
            sx[cube_edges[i][1]], sy[cube_edges[i][1]]);
    }

    for (i = 0; i < 8; i++) {
        old_sx[i] = sx[i];
        old_sy[i] = sy[i];
    }

    first_frame = 0;
}

void cleanup_cube_scene(void)
{

}
