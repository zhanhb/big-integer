#define _GLIBCXX_DEBUG

/*
 * File:   newsimpletest.cpp
 * Author: zhanhb
 *
 * Created on July 25, 2013, 10:37 AM
 */

// <editor-fold defaultstate="collapsed" desc="heads">
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#define MP make_pair
using namespace std;
using namespace std::rel_ops;
#if defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#define LL __int64
#define LLS "%" "I" "6" "4" "d"
#else
#define LL long long
#define LLS "%" "l" "l" "d"
#endif
const LL LL_MAX = ~((LL) ~0 << 63);

template<typename A> inline bool Max(A & a, const A & b) {
	return a < b ? a = b, true : false;
}

template<typename A> inline bool Min(A & a, const A & b) {
	return b < a ? a = b, true : false;
}

template<typename A, typename B> ostream & operator<<(ostream & out, const pair<A, B>&p) {
	return out << '[' << p.first << ',' << p.second << ']';
}

template<typename A> ostream & operator<<(ostream & out, const vector<A>&v) {
	typename vector<A>::const_iterator it = v.begin(), end = v.end();
	if (it == end)return out << "[]";
	out << '[' << *it;
	while (++it != end) {
		out << ',' << *it;
	}
	return out << ']';
}

template<typename A> ostream & operator<<(ostream & out, const set<A>&st) {
	typename set<A>::const_iterator it = st.begin(), end = st.end();
	if (it == end)return out << "[]";
	out << '[' << *it;
	while (++it != end) {
		out << ',' << *it;
	}
	return out << ']';
}

template<typename A> ostream & operator<<(ostream & out, const multiset<A>&st) {
	typename multiset<A>::const_iterator it = st.begin(), end = st.end();
	if (it == end)return out << "[]";
	out << '[' << *it;
	while (++it != end) {
		out << ',' << *it;
	}
	return out << ']';
}

template<typename Tp>
std::string toString(const Tp & val) {
	stringstream ss;
	ss << val;
	return ss.str();
}

template<> ostream & operator<<(ostream & out, const vector<unsigned char>&v) {
	return out << vector<int>(v.begin(), v.end());
}

template<> ostream & operator<<(ostream & out, const vector<char>&v) {
	vector<int>tmp(v.size());
	for (int i = 0, size = tmp.size(); i < size; ++i) {
		tmp[i] = v[i] & 0xff;
	}
	return out << tmp;
}

template<> ostream & operator<<(ostream & out, const vector<signed char>&v) {
	vector<int>tmp(v.size());
	for (int i = 0, size = tmp.size(); i < size; ++i) {
		tmp[i] = v[i] & 0xff;
	}
	return out << tmp;
}
// </editor-fold>

/*
 * Simple C++ Test Suite
 */

#define out(x) (std::cerr <<std::boolalpha <<"Line " <<__LINE__ <<": " <<#x <<": " <<(x) <<std::endl)
#define _BIG_INTEGER_DEBUG
#include "BigInteger.h"

struct Random {

	int nextInt() {
		return ((rand() & 0x7ff) << 21) | ((rand() & 0x7ff) << 10) | (rand() & 0x3ff);
	}

	LL nextLong() {
		return ((LL) nextInt() << 32) + nextInt();
	}

	int nextInt(int n) {
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

	bool nextBoolean() {
		return !!next(1);
	}
private:

	int next(int bits) {
		return nextInt() & ((1 << bits) - 1);
	}
} random;

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
	BigInteger x;
	testCompile0(x);
}

// <editor-fold defaultstate="collapsed" desc="passed tests">

void test0() {
	signed char ch = SCHAR_MIN;
	for (;; ++ch) {
		BigInteger x(ch);
		BigInteger a = 0 + +x + 0;
		assert(a == x);
		assert(0 + +x + 0 == x);
		if (x == SCHAR_MAX)break;
	}
	int ii = INT_MIN;
	for (int i = 0; i < 10000; ++ii, ++i) {
		BigInteger x = ii;
		BigInteger a = 0 + +x + 0;
		assert(a == x);
		assert(0 + +x + 0 == x);
	}
}

void test_1() { // #1
	int x = -983041;
	BigInteger t(x);
	assert(t == x);
	++t;
	assert(t == x + 1);
}

void testAdd() {
	for (int i = -257; i <= 257; ++i) {
		for (int j = -257; j <= 257; ++j) {
			BigInteger ii = i, jj = j;
			int ti = i, tj = j;
			ti += tj;
			ii += jj;
			assert(ti == ii);
		}
	}
}

void testCompare() {
	int i;
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

void testSelfAdd() {
	short x = SHRT_MIN;
	BigInteger y = x;
	for (;; ++x, ++y) {
		if (x != y) {
			out(x);
			out(y);
			assert(x == y);
		}
		if (x == SHRT_MAX)break;
	}
	int i = -1000000;
	y = i;
	for (; i <= 1000000; ++i, ++y) {
		assert(i == y);
	}
}

void testAddOne() {
	int x;
	BigInteger y;

	y = x = -1000000;
	for (; x <= 1000000; ++x) {
		assert(x == y);
		y += 1;
	}

	y = x = -1000000;
	for (; x <= 1000000; ++x) {
		assert(x == y);
		y = y + 1;
	}
}

void testUnaryFunction() {
	for (int i = -100000; i <= 100000; ++i) {
		int x = i;
		BigInteger y = x;

		assert(-x == -y);
		assert(!x == !y);
		assert(~x == ~y);
		assert(x++ == y++);
		assert(x-- == y--);
		assert(++x == ++y);
		assert(--x == --y);
	}
}

void testCalc() {
	for (int i = 0; i < 10; ++i) {
		for (int j = -3; j <= 3; ++j) {
			int x = (1 << i) + j;
			for (int k = 0; k < 10; ++k) {
				for (int l = -3; l <= 3; ++l) {
					int y = (1 << k) + l;
					BigInteger xx(x), yy(y);

					assert(xx + yy == x + y);
					assert(xx - yy == x - y);
					assert(xx * yy == x * y);

					if (y) {
						assert(xx / yy == x / y);
						assert(xx % yy == x % y);
					}
				}
			}
		}
	}
}

void test_2() {
	BigInteger x(INT_MIN);
	assert(x + 1 == INT_MIN + 1);
	assert(x - 1 < INT_MIN);
}

void testShift() {
	for (int i = 0; i < 10; ++i) {
		for (int j = -3; j <= 3; ++j) {
			for (int sign = -1; sign <= 1; sign += 2) {
				int x = ((1 << i) + j) * sign;
				for (int k = 0; k < 10; ++k) {
					assert(BigInteger(x) << k == x << k);
					assert(BigInteger(x) >> k == x >> k);
				}
			}
		}
	}
}

void test_3() { // #3
	BigInteger a(256);
	assert(a >> 1 == 128);
}

void testCalc2() {
	for (int cas = 500000; cas--;) {
		LL a = (random.nextInt() & 0x1FFFF) - 0xFFFF;
		LL b = (random.nextInt() & 0x1FFFF) - 0xFFFF;

		BigInteger aa(a), bb(b);
		assert(a + b == aa + bb);
		assert(a - b == aa - bb);
		assert(a * b == aa * bb);

		if (b != 0) {
			assert(a / b == aa / bb);
			assert(a % b == aa % bb);
		}
	}
}

void testDivide() {
	for (int i = 0; i < 100; ++i) {
		int x = 65536 + random.nextInt(65536);
		for (int j = 0; j < 1000; ++j) {
			int y = random.nextInt(65536) + 1;
			assert(x / y == BigInteger(x) / y);
		}
	}
	int cas = 1000000;
	for (int caseNo = 0; cas--; ++caseNo) {
		LL a = random.nextLong(), b = random.nextLong();
		if (b) {
			assert(a / b == BigInteger(a) / b);
		}
	}
}

void testToString10() {
	for (int i = 0; i < 500000; ++i) {
		LL x = random.nextLong();
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
#define  __testConstructor(type) do {							\
	type mx = numeric_limits<type>::max();						\
	type mn = numeric_limits<type>::min();						\
	ostringstream os; os << mx << ' ' << mn;					\
	assert(os.str()[0]);										\
	BigInteger mmx(mx), mmn(mn);								\
	string a = os.str();										\
	string b = mmx.toString() + " " + mmn.toString();			\
	if(a != b) {												\
		out(#type);												\
		out(a);													\
		out(b);													\
		assert(false);											\
		assert(a == b);											\
	}															\
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

#define __testCharConstructor(type)  do {						\
	type mx = numeric_limits<type>::max();						\
	type mn = numeric_limits<type>::min();						\
	ostringstream os; os << (LL) mx << ' ' << (LL) mn;			\
	assert(os.str()[0]);										\
	BigInteger mmx(mx), mmn(mn);								\
	string a = os.str();										\
	string b = mmx.toString() + " " + mmn.toString();			\
	if(a != b) {												\
		out(#type);												\
		out(a);													\
		out(b);													\
		assert(false);											\
		assert(a == b);											\
	}															\
} while(false)
	__testCharConstructor(char);
	__testCharConstructor(signed char);
	__testCharConstructor(unsigned char);
#undef __testCharConstructor
}// </editor-fold>

void testBit() {
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
		int i = random.nextInt(), j = random.nextInt();
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
		char ch1 = 'z', ch2 = 'z';
		a >> ch1;
		b >> ch2;
		if (ch1 != ch2) {
			out(ch1);
			out(ch2);
			assert(ch1 == ch2);
		}
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
		int len = random.nextInt(4) + 1;
		string src;
		for (int i = 0; i < len; ++i) {
			const string & t = ss[random.nextInt(5)];
			src += t[random.nextInt(t.length())];
		}
		testInputString(src);
	}
}

void doTheTest() {
	typedef void (*Fun)();
	Fun funs[] = {
		test0, test_1, testAdd, testSelfAdd, testCompare,
		testAddOne, testUnaryFunction, testCalc, test_2, testCompile,
		testShift, test_3, testCalc2, testDivide, testToString10,
		testStringConstructor, testConstructor, testBit, testInput
	};
	time_t st = clock();
	for (int i = 0; i < sizeof funs / sizeof funs[0]; ++i) {
		funs[i]();
		time_t now = clock();
		out(now - st);
		st = now;
	}
}

void testToStringWithFlag() {
	;
}

void testAll() {
	srand(time(NULL));
	time_t st = clock();
	doTheTest();
	cout << clock() - st << endl;
}

int main(int argc, char** argv) {
	testAll();
	return EXIT_SUCCESS;
}

