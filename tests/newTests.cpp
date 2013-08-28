/* 
 * File:   newsimpletest.cpp
 * Author: zhanhb
 *
 * Created on Aug 15, 2013, 2:45:02 PM
 */

#include <stdlib.h>
#include <iostream>
#include "includes.h"
#include "BigInteger.h"
#include "Random.h"
#include "suite.h"

/*
 * Simple C++ Test Suite
 */
LL ABS(LL x) {
    return x > 0 ? x : -x;
}

void testGcd(LL a, LL b) {
    BigInteger x = a, y = b;
    assert(x.gcd(y) == ABS(__gcd(a, b)));
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

void testIntValue(const BigInteger & x) {
    int t = x.intValue(), y;
    stringstream os;
    os << x;
    os >> y;
    if (y != t) {
        out(y);
        out(t);
        assert(y == t);
    }
}

template<class A>
void testIntValue(const A & a) {
    testIntValue(BigInteger(a));
}

void testIntRandom() {
    Random random;
    for (int i = 0; i < 10000; ++i) {
        testIntValue(random.nextInt());
    }
}

void testIntRange() {
    for (int i = -1000; i <= 1000; ++i) {
        testIntValue(i);
    }
}

void testIntSpecial() {
    testIntValue(0);
    testIntValue(-1);
    testIntValue(-2);
    testIntValue(1);
    testIntValue(2);
    testIntValue(INT_MAX);
    testIntValue(INT_MAX - 1);
    testIntValue(INT_MAX - 2);

    testIntValue(INT_MIN);
    testIntValue(INT_MIN + 1);
    testIntValue(INT_MIN + 2);
    testIntValue(INT_MIN + 3);
}

void testSwap() {
    for (int i = -1000; i < 1000; ++i) {
        for (int j = -20; j < 20; ++j) {
            BigInteger a = i, b = j;
            std::swap(a, b);
            a.swap(b);
            if (a != i || b != j) {
                out(a);
                out(b);
                out(i);
                out(j);
                assert(false);
                assert(a == i);
                assert(b == j);
            }
        }
    }
}

int main(int argc, char** argv) {
    TestStart();
    TestMethod(testGcd);
    TestMethod(testGcdRandom);
    TestMethod(testSwap);
    TestMethod(testIntRandom);
    TestMethod(testIntRange);
    TestMethod(testIntSpecial);
    TestEnd();
    return EXIT_SUCCESS;
}

