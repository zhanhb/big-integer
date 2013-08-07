/*
 * File:   BigInteger.h
 * Author: zhanhb
 *
 * Created on July 25, 2013, 10:37 AM
 */

#ifndef BIGINTEGER_H
#define BIGINTEGER_H
#if (defined(_MSC_VER)) && _MSC_VER <= 1200
#error Plase use other compliers instead of old version VC++ for it does not support standard c++.
#endif

#include <exception>
#include <string>
#include <istream>
#include <ostream>
#include <vector>
#ifdef _BIG_INTEGER_DEBUG
#include <cassert>
#define __big_integer_assert(e) assert(e)
#else
#define __big_integer_assert(e)
#endif

template<typename StorageType, typename CalculateType>
class BasicBigInteger {
	typedef StorageType Value;
	typedef CalculateType Calc;
	typedef std::vector<Value>Mag;
	typedef Mag & MagRef;
	typedef const Mag & ConstMagRef;
	typedef typename Mag::size_type SizeType;

	const static int SHIFT;
	const static Calc INT_MASK;

	Mag mag;
	int signum;

	BasicBigInteger(ConstMagRef dat, int sign) : mag(dat), signum(dat.size() ? sign : 0) { }
public:

	BasicBigInteger() : mag(), signum(0) { }

	BasicBigInteger(const BasicBigInteger & another) : mag(another.mag), signum(another.signum) { }

	BasicBigInteger(Value x) : mag(!!x, x < 0 ? -x : x), signum((x > 0) - (x < 0)) { }

	template<typename _CharT, typename _Traits, typename _Alloc>
	BasicBigInteger(const std::basic_string<_CharT, _Traits, _Alloc> & s, int radix = 10)
	: mag(), signum(0) {
		assign(s, radix);
	}

	template<typename _CharT>
	BasicBigInteger(const _CharT * s, int radix = 10) : mag(), signum(0) {
		assign(s, radix);
	}

	template<typename IntClass>
	BasicBigInteger(IntClass val) : mag(), signum(0) {
		IntClass zero(0);
		if (val != zero) {
			signum = (val < zero) ? -1 : 1;

			mag.push_back(signum < 0 ? - static_cast<Value> (val) : static_cast<Value> (val));

			int shift = SHIFT;
			if (shift < 8 * sizeof val) {
				if (signum < 0) val = zero - val;
				val >>= shift;
				if (val < zero) val &= ~(~static_cast<IntClass> (0) << ((8 * sizeof val) - shift));
				while (val) {
					mag.push_back(val);
					val >>= shift;
				}
			}
		}
	}

	BasicBigInteger & operator++() {
		if (signum >= 0) {
			signum = 1;
			addOne(mag);
		} else {
			substractOne(mag);
			if (mag.empty())signum = 0;
		}
		return *this;
	}

	BasicBigInteger operator++(int) {
		BasicBigInteger tmp(*this);
		++ * this;
		return tmp;
	}

	BasicBigInteger & operator--() {
		if (signum <= 0) {
			signum = -1;
			addOne(mag);
		} else {
			substractOne(mag);
			if (mag.empty())signum = 0;
		}
		return *this;
	}

	BasicBigInteger operator--(int) {
		BasicBigInteger tmp(*this);
		-- * this;
		return tmp;
	}

	BasicBigInteger operator+() const {
		return *this;
	}

	BasicBigInteger operator-() const {
		return BasicBigInteger(mag, -signum);
	}

	BasicBigInteger operator~() const {
		Mag mag(this->mag);

		int sign = signum < 0 ? 1 : -1;

		if (signum < 0) substractOne(mag);
		else addOne(mag);

		if (mag.empty())sign = 0;
		return BasicBigInteger(mag, sign);
	}

	bool operator!() const {
		return !signum;
	}

#ifndef _BIG_INTEGER_NO_STRICT

	class ParseException : public std::exception {
		const char * message;
	public:

		ParseException(const char * what) : message(what ? what : "") { }

		~ParseException() throw () { }

		const char * what() const throw () {
			return message;
		}
	};
private:

	static void __throw_parse_error(const char * s) {
		throw ParseException(s);
	}
#else
private:

	static void __throw_parse_error(const char * s) {
		// do nothing
	}
#endif
public:

	template<typename _CharT> void assign(const _CharT * val, int radix = 10) {
		assign(std::basic_string<_CharT > (val), radix);
	}

	template<typename _CharT, typename _Traits, typename _Alloc>
	void assign(const std::basic_string<_CharT, _Traits, _Alloc> & __s, int radix = 10) {
		MagRef mag = this->mag;

		int sign = 1;

		if (radix < 2 || radix > 36)
			radix = 10;

		typedef typename std::basic_string<_CharT, _Traits, _Alloc>::size_type Cursor;
		Cursor cursor = 0;
		if (__s[cursor] == '+' || __s[cursor] == '-') {
			if (__s[cursor] == '-')sign = -1;
			++cursor;
		}

		if (!__s[cursor]) {
			__throw_parse_error("Zero length BigInteger");
			mag.clear();
			signum = 0;
			return;
		}

		while (__s[cursor] && tool.digit(__s[cursor], radix) == 0) ++cursor;

		if (!__s[cursor] || tool.digit(__s[cursor], radix) < 0) {
			if (__s[cursor])__throw_parse_error("Illegal digit");
			mag.clear();
			signum = 0;
			return;
		}

		Cursor last(cursor);
		for (; __s[last] && tool.digit(__s[last], radix) >= 0; ++last);

		if (__s[last]) __throw_parse_error("Illegal digit");

		SizeType numDigits = last - cursor;
		__big_integer_assert(numDigits);
		signum = sign;

		// Pre-allocate array of expected size. May be too large but can
		// never be too small. Typically exact.
		Value superRadix = tool.intRadix[radix];
		int digitsLen = tool.digitsPerInt[radix];
		SizeType numBits = SizeType(1.0 * numDigits / digitsLen * bitLength(superRadix));

		SizeType numWords = numBits / SHIFT + 1;

		ensureSize(mag, numWords);

		// Process first (potentially short) digit group
		int firstGroupLen = numDigits % digitsLen;
		if (firstGroupLen == 0)firstGroupLen = digitsLen;

		mag[0] = parseInt(__s.begin() + cursor, __s.begin() + cursor + firstGroupLen, radix);
		cursor += firstGroupLen;

		__big_integer_assert(mag[0] > 0);

		// Process remaining digit groups
		SizeType size = 1;
		for (; cursor < last; cursor += digitsLen) {
			Value groupVal = parseInt(__s.begin() + cursor, __s.begin() + cursor + digitsLen, radix);
			size = destructiveMulAdd(mag, superRadix, groupVal, size);
		}
		__big_integer_assert(cursor == last);
		mag.resize(size);
	}

	BasicBigInteger & operator=(const BasicBigInteger & another) {
		this->mag = another.mag;
		this->signum = another.signum;
		return *this;
	}

	template<typename _CharT, typename _Traits, typename _Alloc>
			BasicBigInteger & operator=
			(const std::basic_string<_CharT, _Traits, _Alloc> & val) {
		assign(val);
		return *this;
	}

	template<typename _CharT> BasicBigInteger & operator=(const _CharT * val) {
		assign(val);
		return *this;
	}

	BasicBigInteger & operator+=(const BasicBigInteger & val) {
		if (val.signum == 0)
			return *this;

		if (signum == 0)
			return *this = val;

		if (signum == val.signum) {
			addTo(mag, val.mag);
			return *this;
		}

		int cmp = compareMagnitude(mag, val.mag);
		if (cmp == 0) {
			mag.clear();
			signum = 0;
			return *this;
		}

		if (cmp > 0) {
			subtractFrom(mag, val.mag);
		} else {
			subtractTo(val.mag, mag);
		}
		normalize(mag);
		signum = (cmp == signum ? 1 : -1);
		return *this;
	}

	BasicBigInteger & operator-=(const BasicBigInteger & val) {
		if (val.signum == 0)
			return *this;
		if (signum == 0) {
			mag = val.mag;
			signum = -val.signum;
			return *this;
		}
		if (val.signum != signum) {
			addTo(mag, val.mag);
			return *this;
		}

		int cmp = compareMagnitude(mag, val.mag);
		if (cmp == 0) {
			mag.clear();
			signum = 0;
			return *this;
		}

		if (cmp > 0) {
			subtractFrom(mag, val.mag);
		} else {
			subtractTo(val.mag, mag);
		}
		normalize(mag);
		signum = (cmp == signum ? 1 : -1);
		return *this;
	}

	BasicBigInteger & operator<<=(int n) {
		if (n && SizeType(n) == n) {
			leftShift(mag, n);
		}
		return *this;
	}

	BasicBigInteger & operator>>=(int n) {
		if (n && SizeType(n) == n) {
			if (signum < 0) {
				substractOne(mag);
				rightShift(mag, n);
				addOne(mag);
			} else {
				rightShift(mag, n);
				if (mag.empty())signum = 0;
			}
		}
		return *this;
	}

	int compareTo(const BasicBigInteger & val) const {
		if (signum == val.signum) {
			switch (signum) {
				case 1:
					return compareMagnitude(mag, val.mag);
				case -1:
					return compareMagnitude(val.mag, mag);
				default:
					return 0;
			}
		}
		return signum < val.signum ? -1 : 1;
	}

	template<typename _CharT, typename _Traits, typename _Alloc>
	std::basic_string<_CharT, _Traits, _Alloc> &
	getChars(std::basic_string<_CharT, _Traits, _Alloc> & toAppend, int radix = 10) const {
		if (signum == 0) {
			toAppend += '0';
			return toAppend;
		}
		if (radix < 2 || radix > 36)
			radix = 10;

		if ((radix & (radix - 1)) == 0) {
			if (signum < 0)
				toAppend += '-';
			return getChars(mag, bitLength(radix - 1), toAppend);
		}

		std::basic_string<_CharT, _Traits, _Alloc> sbuf;
		const int bufsize = 31;
		_CharT buf[bufsize + 1] = {0};

		// Translate number to string, a digit group at a time
		Mag tmp(mag), q, r;
		Calc r2;
		BasicBigInteger d(tool.longRadix[radix]);
		int len = tool.digitsPerLong[radix], k = len, m = 0;
		while (!tmp.empty()) {
			divide(tmp, d.mag, q, r);
			r2 = toLong(r);

			for (; k < len; ++k) {
				buf[m++] = '0';
				if (m == bufsize) sbuf += buf, m = 0;
			}
			k = 0;
			do {
				buf[m++] = digits[r2 % radix];
				r2 /= radix;
				if (m == bufsize) sbuf += buf, m = 0;
				++k;
			} while (r2);

			if (q.empty())break;

			divide(q, d.mag, tmp, r);
			r2 = toLong(r);
			for (; k < len; ++k) {
				buf[m++] = '0';
				if (m == bufsize) sbuf += buf, m = 0;
			}
			k = 0;
			do {
				buf[m++] = digits[r2 % radix];
				r2 /= radix;
				if (m == bufsize) sbuf += buf, m = 0;
				++k;
			} while (r2);
		}
		buf[m] = '\0';
		sbuf += buf;

		// Put sign (if any) and first digit group into result buffer
		if (signum < 0)
			sbuf += '-';

		typedef typename std::basic_string<_CharT, _Traits, _Alloc>::iterator _Ite;
		for (_Ite first = sbuf.begin(), last = sbuf.end();;)
			if (first == last || first == --last)break;
			else {
				int tmp = *first;
				*first = *last;
				*last = tmp;
				++first;
			}

		toAppend += sbuf;
		return toAppend;
	}

	std::string toString(int radix = 10) const {
		std::string buf;
		return getChars(buf, radix);
	}

private:

	const static class Tool {
		friend class BasicBigInteger;
		int digitsPerInt[37];
		int digitsPerLong[37];
		Value intRadix[37];
		Calc longRadix[37];
		int digits[128];

		Tool() {
			int i;
			for (i = 2; i <= 36; ++i) {
				int cnt = 0;
				for (Calc val(1), tmp;; val = tmp, ++cnt) {
					if ((tmp = val * i) / i != val) {
						longRadix[i] = val;
						digitsPerLong[i] = cnt;
						break;
					}
				}
				cnt = 0;
				for (Value val(1), tmp;; val = tmp, ++cnt) {
					if ((tmp = val * i) / i != val) {
						intRadix[i] = val;
						digitsPerInt[i] = cnt;
						break;
					}
				}
			}
			for (i = 0; i < 128; ++i) digits[i] = -1;
			for (i = '0'; i <= '9'; ++i) digits[i] = i - '0';
			for (i = 'A'; i <= 'Z'; ++i) digits[i] = i - 'A' + 10;
			for (i = 'a'; i <= 'z'; ++i) digits[i] = i - 'a' + 10;
		}

		int digit(int ch, int radix) const {
			int digit = (0 <= ch && ch < 128) ? digits[ch] : -1;
			return digit < radix ? digit : -1;
		}
	} tool;

	static void ensureSize(MagRef data, SizeType minSize) {
		SizeType size = data.size();
		if (size < minSize) {
			data.insert(data.end(), minSize - size, 0);
		}
	}

	static void addOne(MagRef data) {
		int carry = 1;
		for (SizeType i = 0, asize = data.size(); carry && i < asize; ++i) {
			carry = !++data[i];
		}
		if (carry)data.push_back(carry);
	}

	static void substractOne(MagRef data) {
		SizeType i = 0, size = data.size();
		__big_integer_assert(size);
		int borrow = 1;
		for (; borrow && i < size; ++i) {
			borrow = !data[i]--;
		}
		if (data[size - 1] == 0)data.pop_back();
	}

	static int compareMagnitude(ConstMagRef a, ConstMagRef b) {
		SizeType i = a.size(), j = b.size();
		if (i != j) {
			return i < j ? -1 : 1;
		}

		while (i--) {
			if (a[i] != b[i]) {
				return (a[i] & INT_MASK) <(b[i] & INT_MASK) ? -1 : 1;
			}
		}
		return 0;
	}

	static void addTo(MagRef data, ConstMagRef add) {
		int carry = 0;
		SizeType i = 0, bsize = add.size();

		ensureSize(data, bsize);

		for (; i < bsize; ++i) {
			Calc t = (data[i] & INT_MASK) + (add[i] & INT_MASK) + carry;
			data[i] = t;
			carry = !!(t >> SHIFT);
		}
		if (!carry)return;
		for (SizeType asize = data.size(); carry && i < asize; ++i) {
			carry = !++data[i];
		}
		if (carry)data.push_back(carry);
	}

	static void subtractFrom(MagRef data, ConstMagRef minus) {
		int borrow = 0;
		SizeType i = 0, ssize = minus.size(), bsize = data.size();
		__big_integer_assert(data.size() >= minus.size());
		for (; i < ssize; ++i) {
			Calc t = (data[i] & INT_MASK) - (minus[i] & INT_MASK) - borrow;
			data[i] = t;
			borrow = (t >> SHIFT) != 0;
		}
		for (; borrow && i < bsize; ++i) {
			borrow = !data[i]--;
		}
		__big_integer_assert(!borrow);
	}

	static void subtractTo(ConstMagRef data, MagRef minus) {
		SizeType size = data.size();
		int borrow = 0;

		ensureSize(minus, data.size());
		__big_integer_assert(data.size() == minus.size());

		SizeType i = 0;
		for (; i < size; ++i) {
			Calc t = (data[i] & INT_MASK) - (minus[i] & INT_MASK) - borrow;
			minus[i] = t;
			borrow = !!(t >> SHIFT);
		}
		__big_integer_assert(!borrow);
	}

	static void normalize(MagRef data) {
		for (SizeType size = data.size(); size && !data[--size]; data.pop_back());
	}

	static void multiplyToLen(ConstMagRef x, ConstMagRef y, MagRef z) {
		SizeType xlen = x.size(), ylen = y.size();

		z.resize(xlen + ylen);
		Calc carry = 0;

		for (SizeType j = 0, k = 0; j < ylen; ++j, ++k) {
			Calc product = (y[j] & INT_MASK) *
					(x[0] & INT_MASK) + carry;
			z[k] = product;
			carry = (product >> SHIFT) & INT_MASK;
		}

		z[ylen] = carry;

		for (SizeType i = 1; i < xlen; ++i) {
			Calc xiLong = x[i] & INT_MASK;
			if (!xiLong)continue;
			carry = 0;
			for (SizeType j = 0, k = j + i; j < ylen; ++j, ++k) {
				carry += (y[j] & INT_MASK) * xiLong + (z[k] & INT_MASK);
				z[k] = carry;
				carry = (carry >> SHIFT) & INT_MASK;
			}
			z[i + ylen] = carry;
		}
		if (carry == 0)z.pop_back();
	}

	inline static void multiply(ConstMagRef x, ConstMagRef y, MagRef z) {
		multiplyToLen(x, y, z);
	}

	static MagRef divide(ConstMagRef a, ConstMagRef b,
			MagRef quotient, MagRef rem) {
		if (!b.size()) { // divide by zero
			// the size of b is zero, divide by it will cause the same result
			quotient.push_back(0 / b.size());
		}

		// Dividend is zero
		if (a.empty()) {
			quotient.clear();
			rem.clear();
			return rem;
		}

		int cmp = compareMagnitude(a, b);
		// Dividend less than divisor
		if (cmp < 0) {
			quotient.clear();
			return rem = a;
		}
		// Dividend equal to divisor
		if (cmp == 0) {
			quotient.assign(1, 1);
			rem.clear();
			return rem;
		}

		// Special case one word divisor
		if (b.size() == 1) {
			Value r = divideOneWord(a, b[0], quotient);
			if (r == 0) {
				rem.clear();
				return rem;
			}
			rem.assign(1, r);
			return rem;
		}

		// Copy divisor value to protect divisor
		return divideMagnitude(a, b, quotient, rem);
	}

	template<typename IntClass> static int bitLength(IntClass n) {
		return 8 * sizeof n - numberOfLeadingZeros(n);
	}

	static bool unsignedLongCompare(Calc a, Calc b) {
		// unsigned compare return true if a is larger than b
		return ((a < 0) == (b < 0)) ? (a > b) : (a < b);
	}

	static Value mulsub(MagRef q, ConstMagRef a, Value x, SizeType len, SizeType offset) {
		Calc xLong = x & INT_MASK;
		Calc carry = 0;

		for (SizeType j = 0; j < len; ++j) {
			Calc product = (a[j] & INT_MASK) * xLong + carry;
			Calc difference = q[offset] - product;
			q[offset++] = difference;
			carry = ((product >> SHIFT) & INT_MASK)
					+ (((difference & INT_MASK)>
					(~product & INT_MASK)) ? 1 : 0);
		}
		return carry;
	}

	static Value divadd(ConstMagRef a, MagRef result, SizeType offset) {
		Calc carry = 0;
		for (SizeType j = 0, size = a.size(); j < size; ++j) {
			Calc sum = (a[j] & INT_MASK) +
					(result[j + offset] & INT_MASK) + carry;
			result[j + offset] = sum;
			carry = (sum >> SHIFT) & INT_MASK;
		}
		return carry;
	}

	static MagRef divideMagnitude(ConstMagRef value, Mag divisor,
			MagRef quotient, MagRef rem) { // divisor is a copy of the orign.
		// Remainder starts as dividend with space for a leading zero
		rem = value;

		SizeType nlen = rem.size();

		// Set the quotient size
		SizeType dlen = divisor.size();
		__big_integer_assert(dlen >= 2);
		SizeType limit = nlen - dlen + 1;

		quotient.resize(limit);

		// D1 normalize the divisor
		int shift = numberOfLeadingZeros(divisor[dlen - 1]);

		if (shift > 0) {
			// First shift will not grow array
			leftShift(divisor, shift);
			// But this one might
			leftShift(rem, shift);
		}
		__big_integer_assert(divisor.size() == dlen);

		// Must insert leading 0 in rem if its length did not change
		if (rem.size() == nlen) {
			rem.push_back(0);
		}

		Value dh = divisor[dlen - 1];
		Calc dhLong = dh & INT_MASK;
		Value dl = divisor[dlen - 2];
		Value qWord[2];

		// D2 Initialize j
		for (SizeType j = limit; j--;) {
			// D3 Calculate qhat
			// estimate qhat
			Value qhat = 0;
			Value qrem = 0;
			bool skipCorrection = false;
			Value nh = rem[dlen + j];
			Calc nhLong = nh & INT_MASK;
			Value nm = rem[dlen + j - 1];

			if (nh == dh) {
				qhat = ~0;
				qrem = nh + nm;
				skipCorrection = (qrem & INT_MASK) < nhLong;
			} else {
				Calc nChunk = ((nh & INT_MASK) << SHIFT) | (nm & INT_MASK);
				if (nChunk >= 0) {
					qhat = nChunk / dhLong;
					qrem = nChunk - qhat * dhLong;
				} else {
					divWord(qWord, nChunk, dh);
					qhat = qWord[1];
					qrem = qWord[0];
				}
			}

			if (qhat == 0) {
				quotient[j] = qhat;
				continue;
			}

			if (!skipCorrection) { // Correct qhat
				__big_integer_assert(dlen + j >= 2);
				__big_integer_assert(dlen + j - 2 >= 0);
				Calc nl = rem[dlen + j - 2] & INT_MASK;
				Calc rs = ((qrem & INT_MASK) << SHIFT) | nl;
				Calc estProduct = (dl & INT_MASK) * (qhat & INT_MASK);

				if (unsignedLongCompare(estProduct, rs)) {
					qhat--;
					qrem = (qrem & INT_MASK) + dhLong;
					if ((qrem & INT_MASK) >= dhLong) {
						estProduct -= (dl & INT_MASK);
						rs = ((qrem & INT_MASK) << SHIFT) | nl;
						if (unsignedLongCompare(estProduct, rs))
							qhat--;
					}
				}
			}

			// D4 Multiply and subtract
			rem[dlen + j] = 0;
			Value borrow = mulsub(rem, divisor, qhat, dlen, j);

			// D5 Test remainder
			if ((borrow & INT_MASK) > nhLong) {
				// D6 Add back
				divadd(divisor, rem, j);
				qhat--;
			}

			// Store the quotient digit
			quotient[j] = qhat;
		} // D7 loop on j

		// D8 Unnormalize
		if (shift > 0)
			rightShift(rem, shift);

		normalize(quotient);
		normalize(rem);
		return rem;
	}

	template<typename IntClass> static int numberOfLeadingZeros(IntClass i) {
		int size = 8 * sizeof i;
		__big_integer_assert((size & (size - 1)) == 0);
		if (i == 0) return size;

		for (int n = 1, sh = size >> 1;; sh >>= 1) {
			IntClass toAnd = ~(~static_cast<IntClass> (0) << sh);
			int toShift = size - sh;
			if (!((i >> toShift) & toAnd)) {
				n += sh;
				i <<= sh;
			}
			if (sh == 2) return n - ((i >> (size - 1)) & 1);
		}
	}

	static Value divideOneWord(ConstMagRef value, Value divisor,
			MagRef quotient) {
		__big_integer_assert(value.size());
		__big_integer_assert(divisor);
		Calc divisorLong = divisor & INT_MASK;

		// Special case of one word dividend
		if (value.size() == 1) {
			Calc dividendValue = value[0] & INT_MASK;
			Value q = dividendValue / divisorLong;
			Value r = dividendValue - q * divisorLong;
			if (q == 0)quotient.clear();
			else quotient.assign(1, q);
			return r;
		}

		SizeType i = value.size();
		quotient.resize(i);

		// Normalize the divisor
		int leading = numberOfLeadingZeros(divisor);

		Value rem = value[--i];
		Calc remLong = rem & INT_MASK;
		if (remLong < divisorLong) {
			quotient[i] = 0;
		} else {
			Value tmp(remLong / divisorLong);
			quotient[i] = tmp;
			rem = remLong - tmp * divisorLong;
			remLong = rem & INT_MASK;
		}

		Value qWord[2];
		while (i) {
			Calc dividendEstimate = (remLong << SHIFT) | (value[--i] & INT_MASK);

			if (dividendEstimate >= 0) {
				qWord[1] = dividendEstimate / divisorLong;
				qWord[0] = dividendEstimate - qWord[1] * divisorLong;
			} else {
				divWord(qWord, dividendEstimate, divisor);
			}
			quotient[i] = qWord[1];
			rem = qWord[0];
			remLong = rem & INT_MASK;
		}

		normalize(quotient);
		// Unnormalize
		if (leading > 0)
			return rem % divisor;
		else
			return rem;
	}

	static void divWord(Value * result, Calc n, Value d) {
		__big_integer_assert(d);
		Calc dLong = d & INT_MASK;

		// Approximate the quotient and remainder
		Calc mask63 = ~(~static_cast<Calc> (0) << (8 * sizeof (Calc) - 1));
		__big_integer_assert(mask63 > 0);
		Calc q = ((n >> 1) & mask63) / ((dLong >> 1) & mask63);
		Calc r = n - q * dLong;

		// Correct the approximation
		while (r < 0) {
			r += dLong;
			q--;
		}
		while (r >= dLong) {
			r -= dLong;
			q++;
		}

		// n - q*dlong == r && 0 <= r <dLong, hence we're done.
		result[0] = r;
		result[1] = q;
	}

	static void leftShift(MagRef val, SizeType n) {
		SizeType intLen = val.size();
		if (intLen == 0 || n == 0)
			return;

		SizeType nInts = n / SHIFT, newLen = nInts + intLen;
		int nBits = n % SHIFT;
		int leadingZerosInHighWord = numberOfLeadingZeros(val[intLen - 1]);

		if (nBits > leadingZerosInHighWord) {
			++newLen;
		}

		val.resize(newLen);

		int nBits2 = SHIFT - nBits;
		SizeType i = newLen - 1;
		Value c = val[i - nInts], b;

		for (; i > nInts; --i) {
			b = c;
			c = val[i - nInts - 1];
			val[i] = (b << nBits) | ((c & INT_MASK) >> nBits2);
		}

		val[i] = val[i - nInts] << nBits;
		while (i) val[--i] = 0;
	}

	static void rightShift(MagRef val, SizeType n) {
		SizeType intLen = val.size();
		if (intLen == 0 || n == 0)
			return;

		SizeType nInts = n / SHIFT;
		if (intLen <= nInts) {
			val.clear();
			return;
		}

		SizeType newSize = intLen - nInts;
		int nBits = n % SHIFT;

		if (nBits >= bitLength(val[intLen - 1])) {
			--newSize;
			if (!newSize) {
				val.clear();
				return;
			}
		}

		SizeType i = 0;
		Value c = val[i + nInts], b;

		int n2 = SHIFT - nBits;
		for (c = val[i]; i + nInts + 1 < intLen; ++i) {
			b = c;
			c = val[i + nInts + 1];
			val[i] = (c << n2) | ((b & INT_MASK) >> nBits);
		}
		val[i] = (val[i + nInts] & INT_MASK) >> nBits;
		val.resize(newSize);
	}

	template<typename _CharT, typename _Traits, typename _Alloc>
	static std::basic_string<_CharT, _Traits, _Alloc> &
	getChars(ConstMagRef mag, int shift, std::basic_string<_CharT, _Traits, _Alloc> & toAppend) {
		SizeType intLen = mag.size();
		SizeType nBits = intLen * SHIFT;

		nBits -= numberOfLeadingZeros(mag[intLen - 1]);

		typename std::basic_string<_CharT, _Traits, _Alloc>::size_type cursor = toAppend.size();
		SizeType add = (nBits + shift - 1) / shift;
		toAppend.resize(cursor + add);

		Calc val = 0, mask = ~(~static_cast<Calc> (0) << shift);
		SizeType magShift = intLen * SHIFT, that = add * shift;

		for (SizeType i = 0; i < add; ++i) {
			that -= shift;
			if (magShift > that) {
				val = (val << SHIFT) | (mag[--intLen] & INT_MASK);
				magShift -= SHIFT;
			}

			toAppend[cursor++] = digits[(val >> (that - magShift)) & mask];
		}
		return toAppend;
	}

	static Calc toLong(ConstMagRef data) {
		Calc ret(0);
		for (SizeType i = data.size(); i--;) {
			Calc tmp((ret << SHIFT) + (data[i] & INT_MASK));
			__big_integer_assert(tmp >= ret);
			ret = tmp;
		}
		return ret;
	}

	static SizeType destructiveMulAdd(MagRef x, Value y, Value z, SizeType size) {
		// Perform the multiplication word by word
		Calc ylong = y & INT_MASK;
		Calc carry = z & INT_MASK;

		for (SizeType i = 0; i < size; ++i) {
			carry += (x[i] & INT_MASK) * ylong;
			x[i] = carry;
			carry = (carry >> SHIFT) & INT_MASK;
		}
		if (carry) {
			x[size++] = carry;
			carry = (carry >> SHIFT) & INT_MASK;
		}
		return size;
	}

	template<typename _ForwardIterator>
	static Value parseInt(_ForwardIterator first, _ForwardIterator last, int radix) {
		Value val(0);
		for (; first != last; ++first) {
			int x = tool.digit(*first, radix);
			if (x < 0) __throw_parse_error("String modified error"), x = 0;
			Value tmp = val * radix + x;
			__big_integer_assert(tmp >= val);
			val = tmp;
		}
		return val;
	}

	static void doAnd(ConstMagRef a, ConstMagRef b, MagRef result) {
		SizeType as = a.size(), bs = b.size();
		SizeType mn = as < bs ? as : bs, mx = as + bs - mn;
		result.resize(mn);
		SizeType i;
		for (i = 0; i < mn; ++i) result[i] = a[i] & b[i];
		normalize(result);
	}

	static void doOr(ConstMagRef a, ConstMagRef b, MagRef result) {
		SizeType as = a.size(), bs = b.size();
		SizeType mn = as < bs ? as : bs, mx = as + bs - mn;
		result.resize(mx);
		SizeType i;
		for (i = 0; i < mn; ++i) result[i] = a[i] | b[i];
		for (; i < as; ++i)result[i] = a[i];
		for (; i < bs; ++i)result[i] = b[i];
	}

	static void andNot(ConstMagRef a, Mag b, MagRef result) {
		ensureSize(b, a.size());
		result.resize(a.size());
		SizeType i;
		for (i = 0; i < a.size(); ++i) {
			result[i] = a[i] &~b[i];
		}
		normalize(result);
	}

	static void doXor(ConstMagRef a, Mag b, MagRef result) {
		SizeType as = a.size(), bs = b.size();
		SizeType mn = as < bs ? as : bs, mx = as + bs - mn;
		SizeType i;
		result.resize(mx);
		for (i = 0; i < mn; ++i) result[i] = a[i] ^ b[i];
		for (; i < as; ++i)result[i] = a[i];
		for (; i < bs; ++i)result[i] = b[i];
		normalize(result);
	}

	template<typename _CharT, typename _Traits>
	static void doGet(std::basic_istream<_CharT, _Traits> & __io, std::ios::iostate & __err, BasicBigInteger & x) {
		std::basic_string<_CharT> __xtrc;
		typedef std::istreambuf_iterator<_CharT, _Traits> _InIter;
		_InIter __beg(__io), __end(0);
		bool __testeof = __beg == __end;
		_CharT __c = _CharT();

		const std::ios::fmtflags __basefield = __io.flags() & std::ios::basefield;
		int __base = __basefield == std::ios::oct ? 8 :
				(__basefield == std::ios::hex ? 16 : 10);

		// First check for sign.
		if (!__testeof) {
			__c = *__beg;
			if (__c == '+' || __c == '-') {
				__xtrc += __c;
				if (++__beg != __end)
					__c = *__beg;
				else
					__testeof = true;
			}
		}

		int __sep_pos = 0;

		while (!__testeof) {
			int __digit = tool.digit(__c, __base);
			if (__digit < 0)
				break;

			__xtrc += __c;
			++__sep_pos;

			if (++__beg != __end)
				__c = *__beg;
			else
				__testeof = true;
		}

		if (!__sep_pos)
			__err = std::ios::failbit;

		if (__testeof)
			__err |= std::ios::eofbit;

		if (__sep_pos)
			x.assign(__xtrc);
#if !(defined(_MSC_VER))
			// different compliers do the different things.
			// I only test VS 2012 and Mingw.
		else if (__err)
			x = 0;
#endif
	}

	static const char * const digits;
public:

	// friends' functions

	template<typename _CharT, typename _Traits>
	friend std::basic_istream<_CharT, _Traits> &
	operator>>(std::basic_istream<_CharT, _Traits> & __io, BasicBigInteger & x) {
		typedef typename std::basic_istream<_CharT, _Traits>::sentry Sentry;
		Sentry __cerb(__io, false);
		if (__cerb) {
			std::ios::iostate __err = std::ios::goodbit;
			doGet(__io, __err, x);
			if (__err)
				__io.setstate(__err);
		}
		return __io;
	}

	template<typename _CharT, typename _Traits>
	friend std::basic_ostream<_CharT, _Traits> &
	operator<<(std::basic_ostream<_CharT, _Traits> & out, const BasicBigInteger &val) {
		std::basic_string<_CharT> _str;
		const std::ios::fmtflags flags = out.flags(), baseFlag = flags & std::ios::basefield;
		int base = baseFlag == std::ios::oct ? 8
				: baseFlag == std::ios::hex ? base = 16 : 10;

		val.getChars(_str, base);

		if (base == 16 && (flags & std::ios::uppercase)) {
			typedef typename std::basic_string<_CharT>::size_type Size;
			for (Size i = _str.size(); i--;) {
				_CharT & _ch = _str[i];
				if ('a' <= _ch && _ch <= 'z')
					_ch ^= 32;
			}
		}
		return out << _str;
	}

	friend BasicBigInteger operator*(const BasicBigInteger & lhs, const BasicBigInteger & rhs) {
		if (lhs.signum == 0 || rhs.signum == 0)
			return BasicBigInteger();

		Mag mag;
		multiply(lhs.mag, rhs.mag, mag);
		return BasicBigInteger(mag, lhs.signum * rhs.signum);
	}

	friend BasicBigInteger operator/(const BasicBigInteger & lhs, const BasicBigInteger &rhs) {
		Mag qmag, rmag;
		divide(lhs.mag, rhs.mag, qmag, rmag);
		return BasicBigInteger(qmag, lhs.signum * rhs.signum);
	}

	friend BasicBigInteger operator%(const BasicBigInteger & lhs, const BasicBigInteger & rhs) {
		Mag qmag, rmag;
		return BasicBigInteger(divide(lhs.mag, rhs.mag, qmag, rmag), lhs.signum);
	}

	friend BasicBigInteger operator &(const BasicBigInteger & lhs, const BasicBigInteger & rhs) {
		int sa = lhs.signum, sb = rhs.signum;
		if (sa == 0)return lhs;
		if (sb == 0)return rhs;
		if (sa == sb) {
			if (sa > 0) {
				Mag result;
				doAnd(lhs.mag, rhs.mag, result);
				return BasicBigInteger(result, 1);
			} else {
				Mag aa(lhs.mag), bb(rhs.mag), result;
				substractOne(aa);
				substractOne(bb);
				doOr(aa, bb, result);
				addOne(result);
				return BasicBigInteger(result, -1);
			}
		} else {
			Mag mag;
			if (sa > 0) {
				Mag bmag(rhs.mag);
				substractOne(bmag);
				andNot(lhs.mag, bmag, mag);
				return BasicBigInteger(mag, 1);
			} else {
				Mag amag(lhs.mag);
				substractOne(amag);
				andNot(rhs.mag, amag, mag);
				return BasicBigInteger(mag, 1);
			}
		}
	}

	friend BasicBigInteger operator |(const BasicBigInteger & lhs, const BasicBigInteger & rhs) {
		int sa = lhs.signum, sb = rhs.signum;
		if (sa == 0)return rhs;
		if (sb == 0)return lhs;
		if (sa == sb) {
			if (sa > 0) {
				Mag result;
				doOr(lhs.mag, rhs.mag, result);
				return BasicBigInteger(result, 1);
			} else {
				Mag aa(lhs.mag), bb(rhs.mag), result;
				substractOne(aa);
				substractOne(bb);
				doAnd(aa, bb, result);
				addOne(result);
				return BasicBigInteger(result, -1);
			}
		} else {
			Mag mag;
			if (sa > 0) {
				Mag bmag(rhs.mag);
				substractOne(bmag);
				andNot(bmag, lhs.mag, mag);
				addOne(mag);
				return BasicBigInteger(mag, -1);
			} else {
				Mag amag(lhs.mag);
				substractOne(amag);
				andNot(amag, rhs.mag, mag);
				addOne(mag);
				return BasicBigInteger(mag, -1);
			}
		}
	}

	friend BasicBigInteger operator ^(const BasicBigInteger & lhs, const BasicBigInteger & rhs) {
		int sa = lhs.signum, sb = rhs.signum;
		if (sa == 0)return rhs;
		if (sb == 0)return lhs;
		if (sa == sb) {
			if (sa > 0) {
				Mag result;
				doXor(lhs.mag, rhs.mag, result);
				return BasicBigInteger(result, 1);
			} else {
				Mag aa(lhs.mag), bb(rhs.mag), result;
				substractOne(aa);
				substractOne(bb);
				doXor(aa, bb, result);
				return BasicBigInteger(result, 1);
			}
		} else {
			Mag mag;
			if (sa > 0) {
				Mag bmag(rhs.mag);
				substractOne(bmag);
				doXor(bmag, lhs.mag, mag);
				addOne(mag);
				return BasicBigInteger(mag, -1);
			} else {
				Mag amag(lhs.mag);
				substractOne(amag);
				doXor(amag, rhs.mag, mag);
				addOne(mag);
				return BasicBigInteger(mag, -1);
			}
		}
	}

#define declareEqualFromFun(cl, op)					\
friend cl operator op##=							\
(cl & __lhs, const cl & __rhs) {					\
	return __lhs = __lhs op __rhs;					\
}

	declareEqualFromFun(BasicBigInteger, &)
	declareEqualFromFun(BasicBigInteger, |)
	declareEqualFromFun(BasicBigInteger, ^)
	declareEqualFromFun(BasicBigInteger, *)
	declareEqualFromFun(BasicBigInteger, /)
	declareEqualFromFun(BasicBigInteger, %)
#undef declareEqualFromFun

#define declareFunFromEqual(cl, op)	 friend			\
cl operator op(const cl & __lhs, const cl & __rhs) {\
	cl __tmp(__lhs);								\
	__tmp op##= __rhs;								\
	return __tmp;									\
}
	declareFunFromEqual(BasicBigInteger, +)
	declareFunFromEqual(BasicBigInteger, -)
#undef declareFunFromEqual

#define declareFunFromEqual2(cl, op) friend			\
cl operator op(const cl & __val, int __shift) {		\
	cl __tmp(__val);								\
	__tmp op##= __shift;							\
	return __tmp;									\
}
	declareFunFromEqual2(BasicBigInteger, <<)
	declareFunFromEqual2(BasicBigInteger, >>)
#undef declareFunFromEqual2


#define declareFunFromCompareTo(cl, op)				\
	friend inline bool								\
	operator op(const cl & a, const cl & b) {		\
		return a.compareTo(b) op 0;					\
	}
	declareFunFromCompareTo(BasicBigInteger, ==)
	declareFunFromCompareTo(BasicBigInteger, !=)
	declareFunFromCompareTo(BasicBigInteger, <=)
	declareFunFromCompareTo(BasicBigInteger, >=)
	declareFunFromCompareTo(BasicBigInteger, <)
	declareFunFromCompareTo(BasicBigInteger, >)
#undef declareFunFromCompareTo

	const static BasicBigInteger ZERO;
	const static BasicBigInteger ONE;
	const static BasicBigInteger TEN;
};

template<typename Value, typename Calc> const char * const BasicBigInteger<Value, Calc>::digits = "0123456789abcdefghijklmnopqrstuvwxyz";
template<typename Value, typename Calc> const int BasicBigInteger<Value, Calc>::SHIFT = 8 * sizeof (Value);
template<typename Value, typename Calc> const Calc BasicBigInteger<Value, Calc>::INT_MASK = ~(~static_cast<Calc> (0) << SHIFT);
template<typename Value, typename Calc> const typename BasicBigInteger<Value, Calc>::Tool BasicBigInteger<Value, Calc>::tool;
template<typename Value, typename Calc> const BasicBigInteger<Value, Calc> BasicBigInteger<Value, Calc>::ZERO(0);
template<typename Value, typename Calc> const BasicBigInteger<Value, Calc> BasicBigInteger<Value, Calc>::ONE(1);
template<typename Value, typename Calc> const BasicBigInteger<Value, Calc> BasicBigInteger<Value, Calc>::TEN(10);

#if (defined(_BIG_INTEGER_DEBUG))
typedef BasicBigInteger<signed char, int> BigInteger;
#elif defined(__int64) || defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
typedef BasicBigInteger<int, __int64> BigInteger;
#else
typedef BasicBigInteger<int, long long> BigInteger;
#endif

#endif	/* BIGINTEGER_H */
