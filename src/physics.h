// physics.h

#ifndef PHYSICS_H
#define PHYSICS_H

#include <gb/gb.h>
#include <stdint.h>


// ---------- Fixed-point setup (8.8) ----------

typedef int16_t fixed;            // 16-bit fixed-point number
#define FIXED_SHIFT   8           // number of fractional bits
#define FIXED_ONE     (1 << FIXED_SHIFT)   // 1.0 in fixed (256)
#define FIXED_HALF    (1 << (FIXED_SHIFT - 1)) // 0.5 (128)
#define FIXED_QUARTER (1 << (FIXED_SHIFT - 2)) // 0.25 (64)
#define FIXED_EIGHTH  (1 << (FIXED_SHIFT - 3)) // 0.125 (32)

// Conversion helpers
#define TO_FIXED(x)   ((fixed)((x) << FIXED_SHIFT))       // int → fixed
#define FROM_FIXED(x) ((x) >> FIXED_SHIFT)                // fixed → int
#define FIXED_ADD(a,b) ((a) + (b))
#define FIXED_SUB(a,b) ((a) - (b))

// Optional multiply/divide (integer-safe, not float)
#define FIXED_MUL(a,b) (((a) * (b)) >> FIXED_SHIFT)
#define FIXED_DIV(a,b) (((a) << FIXED_SHIFT) / (b))


// --- Physics constants (8.8 fixed-point) ---
#define GRAVITY     FIXED_EIGHTH;      // 0.125 pixels/frame
#define MAX_SPEED   2560    // 10 pixels/frame max
#define DAMPING     4       // bounce damping

// --- Data types ---
typedef struct {
    fixed x, y;   // position in 8.8 fixed-point
    fixed  vx, vy; // velocity in 8.8 fixed-point
} Ball;

typedef struct {
    uint8_t x, y;       // top-left in pixels
    uint8_t width, height;
} Wall;

// --- Function prototypes ---
void apply_gravity(Ball *b);
void apply_gravity_multi(Ball *b, uint8_t count);

void check_ball_wall(Ball *b, Wall *w);
void check_ball_wall_multi(Ball *b, Wall *w, uint8_t count);

void apply_impulse(Ball *b, uint8_t impulse);

// Optionally uncomment if you bring init_balls() back later
// void init_balls(Ball *b, uint8_t count);

#endif // PHYSICS_H
