#ifndef Rational_h
#define Rational_h

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

	friend Int abs(const Int &);
	friend int sgn(const Int &);	// return -1, 0 or 1

public:
	// construct a rational number values n / d + i.
	Rat(const Int &numerator=0, const Int &denominator=1, const Int &integer_part=0);
	Rat(double);
	explicit Rat(const std::string &);

    Rat &operator+=(const Rat &);
    Rat &operator-=(const Rat &);
    Rat &operator*=(const Rat &);
    Rat &operator/=(const Rat &);

	Rat operator+() const;
	Rat operator-() const;

	int to_int() const;
	double to_double() const;
	std::string to_decimal() const;
	std::string to_string() const;
	std::string to_string2d() const;

	static Int gcd(Int, Int);	// greatest common denominator
	Rat approx(double);
	Int choose(Int n, Int k);
	Rat Bernoulli(int n);

	static void test_io();
	static void test_relation();
	static void test_arithmetic();

protected:
	Int num;					// numerator
	Int den;					// denominator, guaranteed to be positive

	void reduce();				// fraction reduce, return void
	void check_valid();			// make sure the denominator is positive
};

#endif // Rational_h
