#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdint.h>

#define min(a, b)               \
    __extension__ ({            \
        __auto_type _a = (a);   \
        __auto_type _b = (b);   \
        _a < _b ? _a : _b;      \
    })

#define max(a, b)               \
    __extension__ ({            \
        __auto_type _a = (a);   \
        __auto_type _b = (b);   \
        _a > _b ? _a : _b;      \
    })

#define bit(type, bit_pos) ((type) (1ULL << bit_pos))
#define mask_bits(type, bit_count) ((type) (bit(unsigned long long, bit_count) - 1))

#define modulo(a, n)                            \
    __extension__ ({                            \
        __auto_type _a = (a);                   \
        __auto_type _n = (n);                   \
        __auto_type _m = _a % _n;               \
        if (_m < 0) {                           \
            _m = (_n < 0) ? _m - _n : _m + _n;  \
        }                                       \
        _m;                                     \
    })
#define modulo_neg(a, n)                        \
    __extension__ ({                            \
        __auto_type _a = (a);                   \
        __auto_type _n = (n);                   \
        __auto_type _m = _a % _n;               \
        if (_m > 0) {                           \
            _m = (_n > 0) ? _m - _n : _m + _n;  \
        }                                       \
        _m;                                     \
    })

#define round_down(val, quantum)            \
    __extension__ ({                        \
        __auto_type _val = (val);           \
        __auto_type _quantum = (quantum);   \
        _val - modulo(_val, _quantum);      \
    })
#define round_up(val, quantum)              \
    __extension__ ({                        \
        __auto_type _val = (val);           \
        __auto_type _quantum = (quantum);   \
        _val += (_quantum - 1);             \
        _val - modulo(_val, _quantum);      \
    })
// Prefers rounding to +infinity
#define round_p(val, quantum)               \
    __extension__ ({                        \
        __auto_type _val = (val);           \
        __auto_type _quantum = (quantum);   \
        _val += (_quantum / 2);             \
        _val - modulo(_val, _quantum);      \
    })
// Prefers rounding to -infinity
#define round_n(val, quantum)               \
    __extension__ ({                        \
        __auto_type _val = (val);           \
        __auto_type _quantum = (quantum);   \
        _val -= (_quantum / 2);             \
        _val - modulo_neg(_val, _quantum);  \
    })

static inline uint8_t bswap_u8(uint8_t value) {
    return value;
}
static inline int8_t bswap_s8(int8_t value) {
    return bswap_u8(value);
}
static inline uint16_t bswap_u16(uint16_t value) {
    value = (value >> 8) |
            (value << 8);
    return value;
}
static inline int16_t bswap_s16(int16_t value) {
    return bswap_u16(value);
}
static inline uint32_t bswap_u32(uint32_t value) {
    value = ((value >> 8) & 0x00FF00FFUL) |
            ((value << 8) & 0xFF00FF00UL);
    value = (value >> 16) |
            (value << 16);
    return value;
}
static inline int32_t bswap_s32(int32_t value) {
    return bswap_u32(value);
}
static inline uint64_t bswap_u64(uint64_t value) {
    value = ((value >> 8) & 0x00FF00FF00FF00FFULL) |
            ((value << 8) & 0xFF00FF00FF00FF00ULL);
    value = ((value >> 16) & 0x0000FFFF0000FFFFULL) |
            ((value << 16) & 0xFFFF0000FFFF0000ULL);
    value = (value >> 32) |
            (value << 32);
    return value;
}
static inline int64_t bswap_s64(int64_t value) {
    return bswap_u64(value);
}

#if !defined __BYTE_ORDER__

#error Requires __BYTE_ORDER__

#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define to_little_u8(value) ((uint8_t) (value))
#define to_little_s8(value) ((int8_t) (value))
#define to_little_u16(value) ((uint16_t) (value))
#define to_little_s16(value) ((int16_t) (value))
#define to_little_u32(value) ((uint32_t) (value))
#define to_little_s32(value) ((int32_t) (value))
#define to_little_u64(value) ((uint64_t) (value))
#define to_little_s64(value) ((int64_t) (value))

#define from_little_u8(value) ((uint8_t) (value))
#define from_little_s8(value) ((int8_t) (value))
#define from_little_u16(value) ((uint16_t) (value))
#define from_little_s16(value) ((int16_t) (value))
#define from_little_u32(value) ((uint32_t) (value))
#define from_little_s32(value) ((int32_t) (value))
#define from_little_u64(value) ((uint64_t) (value))
#define from_little_s64(value) ((int64_t) (value))

#define to_big_u8(value) (bswap_u8((value)))
#define to_big_s8(value) (bswap_s8((value)))
#define to_big_u16(value) (bswap_u16((value)))
#define to_big_s16(value) (bswap_s16((value)))
#define to_big_u32(value) (bswap_u32((value)))
#define to_big_s32(value) (bswap_s32((value)))
#define to_big_u64(value) (bswap_u64((value)))
#define to_big_s64(value) (bswap_s64((value)))

#define from_big_u8(value) (bswap_u8((value)))
#define from_big_s8(value) (bswap_s8((value)))
#define from_big_u16(value) (bswap_u16((value)))
#define from_big_s16(value) (bswap_s16((value)))
#define from_big_u32(value) (bswap_u32((value)))
#define from_big_s32(value) (bswap_s32((value)))
#define from_big_u64(value) (bswap_u64((value)))
#define from_big_s64(value) (bswap_s64((value)))

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define to_little_u8(value) (bswap_u8((value)))
#define to_little_s8(value) (bswap_s8((value)))
#define to_little_u16(value) (bswap_u16((value)))
#define to_little_s16(value) (bswap_s16((value)))
#define to_little_u32(value) (bswap_u32((value)))
#define to_little_s32(value) (bswap_s32((value)))
#define to_little_u64(value) (bswap_u64((value)))
#define to_little_s64(value) (bswap_s64((value)))

#define from_little_u8(value) (bswap_u8((value)))
#define from_little_s8(value) (bswap_s8((value)))
#define from_little_u16(value) (bswap_u16((value)))
#define from_little_s16(value) (bswap_s16((value)))
#define from_little_u32(value) (bswap_u32((value)))
#define from_little_s32(value) (bswap_s32((value)))
#define from_little_u64(value) (bswap_u64((value)))
#define from_little_s64(value) (bswap_s64((value)))

#define to_big_u8(value) ((uint8_t) (value))
#define to_big_s8(value) ((int8_t) (value))
#define to_big_u16(value) ((uint16_t) (value))
#define to_big_s16(value) ((int16_t) (value))
#define to_big_u32(value) ((uint32_t) (value))
#define to_big_s32(value) ((int32_t) (value))
#define to_big_u64(value) ((uint64_t) (value))
#define to_big_s64(value) ((int64_t) (value))

#define from_big_u8(value) ((uint8_t) (value))
#define from_big_s8(value) ((int8_t) (value))
#define from_big_u16(value) ((uint16_t) (value))
#define from_big_s16(value) ((int16_t) (value))
#define from_big_u32(value) ((uint32_t) (value))
#define from_big_s32(value) ((int32_t) (value))
#define from_big_u64(value) ((uint64_t) (value))
#define from_big_s64(value) ((int64_t) (value))

#else

#error Unrecognised endiannes

#endif

static inline uint8_t breverse_u8(uint8_t value) {
    value = ((value >> 1) & 0x55U) |
            ((value << 1) & 0xAAU);
    value = ((value >> 2) & 0x33U) |
            ((value << 2) & 0xCCU);
    value = (value >> 4) |
            (value << 4);
    return value;
}
static inline int8_t breverse_s8(int8_t value) {
    return breverse_u8(value);
}
static inline uint16_t breverse_u16(uint16_t value) {
    value = ((value >> 1) & 0x5555U) |
            ((value << 1) & 0xAAAAU);
    value = ((value >> 2) & 0x3333U) |
            ((value << 2) & 0xCCCCU);
    value = ((value >> 4) & 0x0F0FU) |
            ((value << 4) & 0xF0F0U);
    value = bswap_u16(value);
    return value;
}
static inline int16_t breverse_s16(int16_t value) {
    return breverse_u16(value);
}
static inline uint32_t breverse_u32(uint32_t value) {
    value = ((value >> 1) & 0x55555555UL) |
            ((value << 1) & 0xAAAAAAAAUL);
    value = ((value >> 2) & 0x33333333UL) |
            ((value << 2) & 0xCCCCCCCCUL);
    value = ((value >> 4) & 0x0F0F0F0FUL) |
            ((value << 4) & 0xF0F0F0F0UL);
    value = bswap_u32(value);
    return value;
}
static inline int32_t breverse_s32(int32_t value) {
    return breverse_u32(value);
}
static inline uint64_t breverse_u64(uint64_t value) {
    value = ((value >> 1) & 0x5555555555555555ULL) |
            ((value << 1) & 0xAAAAAAAAAAAAAAAAULL);
    value = ((value >> 2) & 0x3333333333333333ULL) |
            ((value << 2) & 0xCCCCCCCCCCCCCCCCULL);
    value = ((value >> 4) & 0x0F0F0F0F0F0F0F0FULL) |
            ((value << 4) & 0xF0F0F0F0F0F0F0F0ULL);
    value = bswap_u64(value);
    return value;
}
static inline int64_t breverse_s64(int64_t value) {
    return breverse_u64(value);
}

#endif
