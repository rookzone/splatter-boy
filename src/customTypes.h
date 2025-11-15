// types.h

#include <gb/gb.h>

#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

typedef struct GameSprite GameSprite;

typedef int16_t fixed_n;   

#define FIXED_SHIFT   8           // number of fractional bits
#define FIXED_ONE     (1 << FIXED_SHIFT)   // 1.0 in fixed (256)
#define FIXED_HALF    (1 << (FIXED_SHIFT - 1)) // 0.5 (128)
#define FIXED_QUARTER (1 << (FIXED_SHIFT - 2)) // 0.25 (64)
#define FIXED_EIGHTH  (1 << (FIXED_SHIFT - 3)) // 0.125 (32)
#define FIXED_TEENTH  (1 << (FIXED_SHIFT - 4)) // (16)
#define FIXED_TENBAG  (1 << (FIXED_SHIFT - 5)) // (8)


// Conversion helpers
#define TO_FIXED(x)   ((fixed_n)((x) << FIXED_SHIFT))       // int -> fixed
#define FROM_FIXED(x) ((x) >> FIXED_SHIFT)                // fixed -> int
#define FIXED_ADD(a,b) ((a) + (b))
#define FIXED_SUB(a,b) ((a) - (b))

// Optional multiply/divide (integer-safe, not float)
#define FIXED_MUL(a,b) (((a) * (b)) >> FIXED_SHIFT)
#define FIXED_DIV(a,b) (((a) << FIXED_SHIFT) / (b))


#endif