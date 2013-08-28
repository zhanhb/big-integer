#define _GLIBCXX_DEBUG
#define _BIG_INTEGER_DEBUG

/*
 * File:   newsimpletest.cpp
 * Author: zhanhb
 *
 * Created on Aug 11, 2013, 12:16:44 PM
 */

#include "includes.h"
#include "BigInteger.h"
#include "Random.h"
#include "suite.h"

/*
 * Simple C++ Test Suite
 */

template<typename IntClass>
void testCompile0(IntClass i) {
    IntClass t, *p, y;
    bool b;
    t = ++i;
    t = --i;
    t = ~i;
    b = !i;
    t = -i;
    t = +i;
    p = &i;
    p = new IntClass;
    delete p;
    p = new IntClass[20000];
    delete[] p;

    t = i * i;
    if (i != 0) {
        t = i / i;
        y = i % i;
        assert(t == 1);
        assert(y == 0);
    }
    t = i + i;
    t = i - i;
    assert(!t);
    t = i << 2000;
    t = i >> 2000;

    b = i <i;
    assert(!b);
    b = i <= i;
    assert(b);
    b = i> i;
    assert(!b);
    b = i >= i;
    assert(b);

    b = i == i;
    assert(b);
    b = i != i;

    t = i & i;
    t = i ^ i;
    t = i | i;

    t = i;
    t *= i;
    if (i != 0) {
        t /= i;
        t %= i;
    }
    t += i;
    t -= i;
    i <<= 2000;
    i >>= 2000;
    t &= i;
    t |= i;
    t ^= i;
}

void testCompile() {
    char ssssss[20] = "123456";
    BigInteger t(ssssss);
    BigInteger x, y(x), z(3), w("abc", 14), r((LL) 124), s(string("gth"), 36);
    testCompile0(x);
    x.assign("abc", 16);
    x.assign(string("abc"), 20);
    x.compareTo(y);
    x.doubleValue();
    string str;
    x.getChars(str);
    !x;
    +x;
    -x;
    ~x;

    ++x;
    x++;
    --x;
    x--;
    x += y;
    x -= z;
    x *= r;
    y /= s;
    x <<= 4;
    z >>= 1;
    z.toString(14);
    z.toString();

    x = x + y;
    z = x & y;
    s = s | w;
    w = w ^ 1;
    y ^= 1;
    z.abs();
    double d = y.doubleValue();
}

void test0() {
    signed char ch = SCHAR_MIN;
    for (;; ++ch) {
        BigInteger x(ch), a = 0 + +x + 0, b = 0 - -x - 0;
        assert(a == x && a == b && x == b);
        if (ch == SCHAR_MAX)break;
    }
    int val = INT_MIN;
    for (int testNo = 0; testNo < 10000; ++val, ++testNo) {
        BigInteger x = val, a = 0 + +x + 0, b = 0 - -x - 0;
        assert(a == x && a == b && x == b);
    }
}

void testAdd() {
    for (int i = -257; i <= 257; ++i) {
        for (int j = -257; j <= 257; ++j) {
            BigInteger ii = i, jj = j;
            int ti = i, tj = j;
            ti += tj;
            ii += jj;
            if (ti != ii) {
                out(ti);
                out(ii);
                out(jj);
                assert(BigInteger(ti) == ii);
            }
        }
    }
}

void testSelfAdd() {
    short x = SHRT_MIN;
    BigInteger y = x;
    for (;; ++x, ++y) {
        assert(BigInteger(x) == y);
        if (x == SHRT_MAX)break;
    }
    int i = -500000;
    y = i;
    for (; i <= 500000; ++i, ++y) {
        assert(BigInteger(i) == y);
    }
}

void testCompare() {
    int i;
    Random random;
    for (i = -65537; i <= 65537; ++i) {
        for (int k = 0; k < 5; ++k) {
            int j = random.nextInt(32768) - 16384;
            BigInteger ii = i, jj = j;
            assert((ii < jj) == (i < j));
            assert((ii > jj) == (i > j));
            assert((ii <= jj) == (i <= j));
            assert((ii >= jj) == (i >= j));
        }
    }
    for (i = -10000; i <= 10000; ++i) {
        BigInteger x = i;
        assert(x + 1 == i + 1);
        assert(x - 1 == i - 1);
        assert(x + x == i * 2);
        assert(x - x == 0);
        assert(x - (x - 1) == 1);
        assert((x - 1) - x == -1);
    }
}

void testAddOne() {
    int x;
    BigInteger y;

    y = x = -100000;
    for (; x <= 100000; ++x) {
        assert(BigInteger(x) == y);
        y += 1;
    }

    y = x = -100000;
    for (; x <= 100000; ++x) {
        assert(BigInteger(x) == y);
        y = y + 1;
    }
}

void testUnaryFunction() {
    for (int i = -70000; i <= 70000; ++i) {
        int x = i;
        BigInteger y = x;

        assert(!x == !y);
        assert(-x == -y);
        assert(!x == !y);
        assert(~x == ~y);
        assert(x++ == y++);
        assert(x-- == y--);
        assert(++x == ++y);
        assert(--x == --y);
    }
}

void testShift0(int val, int k) {
    assert(BigInteger(val) << k == val << k);
    assert(BigInteger(val) >> k == val >> k);
}

void testShift() {
    for (int i = 0; i < 10; ++i) {
        for (int j = -3; j <= 3; ++j) {
            for (int sign = -1; sign <= 1; sign += 2) {
                int x = ((1 << i) + j) * sign;
                for (int k = 0; k < 10; ++k) {
                    testShift0(x, k);
                }
            }
        }
    }
}

void testCoutWithFlag() {
    ;
}

void testCinWithFlag() {
    ;
}

int main(int argc, char** argv) {
    TestStart();
    TestMethod(testCompile);
    TestMethod(test0);
    TestMethod(testAdd);
    TestMethod(testSelfAdd);
    TestMethod(testCompare);
    TestMethod(testAddOne);
    TestMethod(testUnaryFunction);
    TestMethod(testShift);
    TestEnd();
    return (EXIT_SUCCESS);
}

