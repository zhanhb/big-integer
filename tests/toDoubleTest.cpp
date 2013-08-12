#define _GLIBCXX_DEBUG
#define _BIG_INTEGER_DEBUG

/*
 * File:   testtodouble.cpp
 * Author: zhanhb
 *
 * Created on Aug 11, 2013, 5:56:34 PM
 */

/*
 * Simple C++ Test Suite
 */

#include "includes.h"
#include "BigInteger.h"
#include "suite.h"
#include "Random.h"

Random myrandom;

LL doubleBitsToLong(double d) {
    return *((LL*) & d);
}

double longBitsToDouble(LL v) {
    return *((double*) &v);
}

void testToDouble0(const BigInteger & big) {
    double dbVal1 = big.doubleValue(), dbVal2;
    stringstream os;
    os << big;
    os >> dbVal2;

    assert(doubleBitsToLong(dbVal1 + 0.0) == doubleBitsToLong(dbVal1));
    if (doubleBitsToLong(dbVal1) != doubleBitsToLong(dbVal2)) {
        out(doubleBitsToLong(dbVal1));
        out(doubleBitsToLong(dbVal2));
        out(dbVal1);
        out(dbVal2);
        out(big.toString(10));
        out(big.toString(16));
        assert(doubleBitsToLong(dbVal1) == doubleBitsToLong(dbVal2));
    }
    out(dbVal1);
}

template<typename cl>
void testToDouble0(const cl & xxxxx) {
    testToDouble0(BigInteger(xxxxx));
}

template<typename A, typename B>
void testToDouble0(const A & a, const B & b) {
    testToDouble0(BigInteger(a, b));
}

void testDoubleValue() {
    BigInteger x = 1;
    for (int i = 1; i <= 180; ++i) {
        x *= i;
        testToDouble0(x);
        testToDouble0(-x);
    }
    for (int cas = 100000; cas--;) {
        LL xxxxx = myrandom.nextLong();
        testToDouble0(xxxxx);
    }
}

void tmpMethod() {
    testToDouble0("3a1d5e4bb884258", 16);
    testToDouble0("2b2a188a0624db00", 16);
    testToDouble0("-2b2a188a0624db00", 16);
    testToDouble0("15511210043330985984000000");
    testToDouble0("15511210043330983907819520");
    testToDouble0("15511210043330986055303168");
    testToDouble0(0);
    testToDouble0(1);
    testToDouble0(-1);
    testToDouble0(20);
    testToDouble0("1124000727777607680000");
}

void testDoubleMax() {
    double dbVal = 0.0;
    dbVal = 1 / dbVal;
    dbVal = longBitsToDouble(doubleBitsToLong(dbVal) - 1);

    stringstream stream;
    stream << fixed << setprecision(0) << dbVal;
    BigInteger a, b;
    stream >> a;

    dbVal = longBitsToDouble(doubleBitsToLong(dbVal) - 1);
    stream << dbVal;
    stream >> b;
    BigInteger tmp = a - b >> 4;
    for (int i = 0; i < 40; ++i) {
        testToDouble0(b += tmp);
    }
}

int main(int argc, char** argv) {
    cout << hex;
    stringstream ss;
    ss << fixed << setprecision(0) << longBitsToDouble(0x4529a940c33f6120LL);

    cout << fixed << setprecision(0);
    TestStart();
    //    TestMethod(testDoubleValue);
    TestMethod(tmpMethod);
    TestMethod(testDoubleMax);
    TestEnd();
    return (EXIT_SUCCESS);
}

