/* 
 * File:   newsimpletest.cpp
 * Author: zhanhb
 *
 * Created on Aug 12, 2013, 8:22:07 PM
 */

#include "includes.h"
#include "BigInteger.h"
#include "Random.h"
#include "suite.h"

void testIntValue(const BigInteger & x) {
    int t = x.intValue(), y;
    stringstream os;
    os << x;
    os >> y;
    assert(y == t);
}

template<class A>
void testIntValue(const A & a) {
    testIntValue(BigInteger(a));
}

void testIntValue(int x) {
    testIntValue(BigInteger(x));
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
    testIntValue(INT_MAX);
    testIntValue(INT_MAX + 1);
    testIntValue(INT_MAX - 1);
    testIntValue(INT_MIN);
    testIntValue(INT_MIN - 1);
    testIntValue(INT_MIN + 1);
}

int main(int argc, char** argv) {
    TestStart();
    TestMethod(testIntRandom);
    TestMethod(testIntRange);
    TestMethod(testIntSpecial);
    TestEnd();
    return (EXIT_SUCCESS);
}

