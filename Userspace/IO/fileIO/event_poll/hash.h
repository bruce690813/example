#ifndef _HASH_H
#define _HASH_H

#ifndef u32
#define u32 unsigned int
#endif

typedef unsigned int        __u32;

/**
 * rol32 - rotate a 32-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u32 rol32(__u32 word, unsigned int shift)
{
    return (word << shift) | (word >> ((-shift) & 31));
}

/* __jhash_mix -- mix 3 32-bit values reversibly. */
#define __jhash_mix(a, b, c)            \
{                       \
    a -= c;  a ^= rol32(c, 4);  c += b; \
    b -= a;  b ^= rol32(a, 6);  a += c; \
    c -= b;  c ^= rol32(b, 8);  b += a; \
    a -= c;  a ^= rol32(c, 16); c += b; \
    b -= a;  b ^= rol32(a, 19); a += c; \
    c -= b;  c ^= rol32(b, 4);  b += a; \
}

/* __jhash_final - final mixing of 3 32-bit values (a,b,c) into c */
#define __jhash_final(a, b, c)          \
{                       \
    c ^= b; c -= rol32(b, 14);      \
    a ^= c; a -= rol32(c, 11);      \
    b ^= a; b -= rol32(a, 25);      \
    c ^= b; c -= rol32(b, 16);      \
    a ^= c; a -= rol32(c, 4);       \
    b ^= a; b -= rol32(a, 14);      \
    c ^= b; c -= rol32(b, 24);      \
}

/* An arbitrary initial parameter */
#define JHASH_INITVAL       0xdeadbeef

/* jhash2 - hash an array of u32's
 * @k: the key which must be an array of u32's
 * @length: the number of u32's in the key
 * @initval: the previous hash, or an arbitray value
 *
 * Returns the hash value of the key.
 */
static inline u32 jhash2(const u32 *k, u32 length, u32 initval)
{
        u32 a, b, c;

        /* Set up the internal state */
        a = b = c = JHASH_INITVAL + (length<<2) + initval;

        /* Handle most of the key */
        while (length > 3) {
                a += k[0];
                b += k[1];
                c += k[2];
                __jhash_mix(a, b, c);
                length -= 3;
                k += 3;
        }

        /* Handle the last 3 u32's: all the case statements fall through */
        switch (length) {
        case 3: c += k[2];
        case 2: b += k[1];
        case 1: a += k[0];
                __jhash_final(a, b, c);
        case 0: /* Nothing left to add */
                break;
        }

        return c;
}


#endif /* _HASH_H */
