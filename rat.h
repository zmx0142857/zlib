#ifndef Rat_h
#define Rat_h

#if __cplusplus < 201101
#error "require c++11 or later"
#endif

#include <iostream>
#include <stdexcept>
#include "int.h"

// typedef int Int;

class Rat
{
	friend bool operator==(const Rat &, const Rat &);
	friend bool operator!=(const Rat &, const Rat &);
	friend bool operator<(const Rat &, const Rat &);
	friend bool operator>(const Rat &, const Rat &);
	friend bool operator<=(const Rat &, const Rat &);
	friend bool operator>=(const Rat &, const Rat &);

	friend Rat operator+(const Rat &, const Rat &);
	friend Rat operator-(const Rat &, const Rat &);
	friend Rat operator*(const Rat &, const Rat &);
	friend Rat operator/(const Rat &, const Rat &);

	friend std::ostream& operator<<(std::ostream &, const Rat &);
	friend std::istream& operator>>(std::istream &, Rat &);

	friend Rat abs(const Rat &);
	friend int sgn(const Rat &);	// return -1, 0 or 1

public:
	// construct a rational number values n / d + i.
	Rat(const Int &numerator=0,
		const Int &denominator=1,
		const Int &integer_part=0);
	Rat(double);
	explicit Rat(const std::string &);

    Rat &operator+=(const Rat &);
    Rat &operator-=(const Rat &);
    Rat &operator*=(const Rat &);
    Rat &operator/=(const Rat &);

	Rat operator+() const;
	Rat operator-() const;

	operator double() const;
	operator float() const;

	Int to_Int() const;
	long double to_double() const;
	std::string to_decimal() const;
	std::string to_string() const;
	std::string to_string2d() const;

	static Int gcd(Int, Int);	// greatest common denominator
	Rat approx(double);
	Int choose(Int n, Int k);
	Rat Bernoulli(int n);

	// test
	friend void test_io();
	friend void test_relation();
	friend void test_arithmetic();

protected:
	Int num;					// numerator
	Int den;					// denominator, guaranteed to be positive

	void reduce();				// fraction reduce, return void
	void validate();			// make sure the denominator is positive
};

#endif // Rat_h
