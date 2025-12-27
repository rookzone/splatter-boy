// main.c

#include "scene_manager.h"
#include "debug.h"
#include "game_state.h"
#include "input.h"
#include "graphics.h"
#include "platform.h"
#include "tiles/menuFont.h"

#include <gb/drawing.h>
#include <gb/gb.h>
#include "types.h"
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


void render_rotating_cube() {
    uint8_t i;
    int16_t sx[8], sy[8]; 
    
    // 1. ERASE PREVIOUS FRAME
    // Instead of clearing the whole screen, we redraw the old lines in XOR mode.
    color(WHITE,WHITE,SOLID);
    if (!first_frame) {
        for (i = 0; i < 12; i++) {
            line(old_sx[cube_edges[i][0]], old_sy[cube_edges[i][0]],
                 old_sx[cube_edges[i][1]], old_sy[cube_edges[i][1]]);
        }
    }

    // 2. CALCULATE NEW ROTATION
    cube_angle = (cube_angle + 1) & 63; 
    fixed_t s = get_sin(cube_angle);
    fixed_t c = get_cos(cube_angle);

    // Pre-scale sin/cos by 0.25 (shift by 2) to avoid vertex multiplication
    fixed_t sc = c >> 2; 
    fixed_t ss = s >> 2;

    for (i = 0; i < 8; i++) {
        // Use the vertex index 'i' to determine signs based on your cube.js data
        // x is -64 for 1,2,5,6. z is -64 for 4,5,6,7. y is -64 for 2,3,6,7.
        int16_t x_comp = (i == 1 || i == 2 || i == 5 || i == 6) ? -sc : sc;
        int16_t z_comp = (i >= 4) ? -ss : ss;
        
        // rx = (x*c - z*s) >> 8  ==> (x/256)*c - (z/256)*s
        int16_t rx = x_comp - z_comp;
        // rz = (x*s + z*c) >> 8
        int16_t rz = ((i == 1 || i == 2 || i == 5 || i == 6) ? -ss : ss) + 
                     ((i >= 4) ? -sc : sc);
        
        int16_t ry = (i == 2 || i == 3 || i == 6 || i == 7) ? -64 : 64;
        int16_t pz = rz + cube_dz;

        // 3. PROJECT WITH BITSHIFTS
        // Replacing (rx * 80) with (rx << 6) + (rx << 4) [64 + 16 = 80]
        // Replacing (ry * 70) with (ry << 6) + (ry << 2) + (ry << 1) [64 + 4 + 2 = 70]
        sx[i] = (((rx << 6) + (rx << 4)) / pz) + 80;
        sy[i] = 72 - (((ry << 6) + (ry << 2) + (ry << 1)) / pz);
    }

    color(BLACK,WHITE,SOLID);
    // 4. DRAW NEW FRAME
    for (i = 0; i < 12; i++) {
        line(sx[cube_edges[i][0]], sy[cube_edges[i][0]],
             sx[cube_edges[i][1]], sy[cube_edges[i][1]]);
    }

    // 5. SAVE COORDINATES FOR NEXT FRAME
    for (i = 0; i < 8; i++) {
        old_sx[i] = sx[i];
        old_sy[i] = sy[i];
    }
    first_frame = 0;
}

// Forward declarations
void pause_game(void);

void main(void) 
{
    // Load first "game" scene
    //set_scene(SCENE_TITLE_SCREEN);

    color(WHITE, WHITE, SOLID);
    box(0, 0, 159, 143, M_FILL); // Clear once at start
    
    while(1) {
        uint8_t i;
        int16_t sx[8], sy[8]; 

        // --- STEP 1: CALCULATE (Do this BEFORE VSync) ---
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

        // --- STEP 2: WAIT FOR VSYNC ---
        // This ensures the following drawing happens at the very start of the frame
        vsync(); 

        // --- STEP 3: ERASE (Redraw old lines in White) ---
        if (!first_frame) {
            color(WHITE, WHITE, SOLID);
            for (i = 0; i < 12; i++) {
                line(old_sx[cube_edges[i][0]], old_sy[cube_edges[i][0]],
                    old_sx[cube_edges[i][1]], old_sy[cube_edges[i][1]]);
            }
        }

        // --- STEP 4: DRAW (Draw new lines in Black) ---
        color(BLACK, WHITE, SOLID);
        for (i = 0; i < 12; i++) {
            line(sx[cube_edges[i][0]], sy[cube_edges[i][0]],
                sx[cube_edges[i][1]], sy[cube_edges[i][1]]);
        }

        // --- STEP 5: SAVE FOR NEXT FRAME ---
        for (i = 0; i < 8; i++) {
            old_sx[i] = sx[i];
            old_sy[i] = sy[i];
        }
        first_frame = 0;
    }


    while (1==2) {

        // Update state, required every frame
        update_game_state();

        // Pause on start
        if (get_key_pressed_down(J_START)){
            pause_game(); // Update text showing if paused
        }

        update_scene(); // Update game scene

        platform_vsync(); // Await frame end

    }
}

void pause_game(void)
{

    if(game.system.current_scene == SCENE_TITLE_SCREEN)
        return; // Do not pause on title screen

    game.system.paused = !game.system.paused;

    // Load upper case font
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);

    if (game.system.paused == 1){
        platform_hide_sprites();
        print_text("GAME PAUSED...",3 ,8);
    } else if (game.system.paused == 0) {
        // Clear text
        reload_active_background();
        platform_show_sprites();
    }
}
/* End of main.c */