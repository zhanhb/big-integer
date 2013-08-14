/*
 * File:   testGcd.cpp
 * Author: zhanhb
 *
 * Created on Aug 14, 2013, 8:11:51 PM
 */

#include "includes.h"
#include "BigInteger.h"
#include "Random.h"
#include "suite.h"
#if defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#define LL __int64
#define LLS "%" "I" "6" "4" "d"
#else
#define LL long long
#define LLS "%" "l" "l" "d"
#endif
using namespace std;

LL ABS(LL x) {
    return x > 0 ? x : -x;
}

LL gcd(LL a, LL b) {
    return __gcd(a, b);
}

void testGcd(LL a, LL b) {
    BigInteger x = a, y = b;
    assert(x.gcd(y) == ABS(gcd(a, b)));
}

void testGcd() {
    for (int i = -20; i <= 20; ++i) {
        for (int j = -20; j <= 20; ++j) {
            testGcd(i, j);
        }
    }
}

void testGcdRandom() {
    Random ram;
    srand(time(NULL));
    for (int i = 0; i < 10000; ++i) {
        testGcd(ram.nextLong(), ram.nextLong());
        testGcd(ram.nextInt(), ram.nextInt());
    }
}

/*
 * Simple C++ Test Suite
 */

int main(int argc, char** argv) {
    TestStart();
    TestMethod(testGcd);
    TestMethod(testGcdRandom);
    TestEnd();
    return EXIT_SUCCESS;
}

