// Implementation for 'int.h'
#include "int.h"

//---------------- io --------------------

std::string Int::dump() const
{
	std::ostringstream ss;
	ss << '[';
	CRIt rit = data.rbegin();
	if (rit != data.rend())
		ss << *rit++;
	while (rit != data.rend())
		ss << " ," << *rit++;
	ss << ']';
	return ss.str();
}

std::ostream &operator<<(std::ostream &os, const Int &rhs)
{
	return os << rhs.to_string();
}

std::istream &operator>>(std::istream &is, Int &rhs)
{
	std::string s;
	is >> s;
	try
	{
		rhs = Int(s);
	}
	catch (std::invalid_argument)
	{
		rhs = 0;
		is.setstate(std::istream::failbit);
	}
	return is;
}

//--------------- relationship -----------------

bool operator==(const Int &lhs, const Int &rhs)
{
	return lhs.neg == rhs.neg && lhs.data == rhs.data;
}

bool operator!=(const Int &lhs, const Int &rhs) { return !(lhs == rhs); }

// compare absolute value of Int(lbeg, lend) and Int(rbeg, rend)
// return -1 if left < right
// return 1  if left > right
// return 0  if left == right
int Int::compare(CIt lbeg, CIt lend, CIt rbeg, CIt rend)
{
	size_t lsz = lend - lbeg, rsz = rend - rbeg;
	if (lsz < rsz)	return -1;
	if (lsz > rsz)	return 1;

	--lend; --rend;
	while (lend != lbeg-1)
	{
		if (*lend < *rend)	return -1;
		if (*lend > *rend)	return 1;
		--lend; --rend;
	}
	return 0;
}

inline int Int::compare_abs(const Int &lhs, const Int &rhs)
{
	return compare(lhs.data.begin(), lhs.data.end(), rhs.data.begin(),
			rhs.data.end());
}

bool operator<(const Int &lhs, const Int &rhs)
{
	if (lhs.neg && !rhs.neg)	return true;
	if (!lhs.neg && rhs.neg)	return false;
	if (lhs.neg && rhs.neg)		return Int::compare_abs(lhs, rhs) > 0;
	return Int::compare_abs(lhs, rhs) < 0;
}

inline bool operator>(const Int &lhs, const Int &rhs) { return rhs < lhs; }
inline bool operator<=(const Int &lhs, const Int &rhs) { return !(rhs < lhs); }
inline bool operator>=(const Int &lhs, const Int &rhs) { return !(lhs < rhs); }

//-------------- arithmetics ----------------------

Int operator+(const Int &lhs, const Int &rhs)
{
	Int ret(lhs);
	return ret += rhs;
}

Int operator-(const Int &lhs, const Int &rhs)
{
	Int ret(lhs);
	return ret -= rhs;
}

// compute product of two non-negative Int [lbeg, lend) * [rbeg, rend)
// the result is stored in dest, which must be passed in as Int::ZERO
// use common algorithm
void Int::mul(CIt lbeg, CIt lend, CIt rbeg, CIt rend, Int &dest)
{
	if (lbeg == lend || rbeg == rend)
		return;

//	L\R   * ← * ← *
//
//	 *    *   *   *
//	 ↓      /   /
//	 *    *   *   *
//	 ↓      /   /
//	 *    *   *   *

	uint64_t sum = 0ull;
	CIt lit = lbeg, rit = rbeg-1, it1, it2;
	while (++rit != rend)
	{
		for (it1 = lit, it2 = rit; it1 != lend && it2 != rbeg-1; ++it1, --it2)
		{
			sum += uint64_t(*it1) * uint64_t(*it2);
		}
		dest.data.push_back(sum & Int::MASK);	// sum % BASE
		sum >>= Int::WIDTH;						// sum /= BASE
	}

	// now lit == lbeg, rit = rend
	--rit;
	while (++lit != lend)
	{
		for (it1 = lit, it2 = rit; it1 != lend && it2 != rbeg-1; ++it1, --it2)
		{
			sum += uint64_t(*it1) * uint64_t(*it2);
		}
		dest.data.push_back(sum & Int::MASK);
		sum >>= Int::WIDTH;
	}

	while (sum)
	{
		dest.data.push_back(sum & Int::MASK);
		sum >>= Int::WIDTH;
	}
}

// compute product of two non-negative Int [lbeg, lend) * [rbeg, rend)
// the result is stored in dest, which must be passed in as Int::ZERO
// use a more efficient algorithm:
// Let A, B be integers with length n,
//
// A: lend--(a)--lbeg+m--(b)--lbeg
// B: rend--(c)--rbeg+m--(d)--rbeg
//
// A = am + b, B = cm + d; a, b, c, d are intergers with length n/2. then
// A*B = (a + bm) * (c + dm) = ac m^2 + ((a+b)(c+d) - (ac+bd)) m + bd.
// Again we divide the n/2 sized intergers into n/4...
// This algorithm has complexity O(n^(lg 3)) rather than O(n^2).
void Int::recursive_mul(CIt lbeg, CIt lend, CIt rbeg, CIt rend, Int &dest)
{
	const size_t M = 20;
	size_t lsz = lend - lbeg, rsz = rend - rbeg;
	if (lsz < M || rsz < M)
	{
		mul(lbeg, lend, rbeg, rend, dest);
		return;
	}

	size_t m = (lsz < rsz ? lsz / 2 : rsz / 2);
	Int &ac = dest, bd, apb, cpd, ef;
	recursive_mul(lbeg + m, lend, rbeg + m, rend, ac);
	recursive_mul(lbeg, lbeg + m, rbeg, rbeg + m, bd);
	add(lbeg + m, lend, lbeg, lbeg + m, apb);
	add(rbeg + m, rend, rbeg, rbeg + m, cpd);
	recursive_mul(apb.data.begin(), apb.data.end(), cpd.data.begin(),
			cpd.data.end(), ef);
	ef -= ac; ef -= bd;
	ac.data.insert(ac.data.begin(), m, 0ul);
	ac += ef;
	ac.data.insert(ac.data.begin(), m, 0ul);
	ac += bd;
}

Int operator*(const Int &lhs, const Int &rhs)
{
	Int ret;
	ret.neg = lhs.neg ^ rhs.neg;
	Int::recursive_mul(lhs.data.begin(), lhs.data.end(), rhs.data.begin(),
			rhs.data.end(), ret);
	return ret;
}

// suppose lhs, rhs >= 0
// compute lhs / rhs as well as lhs % rhs.
// return quotient
// lhs becomes residue after the call.
Int Int::divmod(Int &lhs, const Int &rhs)
{
	if (rhs.is_zero())
		throw std::domain_error("division by zero!");
	if (compare_abs(lhs, rhs) < 0)
		return ZERO;

	// during each iteration, take one more digit from lhs.
	// let q be the highest digit of quotient, then q subjects to:
	// floor(lback / (rback + 1)) <= q <= floor(lback / rback)
	// where lback = lhs.data.back(), rback = rhs.data.back()

	Int quotient;
	for (It left = lhs.data.end() - rhs.data.size();
			left >= lhs.data.begin(); --left)
	{
		CIt right = lhs.data.end();

		// if lhs part < rhs, move on
		if (right < left + rhs.data.size()
			|| compare(left, right, rhs.data.begin(), rhs.data.end()) < 0)
		{
			quotient.data.push_back(0ul);
			continue;
		}

		// it is guaranteed that lhs.data and rhs.data not empty,
		// and lback, rback != 0
		uint32_t lback = lhs.data.back(), rback = rhs.data.back();
		uint32_t lo, hi;
		if (right == left + rhs.data.size())
		{
			lo = lback / (rback + 1);
			hi = lback / rback + 1;
		}
		else // borrowed
		{
			lo = BASE / (rback + 1) * lback;
			hi = (BASE / rback + 1) * (lback + 1);
		}

		// try to locate [lo, hi)
		Int prod = rhs * lo;
		while (lo + 1 < hi)
		{
			uint32_t mid = (lo + hi) >> 1; // (lo + hi) / 2
			Int tmp = rhs * mid;
			if (compare(left, right, tmp.data.begin(), tmp.data.end()) < 0)
			{
				hi = mid;
			}
			else
			{
				lo = mid;
				prod.swap(tmp);
			}
		}
		quotient.data.push_back(lo);
		// extra zeros of lhs are poped in process of subtract()
		subtract(left, right, prod.data.begin(), prod.data.end(), lhs, left);
	}
	reverse(quotient.data.begin(), quotient.data.end());
	quotient.remove_zeros();
	quotient.neg = lhs.neg ^ rhs.neg;
	// there is no such '-0'
	if (lhs.is_zero())
		lhs.neg = false;
	return quotient;
}

// a friend-overloaded version of the function above
Int divmod(const Int &lhs, const Int &rhs, Int &residue)
{
	residue = lhs;
	return Int::divmod(residue, rhs);
}

Int operator/(const Int &lhs, const Int &rhs)
{
	Int residue(lhs);
	return Int::divmod(residue, rhs);
}

Int operator%(const Int &lhs, const Int &rhs)
{
	Int residue(lhs);
	Int::divmod(residue, rhs);
	return residue;
}

// for example, a^11 = a^(2^3 + 2^1 + 2^0)
Int Int::pow(const Int &exp, const Int &modulo)
{
	Int ret(1);
	if (modulo.is_zero())
	{
		for (CRIt rit = exp.data.rbegin(); rit != exp.data.rend(); ++rit)
		{
			for (size_t shift = 0; shift != WIDTH; ++shift)
			{
				ret *= ret;
				if (*rit & (1ul << (WIDTH-1-shift)))
				{
					ret *= (*this);
				}
			}
		}
	}
	else
	{
		for (CRIt rit = exp.data.rbegin(); rit != exp.data.rend(); ++rit)
		{
			for (size_t shift = 0; shift != WIDTH; ++shift)
			{
				ret *= ret;
				ret %= modulo;
				if (*rit & (1ul << (WIDTH-1-shift)))
				{
					ret *= (*this);
					ret %= modulo;
				}
			}
		}
	}
	return ret;
}

//------------ unary functions --------------

Int abs(const Int &x)
{
	Int ret(x);
	ret.neg = false;
	return ret;
}

int sgn(const Int &x)
{
	if (x.neg)			return -1;
	if (x.is_zero())	return 0;
	return 1;
}

//------------ constructors ---------------

Int::Int(CIt beg, CIt end, bool neg_flag):
	data(std::vector<uint32_t>(beg, end)), neg(neg_flag) {}

void Int::construct(uint64_t u)
{
	// break u into integers in range [0, BASE)
	while (u != 0ull)
	{
		data.push_back(u % BASE);
		u /= BASE;
	}
}

Int::Int(uint64_t u): data(), neg(false)
{
	construct(u);
}

Int::Int(uint32_t u): data(), neg(false)
{
	construct(u);
}

Int::Int(int64_t i): data(), neg(i < 0)
{
	construct(i < 0 ? -i : i);
}

Int::Int(int i): data(), neg(i < 0)
{
	construct(i < 0 ? -i : i);
}

Int::Int(long double d): data(), neg(d < 0)
{
	construct(d < 0 ? -d : d);
}

Int::Int(double d): data(), neg(d < 0)
{
	construct(d < 0 ? -d : d);
}

Int::Int(float d): data(), neg(d < 0)
{
	construct(d < 0 ? -d : d);
}

// convert string to Int
// acceptable: "-123" "123456" "+123"
// not acceptable: "+ 123" " - 123" "++123" "--123" "123++" ""
Int::Int(const std::string &str): data(), neg(false)
{
	// step 1: find the digits substring
	// b is the index of the first digit (or the end) of s
	// e is the index of the first non-digit after b (or the end of s)
	// so [b, e) is marks range of the digits string
	std::string s(str);
	std::string::size_type b = 0, e;
	if (b != s.size() && !isdigit(s[b]))
	{
		switch (s[b])
		{
			case '-': neg = true; // no break here
			case '+': ++b; break;
			default: throw std::invalid_argument("the first character must be a "
							 "digit, or '-', or '+'.");
		}
	}

	if (b == s.size() || !isdigit(s[b]))
		throw std::invalid_argument("the character following the sign (or, "
				"the first character if there is no sign) must be a digit.");

	for (e = b; e != s.size() && isdigit(s[e]); ++e)
		s[e] -= '0';

	if (e != s.size())
		throw std::invalid_argument("the string contains invalid trailing "
			"character(s).");

	// step 2: convert the base 10 digits to base 2^30 by modulo 2
	while (b != e)
	{
		uint32_t digit = 0ul;
		for (size_t shift = 0; shift != WIDTH; ++shift)
		{
			// remove leading zeros, note that "0" becomes ""
			while (b != e && s[b] == 0) ++b;
			if (b == e) break;

			char borrow = 0;
			for (auto i = b; i != e; ++i)
			{
				char tmp = s[i];
				s[i] = s[i] / 2 + borrow;
				borrow = (tmp % 2) ? 5 : 0;
			}
			if (borrow)
				digit |= (1 << shift);
		}
		if (digit || b != e)
			data.push_back(digit);
	}

	// there's no such "-0"
	if (is_zero())
		neg = false;
}

//---------- swapping ------------

void Int::swap(Int &rhs)
{
	bool tmp = neg;
	neg = rhs.neg;
	rhs.neg = tmp;
	data.swap(rhs.data);
}

//----------- arithmetic assignment -----------

// compute Int(lbeg, lend) + Int(rbeg, rend), the result is stored in dest
Int &Int::add(CIt lbeg, CIt lend, CIt rbeg, CIt rend, Int &dest)
{
	uint32_t sum = 0ul;
	It it = dest.data.begin();
	while (lbeg != lend || rbeg != rend)
	{
		// sum, *lit, *rit < BASE == 2^30
		// so sum + *lit + *rit < 2^32
		uint32_t lu = (lbeg != lend ? *lbeg++ : 0ul);
		uint32_t ru = (rbeg != rend ? *rbeg++ : 0ul);
		sum += lu + ru;
		if (it != dest.data.end())
		{
			*it = (sum & MASK);		// sum % BASE
			++it;
		}
		else
		{
			dest.data.push_back(sum & MASK);
			it = dest.data.end();
		}
		sum >>= WIDTH;				// sum /= BASE
	}
	if (sum)
	{
		if (it == dest.data.end())
			dest.data.push_back(sum);
		else
			*it = sum;
	}
	return dest;
}

// compute abs(*this) += abs(rhs), with neg = this->neg
inline Int &Int::add_by(const Int &rhs)
{
	return add(this->data.begin(), this->data.end(), rhs.data.begin(),
			rhs.data.end(), *this);
}

Int &Int::operator+=(const Int &rhs)
{
	if (is_zero())			return *this = rhs;	// copy
	if (rhs.is_zero())		return *this;
	if (neg == rhs.neg)		return this->add_by(rhs);

	// if this->neg != rhs.neg
	int cmp = compare_abs(*this, rhs);
	if (cmp > 0) return this->subtract_by(rhs);
	if (cmp < 0) return this->subtract_from(rhs);
	return *this = ZERO;
}

// suppose Int(lbeg, lend) > Int(rbeg, rend)
// compute Int(lbeg, lend) - Int(rbeg, rend),
// the result is stored in dest, the memory block begins with beg
void Int::subtract(CIt lbeg, CIt lend, CIt rbeg, CIt rend, Int &dest, It beg)
{
	uint32_t diff = 0ul, borrow = 0ul, sub = 0ul;

	// due to suppose, L-size > R-size
	while (lbeg != lend)
	{
		sub = (rbeg != rend ? *rbeg++ : 0ul) + borrow;
		borrow = (*lbeg < sub);
		// *lbeg + BASE < 2*BASE = 2^31, so uint32_t is enough
		diff = (*lbeg++) + (borrow ? BASE : 0ul) - sub;
		if (beg != dest.data.end())
		{
			*beg = diff;
			++beg;
		}
		else
		{
			dest.data.push_back(diff);
		}
	}
	dest.remove_zeros();
}

// overloaded version of the function above
inline Int &Int::subtract(CIt lbeg, CIt lend, CIt rbeg, CIt rend, Int &dest)
{
	subtract(lbeg, lend, rbeg, rend, dest, dest.data.begin());
	return dest;
}

// suppose abs(*this) > abs(rhs)
// the result is abs(*this) - abs(rhs), with neg = this->neg
inline Int &Int::subtract_by(const Int &rhs)
{
	return subtract(this->data.begin(), this->data.end(),
			rhs.data.begin(), rhs.data.end(), *this);
}

// suppose abs(*this) < abs(rhs)
// the result is abs(rhs) - abs(*this), with neg = !this->neg
Int &Int::subtract_from(const Int &rhs)
{
	// flip the sign
	neg = !neg;
	return subtract(rhs.data.begin(), rhs.data.end(), this->data.begin(),
			this->data.end(), *this);
}

Int &Int::operator-=(const Int &rhs)
{
	if (is_zero())			return *this = -rhs;	// copy may be reduced
	if (rhs.is_zero())		return *this;
	if (neg != rhs.neg)		return this->add_by(rhs);

	// now this->neg == rhs.neg
	int cmp = compare_abs(*this, rhs);
	if (cmp > 0)	return this->subtract_by(rhs);
	if (cmp < 0)	return this->subtract_from(rhs);
	return *this = ZERO;
}

Int &Int::operator*=(const Int &rhs)
{
	Int tmp;
	tmp.neg = this->neg ^ rhs.neg;
	Int::recursive_mul(this->data.begin(), this->data.end(), rhs.data.begin(),
			rhs.data.end(), tmp);
	swap(tmp);
	return *this;
}

Int &Int::operator/=(const Int &rhs)
{
	Int quotient = divmod(*this, rhs);
	swap(quotient);
	return *this;
}

Int &Int::operator%=(const Int &rhs)
{
	divmod(*this, rhs);
	return *this;
}

//---------- unary operators ------------

Int Int::operator+() const
{
	return Int(*this);
}

Int Int::operator-() const
{
	Int ret(*this);
	if (!is_zero())
		ret.neg = !neg;
	return ret;
}

Int &Int::operator++()
{
	*this += 1;
	return *this;
}

Int &Int::operator--()
{
	*this -= 1;
	return *this;
}

Int Int::operator++(int)
{
	Int ret(*this);
	++(*this);
	return ret;
}

Int Int::operator--(int)
{
	Int ret(*this);
	--(*this);
	return ret;
}

//--------------- bit operations ------------

void Int::remove_zeros()
{
	while (!data.empty() && data.back() == 0)
		data.pop_back();
}

Int operator&(const Int &lhs, const Int &rhs)
{
	Int ret(lhs);
	ret &= rhs;
	return ret;
}

Int operator|(const Int &lhs, const Int &rhs)
{
	Int ret(lhs);
	ret |= rhs;
	return ret;
}

Int operator^(const Int &lhs, const Int &rhs)
{
	Int ret(lhs);
	ret ^= rhs;
	return ret;
}

Int &Int::operator&=(const Int &rhs)
{
	It lit = data.begin();
	CIt rit = rhs.data.begin();
	while (lit != data.end() && rit != rhs.data.end())
	{
		(*lit) &= (*rit);
		++lit, ++rit;
	}
	data.erase(lit, data.end());
	remove_zeros();
	return *this;
}

Int &Int::operator|=(const Int &rhs)
{
	It lit = data.begin();
	CIt rit = rhs.data.begin();
	while (lit != data.end() && rit != rhs.data.end())
	{
		(*lit) |= (*rit);
		++lit, ++rit;
	}
	while (rit != rhs.data.end())
		data.push_back(*rit++);
	return *this;
}

Int &Int::operator^=(const Int &rhs)
{
	It lit = data.begin();
	CIt rit = rhs.data.begin();
	while (lit != data.end() && rit != rhs.data.end())
	{
		(*lit) ^= (*rit);
		++lit, ++rit;
	}
	while (lit != data.end())
	{
		*lit = ~(*lit);
		++lit;
	}
	while (rit != rhs.data.end())
		data.push_back(~(*rit++));
	remove_zeros();
	return *this;
}

Int Int::operator~() const
{
	Int ret;
	for (CIt cit = data.begin(); cit != data.end(); ++cit)
	{
		ret.data.push_back(~(*cit));
	}
	ret.remove_zeros();
	return ret;
}

Int Int::operator<<(size_t shift)
{
	Int ret(*this);
	ret <<= shift;
	return ret;
}

Int Int::operator>>(size_t shift)
{
	Int ret(*this);
	ret >>= shift;
	return ret;
}

Int &Int::operator<<=(size_t shift)
{
	if (data.empty()) return *this;

	size_t blocks = shift / WIDTH;
	size_t moves = shift % WIDTH;
	if (blocks)
		data.insert(data.begin(), blocks, 0ul);
	if (moves)
	{
		size_t remains = WIDTH - moves;
		data.push_back(data.back() >> remains);
		RIt it = data.rbegin(), rlast = data.rend()-1;
		while (it != rlast)
		{
			*it <<= moves;
			*it |= (*(it+1) >> remains);
			*it &= MASK;
			++it;
		}
		*it <<= moves;
	}
	return *this;
}

Int &Int::operator>>=(size_t shift)
{
	size_t blocks = shift / WIDTH;
	size_t moves = shift % WIDTH;
	if (blocks)
		data.erase(data.begin(), data.begin() + blocks);
	if (moves)
	{
		size_t remains = WIDTH - moves;
		It it = data.begin(), last = data.end()-1;
		while (it != last)
		{
			*it >>= moves;
			*it |= (*(it+1) << remains);
			*it &= MASK;
			++it;
		}
		*it >>= moves;
	}
	return *this;
}

//--------------- conversion ---------------

int64_t Int::to_int() const
{
	int64_t ret = 0LL;
	CRIt it = data.rbegin();
	while (it != data.rend())
	{
		// for n in N and x in R
		// n <= x if and only if n < [x] + 1
		if (ret < (LLONG_MAX - *it) / BASE + 1)
			ret = BASE * ret + (*it++);
		else
			return neg ? LLONG_MIN : LLONG_MAX; // overflow
	}
	return neg ? -ret : ret;
}

long double Int::to_double() const
{
	long double ret = 0.0;
	CRIt it = data.rbegin();
	while (it != data.rend())
	{
		if (ret < (LDBL_MAX - *it) / BASE)
			ret = BASE * ret + (*it++);
		else
		{
			ret = 1.0 / 0.0; // ret = inf
			break;
		}
	}
	return neg ? -ret : ret;
}

std::string Int::to_string() const
{
	// convert binary bits to a decimal string (lower digits first)
	// it's good news that multi-carry can't happen in this case, for
	// example 89629569 * 2:
	// 111 1111
	//  68248028
	// ---------
	// 179259138
	std::string ret("");
	ret += char(0);
	for (CRIt rit = data.rbegin(); rit != data.rend(); ++rit)
	{
		for (uint32_t mask = 1 << (WIDTH-1); mask != 0ul; mask >>= 1)
		{
			char carry = (*rit & mask ? 1 : 0);
			// decimal string *= 2
			for (std::string::size_type i = 0; i != ret.length(); ++i)
			{
				char tmp = ret[i];
				ret[i] = (ret[i] * 2 + carry) % 10;
				carry = tmp > 4 ? 1 : 0;
			}
			if (carry)
				ret += char(1);
		}
	}

	// add '0' and reverse
	for (std::string::size_type i = 0; i != ret.length(); ++i)
		ret[i] += '0';
	if (neg) ret += '-';
	reverse(ret.begin(), ret.end());
	return ret;
}

//---------- other methods --------------

inline bool Int::is_zero() const
{
	return data.empty();
}

//--------------- static consts ------------------

const Int Int::ZERO;					// by default constructor
const size_t Int::WIDTH = 30;			// bits per digit
const uint32_t Int::BASE = 1 << WIDTH;	// 2^30 = 1073741824
const uint32_t Int::MASK = BASE - 1;	// 30 '1's in memory

//------------- test --------------

using namespace std;
void Int::test_io()
{
	Int a;
	while (cin >> a)
		cout << a << endl;
}

void Int::test_arithmetic()
{
	Int a, b, r; char op;
	while (cin >> a >> op >> b)
	{
		switch (op)
		{
			case '<': cout << (a < b) << endl; break;
			case '+': cout << a + b << endl; break;
			case '-': cout << a - b << endl; break;
			case '*': cout << a * b << endl; break;
			case '/': cout << ::divmod(a, b, r) << ", ";
					  cout << r << endl; break;
			case '^': cout << a.pow(b) << endl; break;
		}
	}
}

void Int::test_factorial(int n)
{
	// compute (99!) = 933262154439441526816992388562667004907159682643816214685929638952175999932299156089414639761565182862536979208272237582511852109168640000000000000000000000
	Int prod = 1;
	while (n != 0)
		prod *= (n--);
	cout << prod << endl;
}

void Int::test_bit()
{
	Int a, b; char op;
	while (cin >> a >> op >> b)
	{
		switch (op)
		{
			case '<': cout << (a << b.to_int()) << endl; break;
			case '>': cout << (a >> b.to_int()) << endl; break;
			case '^': cout << (a ^ b) << endl; break;
			case '&': cout << (a & b) << endl; break;
			case '|': cout << (a | b) << endl; break;
			case '~': cout << (~a) << endl; break;
		}
	}
}

void Int::test_to_int()
{
	Int a;
	cout << "LLONG_MAX =\n" << LLONG_MAX << '\n';
	while (cin >> a)
		cout << a.to_int() << endl;
}

void Int::test_add()
{
	Int a, b, dest;
	while (cin >> a >> b)
	{
		add(a.data.begin(), a.data.end(), b.data.begin(), b.data.end(), dest);
		cout << dest << endl;
	}
}
