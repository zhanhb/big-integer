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

LL doubleToRawLongBits(double value) {
    return *((LL*) & value);
}

LL doubleToLongBits(double value) {
    LL result = doubleToRawLongBits(value);
    // Check for NaN based on values of bit fields, maximum
    // exponent and nonzero significand.
    if (((result & 0x7ff0000000000000LL) ==
            0x7ff0000000000000LL) &&
            (result & 0x000fffffffffffffLL) != 0L)
        result = 0x7ff8000000000000LL;
    return result;
}

double longBitsToDouble(LL bits) {
    return *((double*) &bits);
}

void testToDouble(const BigInteger & big) {
    double dbVal1 = big.doubleValue(), dbVal2;
    stringstream os;
    os << big;
    os >> dbVal2;

    assert(doubleToLongBits(dbVal1 + 0.0) == doubleToLongBits(dbVal1));
    if (doubleToLongBits(dbVal1) != doubleToLongBits(dbVal2)) {
        out(doubleToLongBits(dbVal1));
        out(doubleToLongBits(dbVal2));
        out(dbVal1);
        out(dbVal2);
        out(big.toString(10));
        out(big.toString(16));
        assert(doubleToLongBits(dbVal1) == doubleToLongBits(dbVal2));
    }
}

template<typename cl>
void testToDouble0(const cl & xxxxx) {
    testToDouble(BigInteger(xxxxx));
}

template<typename A, typename B>
void testToDouble(const A & a, const B & b) {
    testToDouble(BigInteger(a, b));
}

void testDoubleValue() {
    BigInteger x = 1;
    for (int i = 1; i <= 180; ++i) {
        x *= i;
        testToDouble(x);
        testToDouble(-x);
    }
    for (int cas = 100000; cas--;) {
        LL xxxxx = myrandom.nextLong();
        testToDouble(xxxxx);
    }
}

void tmpMethod() {
    testToDouble("3a1d5e4bb884258", 16);
    testToDouble("2b2a188a0624db00", 16);
    testToDouble("-2b2a188a0624db00", 16);
    testToDouble("15511210043330985984000000");
    testToDouble("15511210043330983907819520");
    testToDouble("15511210043330986055303168");
    testToDouble(0);
    testToDouble(1);
    testToDouble(-1);
    testToDouble(20);
    testToDouble("1124000727777607680000");
}

void testDoubleMax() {
    double dbVal = 0.0;
    dbVal = 1 / dbVal;
    dbVal = longBitsToDouble(doubleToLongBits(dbVal) - 1);

    stringstream stream;
    stream << fixed << setprecision(0) << dbVal;
    BigInteger a, b;
    stream >> a;

    dbVal = longBitsToDouble(doubleToLongBits(dbVal) - 2);
    stream << dbVal;
    stream >> b;
    BigInteger tmp = a - b >> 5;
    for (int i = 0; i < 60; ++i) {
        testToDouble(b += tmp);
        testToDouble(-b);
    }
}

int main(int argc, char** argv) {
    cout << fixed << setprecision(0);
    TestStart();
    TestMethod(testDoubleValue);
    TestMethod(tmpMethod);
    TestMethod(testDoubleMax);
    TestEnd();
    return (EXIT_SUCCESS);
}

