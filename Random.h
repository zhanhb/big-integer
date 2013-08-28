/* 
 * File:   Random.h
 * Author: zhanhb
 *
 * Created on August 11, 2013, 6:06 PM
 */

#ifndef RANDOM_H
#define	RANDOM_H

#include <ctime>
#include <exception>

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

namespace java {
    namespace util {

        class IllegalArgumentException : public std::exception {
            const char * message;
        public:

            IllegalArgumentException(const char * what) : message(what) { }

            const char * what() const throw () {
                return message;
            }
        };

        class Random {
            const static LL multiplier = 0x5DEECE66DLL;
            const static LL addend = 0xBLL;
            const static LL mask = (1LL << 48) - 1;
            LL seed;
        public:

            Random() {
                setSeed(++seedUniquifier + std::clock());
            }
        private:
            static volatile LL seedUniquifier;
        public:

            Random(LL seed) {
                setSeed(seed);
            }

            void setSeed(LL seed) {
                seed = (seed ^ multiplier) & mask;
                this->seed = seed;
                haveNextNextGaussian = false;
            }
        protected:

            int next(int bits) {
                LL &seed = this->seed;
                LL nextseed = (seed * multiplier + addend) & mask;
                seed = nextseed;
                return (int) (nextseed >> (48 - bits));
            }
        public:
            typedef char byte;

            void nextBytes(byte * bytes, int len) {
                for (int i = 0; i < len;) {
                    int rnd = nextInt();
                    int n = len - i, x = sizeof (int) / sizeof (byte);
                    if (n > x)n = x;
                    for (; n--; rnd >>= sizeof (byte))
                        bytes[i++] = (byte) rnd;
                }
            }

            inline int nextInt() {
                return next(32);
            }

            int nextInt(int n) {
                if (n <= 0)
                    throw IllegalArgumentException("n must be positive");

                if ((n & -n) == n) // i.e., n is a power of 2
                    return (int) ((n * (LL) next(31)) >> 31);

                int bits, val;
                do {
                    bits = next(31);
                    val = bits % n;
                } while (bits - val + (n - 1) < 0);
                return val;
            }

            LL nextLong() {
                // it's okay that the bottom word remains signed.
                return ((LL) (next(32)) << 32) +next(32);
            }

            bool nextBoolean() {
                return next(1) != 0;
            }

            float nextFloat() {
                return next(24) / ((float) (1 << 24));
            }

            double nextDouble() {
                return (((LL) (next(26)) << 27) + next(27))
                        / (double) (1LL << 53);
            }
        private:
            double nextNextGaussian;
            bool haveNextNextGaussian;

            double nextGaussian() {
                // See Knuth, ACP, Section 3.4.1 Algorithm C.
                if (haveNextNextGaussian) {
                    haveNextNextGaussian = false;
                    return nextNextGaussian;
                } else {
                    double v1, v2, s;
                    do {
                        v1 = 2 * nextDouble() - 1; // between -1 and 1
                        v2 = 2 * nextDouble() - 1; // between -1 and 1
                        s = v1 * v1 + v2 * v2;
                    } while (s >= 1 || s == 0);
                    double multiplier = std::sqrt(-2 * std::log(s) / s);
                    nextNextGaussian = v2 * multiplier;
                    haveNextNextGaussian = true;
                    return v1 * multiplier;
                }
            }
            static const LL seedOffset;
        };
        volatile LL Random::seedUniquifier = 8682522807148012LL;
    }
}

using ::java::util::Random;

#ifdef __CLEARLL
#undef LL
#undef __CLEARLL
#endif

#endif	/* RANDOM_H */

