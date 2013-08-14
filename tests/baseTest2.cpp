#include "includes.h"
#include "BigInteger.h"
#include "suite.h"
#include "Random.h"

Random myrandom;

template<class IntClass>
void testCalc0(const IntClass & x, const IntClass & y) {
    BigInteger xx(x), yy(y);

    xx += yy;
    assert(x + y == xx);

    xx = x;
    xx -= yy;
    assert(x - y == xx);

    xx = x;
    xx *= yy;
    assert(x * y == xx);

    if (y) {
        xx = x;
        xx /= yy;
        assert(x / y == xx);
    }

    xx = x;
    assert(xx + yy == x + y);
    assert(xx - yy == x - y);
    assert(xx * yy == x * y);

    if (y) {
        assert(xx / yy == x / y);
        if (xx % yy != x % y) {
            out(xx);
            out(yy);
            out(x);
            out(y);
            out(xx % yy);
            out(x % y);
            assert(xx % yy == x % y);
        }
    }
}

void testCalc() {
    for (int i = 0; i < 10; ++i) {
        for (int j = -3; j <= 3; ++j) {
            int x = (1 << i) + j;
            for (int k = 0; k < 10; ++k) {
                for (int l = -3; l <= 3; ++l) {
                    int y = (1 << k) + l;
                    testCalc0(x, y);
                }
            }
        }
    }
}

void testCalc2() {
    for (int cas = 500000; cas--;) {
        LL a = myrandom.nextInt(0x20000) - 0xFFFF;
        LL b = myrandom.nextInt(0x20000) - 0xFFFF;
        testCalc0(a, b);
    }
}

void testDivide0(LL a, LL b) {
    if (b) {
        assert(a / b == BigInteger(a) / b);
    }
}

void testDivide() {
    for (int i = 0; i < 1000; ++i) {
        int x = 65536 + myrandom.nextInt(65536);
        for (int j = 0; j < 100; ++j) {
            int y = myrandom.nextInt(65536) + 1;
            testDivide0(x, y);
        }
    }
    int cas = 1000000;
    for (int caseNo = 0; cas--; ++caseNo) {
        LL a = myrandom.nextLong(), b = myrandom.nextLong();
        testDivide0(a, b);
    }
}

void testToString10() {
    for (int i = 0; i < 500000; ++i) {
        LL x = myrandom.nextLong();
        BigInteger yy(x);

        ostringstream ssa, ssb;
        ssa << yy;
        ssb << x;
        string sa = ssa.str(), sb = ssb.str();
        assert(sa[0] && sa == sb);
    }
}

void testStringConstructor() {
#ifndef _BIG_INTEGER_NO_STRICT
    const char * zeroLength = "Zero length BigInteger", *illegal = "Illegal digit";
    const char *tests[] = {
        "", zeroLength,
        "-", zeroLength,
        "a", illegal,
        "++0", illegal,
        "0+0", illegal,
        "0-0", illegal,
        " ", illegal
    };
    for (int i = 0; i + 1 < sizeof tests / sizeof tests[0]; i += 2) {
        try {
            BigInteger x(tests[i]);
            assert(false);
        } catch (const BigInteger::ParseException & p) {
            assert(!strcmp(p.what(), tests[i + 1]));
        }
    }
#else
    const char *tests[] = {
        "", "0",
        "-", "0",
        "a", "0",
        "++0", "0",
        "1+0", "1",
        "0-0", "0",
        " ", "0",
        "77 ", "77"
    };
    for (int i = 0; i + 1 < sizeof tests / sizeof tests[0]; i += 2) {
        assert(BigInteger(tests[i]) == BigInteger(tests[i + 1]));
    }
#endif

    for (int radix = 2; radix <= 36; ++radix) {
        string s = "";
        char ch = radix > 10 ? (radix - 11 + 'a') : (radix + '0' - 1);
        for (int i = 0; i < 10000; ++i) {
            s += ch;
        }
        BigInteger x(s, radix);
    }
}

void testConstructor() {
    numeric_limits<bool>::max();
#define  __testConstructor(type) do {                           \
    type mx = numeric_limits<type>::max();                      \
    type mn = numeric_limits<type>::min();                      \
    ostringstream os; os << mx << ' ' << mn;                    \
    assert(os.str()[0]);                                        \
    BigInteger mmx(mx), mmn(mn);                                \
    string a = os.str();                                        \
    string b = mmx.toString() + " " + mmn.toString();           \
    assert(a == b);                                             \
} while(false)
    __testConstructor(bool);
    __testConstructor(wchar_t);
    __testConstructor(short);
    __testConstructor(unsigned short);
    __testConstructor(int);
    __testConstructor(unsigned int);
    __testConstructor(long);
    __testConstructor(unsigned long);
    __testConstructor(LL);
    __testConstructor(unsigned LL);
#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_INT128)
    __testConstructor(__int128);
    __testConstructor(unsigned __int128);
#endif
#undef __testConstructor

#define __testCharConstructor(type)  do {                       \
    type mx = numeric_limits<type>::max();                      \
    type mn = numeric_limits<type>::min();                      \
    ostringstream os; os << (LL) mx << ' ' << (LL) mn;          \
    assert(os.str()[0]);                                        \
    BigInteger mmx(mx), mmn(mn);                                \
    string a = os.str();                                        \
    string b = mmx.toString() + " " + mmn.toString();           \
    if(a != b) {                                                \
        out(#type);                                             \
        out(a);                                                 \
        out(b);                                                 \
        assert(false);                                          \
        assert(a == b);                                         \
    }                                                           \
} while(false)
    __testCharConstructor(char);
    __testCharConstructor(signed char);
    __testCharConstructor(unsigned char);
#undef __testCharConstructor
}

void testBitWise() {
    for (int i = -512; i <= 512; ++i) {
        for (int j = -512; j <= 512; ++j) {
            BigInteger x(i), y(j);
            assert((x & y) == (i & j));
            assert((x | y) == (i | j));
            assert((x ^ y) == (i ^ j));
        }
    }
    int cas = 100000;
    while (cas--) {
        int i = myrandom.nextInt(), j = myrandom.nextInt();
        BigInteger x(i), y(j);
        assert((x & y) == (i & j));
        assert((x | y) == (i | j));
        assert((x ^ y) == (i ^ j));
    }
}

void testInputString(const string & src) {
    istringstream a(src), b(src);
    int x = 999999;
    BigInteger y = 999999;
    a >> x;
    b >> y;
    if (x != y) {
        out(src);
        out(x);
        out(y);
        assert(x == y);
    }
    if (a.rdstate() != b.rdstate()) {
        out(src);
        out((int) a.rdstate());
        out((int) b.rdstate());
        assert(a.rdstate() == b.rdstate());
    }
    assert(a.exceptions() == b.exceptions());
    if (a.rdstate()) {
        a.clear();
        b.clear();
    }
    char ch1 = 'z', ch2 = 'z';
    a >> ch1;
    b >> ch2;
    if (ch1 != ch2) {
        out(ch1);
        out(ch2);
        assert(ch1 == ch2);
    }
}

void testInput() {
    const string ss[] = {
        "+-",
        "0123456789",
        ".",
        "abcdefghijklm",
        " \r\n\t"
    };
    testInputString("");
    for (int cas = 500000; cas--;) {
        int len = myrandom.nextInt(4) + 1;
        string src;
        for (int i = 0; i < len; ++i) {
            const string & t = ss[myrandom.nextInt(sizeof ss / sizeof ss[0])];
            src += t[myrandom.nextInt(t.length())];
        }
        testInputString(src);
    }
}

void testAbs() {
    for (int i = -10000; i <= 10000; ++i) {
        BigInteger x(i);
        BigInteger t = x.abs();
        assert(t >= 0 && (t == x || t == -x));
    }
}

int main(int argc, char** argv) {
    TestStart();
    TestMethod(testCalc);
    TestMethod(testCalc2);
    TestMethod(testDivide);
    TestMethod(testToString10);
    TestMethod(testStringConstructor);
    TestMethod(testConstructor);
    TestMethod(testBitWise);
    TestMethod(testInput);
    TestMethod(testAbs);
    TestEnd();
    return (EXIT_SUCCESS);
}

