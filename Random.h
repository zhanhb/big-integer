/* 
 * File:   Random.h
 * Author: zhanhb
 *
 * Created on August 11, 2013, 6:06 PM
 */

#ifndef RANDOM_H
#define	RANDOM_H

#include <cstdlib>

#ifndef LL
#define __CLEARLL
#if defined(__int64) || defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#define LL __int64
#define LLS "%" "I" "6" "4" "d"
#else
#define LL long long
#define LLS "%" "l" "l" "d"
#endif
#endif

struct Random {

    int nextInt() const {
        return ((std::rand() & 0x7ff) << 21) | ((std::rand() & 0x7ff) << 10) | (std::rand() & 0x3ff);
    }

    LL nextLong() const {
        return ((LL) nextInt() << 32) +nextInt();
    }

    int nextInt(int n) const {
        if (n <= 0)return 0;
        if ((n & -n) == n) // i.e., n is a power of 2
            return (int) ((n * (LL) next(31)) >> 31);
        int bits, val;
        do {
            bits = next(31);
            val = bits % n;
        } while (bits - val + (n - 1) < 0);
        return val;
    }

    bool nextBoolean() const {
        return !!next(1);
    }
private:

    int next(int bits) const {
        return nextInt() & ((1 << bits) - 1);
    }
};

#ifdef __CLEARLL
#undef LL
#undef __CLEARLL
#endif

#endif	/* RANDOM_H */

