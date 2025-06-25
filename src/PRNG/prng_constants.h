#ifndef PRNG_CONSTANTS_H
#define PRNG_CONSTANTS_H

#define PRNG_ENGINE_COUNT   5

const char prng_to_string[PRNG_ENGINE_COUNT][31] = {
    "Marsaglia's MWC",
    "XORShift128**",
    "Standard C99",
    "PCG32",
    "SplitMix64"
};

/* ===================== *
 * Seed Generation Constants
 * ===================== */

/* Seed Hashing */
#define SEED_HASH_K1      0xff51afd7ed558ccdULL  // MurmurHash3 finalizer constant 1
#define SEED_HASH_K2      0xc4ceb9fe1a85ec53ULL  // MurmurHash3 finalizer constant 2
#define SEED_HASH_SHIFT   33                     // Avalanche effect rotation bits
#define GOLDEN_RATIO_64   0x9E3779B97F4A7C15ULL  // 64 bit golden ration
#define GOLDEN_RATIO_32   0x9E3779B9UL           // 32 bit golden ration

/* Default Seed Components */
#define SEED_GOLDEN_RATIO 0x9E3779B9UL           // Fractional part of (sqrt(5)-1)/2
#define SEED_TIME_SHIFT   32                     // Bitshift for timestamp in default seed
#define SEED_CLOCK_MASK   0xFFFF                 // Mask for clock bits in default seed



/* Engine-Specific Constraints */
#define SEED_MWC_MIN      1UL                    // Marsaglia MWC minimum seed value
#define SEED_XSHIFT_MASK  0xF                    // XORShift low-entropy bit mask

/* Warm-up Defaults */
#define WARMUP_DEFAULT    16                     // Recommended minimum warm-up rounds
#define WARMUP_MAX        1024                   // Safety limit for warm-up rounds

/* ===================== *
 * Engine-Specific Constants
 * ===================== */

// Marsaglia MWC
#define MWC_MULTIPLIER_A  36969U
#define MWC_MULTIPLIER_B  18000U
#define MWC_MULTIPLIER_C  29000U
#define MWC_MULTIPLIER_D  30970U
#define MWC_DEFAULT_SEED  1U     // Default seed if 0 provided

// XORShift128
#define XORSHIFT_INIT_SEED  0xBADF00D
#define XORSHIFT_SEED_STEP  0x11111111
#define XORSHIFT_SHIFT_1    11U
#define XORSHIFT_SHIFT_2    8U
#define XORSHIFT_SHIFT_3    19U

// PCG32
#define PCG_MULTIPLIER      6364136223846793005ULL
#define PCG_INCREMENT       1442695040888963407ULL  // Default stream selector
#define PCG_ROTATE_BITS     59U
#define PCG_XSHIFT_BITS     27U
#define PCG_OUTPUT_BITS     31U

// SplitMix
#define SPLITMIX_INCREMENT  0x9e3779b97f4a7c15ULL
#define SPLITMIX_MULT_1     0xbf58476d1ce4e5b9ULL
#define SPLITMIX_MULT_2     0x94d049bb133111ebULL
#define SPLITMIX_SHIFT_1    30U
#define SPLITMIX_SHIFT_2    27U

// Float generation
#define FLOAT_2POW32        4294967296.0
#define FLOAT_INV_2POW32    1.0 / FLOAT_2POW32

#endif
