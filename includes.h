/* 
 * File:   includes.h
 * Author: zhanhb
 *
 * Created on August 12, 2013, 10:36 AM
 */

#ifndef INCLUDES_H
#define	INCLUDES_H

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
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

typedef vector<int>VI;

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

#define out(x) (std::cout << std::boolalpha << "Line " << __LINE__ << ": " << #x << ": " << (x) << std::endl)

#endif	/* INCLUDES_H */

