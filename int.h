#ifndef Int_h
#define Int_h

#if __cplusplus < 201101
#error "require c++11 or later"
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>	// reverse()
#include <cctype>		// isdigit()
#include <cinttypes>	// int64_t, uint64_t,  uint32_t
#include <climits>		// LLONG_MAX
#include <cfloat>		// LDBL_MAX
#include <stdexcept>	// domain_error argument_error

/* Instances represent arbitrarily large integers.
 * Invariants:	Positive integers are stored in an vector 'data', with 
 *				base 2^30; higher digits are stored at the end;
 *				Negative integers are represented by neg == true.
 *				Zero is represented by data.size() == 0, neg == false.
 *				In a normalized number, for all valid i,
 *				0 <= data[i] < BASE.
 * Example:		123456789101112131415 is stored as
 *				data: [437976919, 87719511, 107]
 *				neg: false
 */
class Int
{
	public:
		// io, the scanning stops when meets first non-digit character
		friend std::istream &operator>>(std::istream &, Int &);
		friend std::ostream &operator<<(std::ostream &, const Int &);

		// relationship
		friend bool operator==(const Int &, const Int &);
		friend bool operator!=(const Int &, const Int &);
		friend bool operator<(const Int &, const Int &);
		friend bool operator>(const Int &, const Int &);
		friend bool operator<=(const Int &, const Int &);
		friend bool operator>=(const Int &, const Int &);

		// arithmetics
		friend Int operator+(const Int &, const Int &);
		friend Int operator-(const Int &, const Int &);
		friend Int operator*(const Int &, const Int &);
		friend Int operator/(const Int &, const Int &);
		friend Int operator%(const Int &, const Int &);
		Int pow(const Int &, const Int &modulo=0);

		// other friends
		friend Int abs(const Int &);
		friend int sgn(const Int &);	// return -1, 0 or 1
		friend Int divmod(const Int &, const Int &, Int &residue);

		// constructors
		Int(uint64_t=0ull);
		Int(uint32_t);
		Int(int64_t);
		Int(int);
		explicit Int(long double);	// may lost precision
		explicit Int(double);		// may lost precision
		explicit Int(float);		// may lost precision
		Int(const std::string &);

		// swapping
		void swap(Int &);

		// arithmetic assignment
		Int &operator+=(const Int &);
		Int &operator-=(const Int &);
		Int &operator*=(const Int &);
		Int &operator/=(const Int &);
		Int &operator%=(const Int &);

		// unary operators
		Int operator+() const;		// returns a copy of self
		Int operator-() const;
		Int &operator++();
		Int &operator--();
		Int operator++(int);		// postfix increment
		Int operator--(int);		// postfix decrement

		// bit operations
		// Warning: negative numbers are not stored as two's complement
		friend Int operator&(const Int &, const Int &);
		friend Int operator|(const Int &, const Int &);
		friend Int operator^(const Int &, const Int &);
		Int operator~() const;
		Int operator<<(size_t);
		Int operator>>(size_t);
		Int &operator&=(const Int &);
		Int &operator|=(const Int &);
		Int &operator^=(const Int &);
		Int &operator<<=(size_t);
		Int &operator>>=(size_t);

		// conversion
		int64_t to_int() const;		// return LLONG_MAX for overflow,
									// LLONG_MIN for underflow
		long double to_double() const;
		std::string to_string() const;

		// other methods
		bool is_zero() const;

		// static consts
		static const Int ZERO;
		static const size_t WIDTH;
		static const uint32_t BASE;
		static const uint32_t MASK;

		// test
		friend void test_io();
		friend void test_arithmetic();
		friend void test_factorial();
		friend void test_bit();	
		friend void test_to_int();
		friend void test_add();

	protected:
		std::vector<uint32_t> data;	// array of digits base 10^WIDTH
		bool neg;

		// typedefs
		typedef std::vector<uint32_t>::iterator It;
		typedef std::vector<uint32_t>::const_iterator CIt;
		typedef std::vector<uint32_t>::reverse_iterator RIt;
		typedef std::vector<uint32_t>::const_reverse_iterator CRIt;

		// protected functions
		std::string dump() const;
		Int(CIt beg, CIt end, bool=false);		// init from a pair of iterators
		void construct(uint64_t);				// help with construction
		void remove_zeros();					// remove leading zeros
		static int compare(CIt, CIt, CIt, CIt);	// help with comparison
		static int compare_abs(const Int &, const Int &);
												// help with comparison
		static Int &add(CIt, CIt, CIt, CIt, Int &);	// help with operator +=
		Int &add_by(const Int &);				// help with operator +=
		static void subtract(CIt, CIt, CIt, CIt, Int &, It);
												// help with operator -=
		static Int &subtract(CIt, CIt, CIt, CIt, Int &);
												// help with operator -=
		Int &subtract_by(const Int &);			// help with operator -=
		Int &subtract_from(const Int &);		// help with operator -=
		static void mul(CIt, CIt, CIt, CIt, Int &);
												// help with operator *
		static void recursive_mul(CIt, CIt, CIt, CIt, Int &);
												// help with operator *
		static Int divmod(Int &, const Int &);
												// help with operator / and  %
};

#endif // Int_h
