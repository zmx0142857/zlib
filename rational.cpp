// implementation for class rational

#include "rational.h"

//------------- constructors -----------------

void Rat::validate()
{
    if (den == 0)
		throw std::invalid_argument("demominator cannot be zero!");
	else if (den < 0) {
	    num = -num;
        den = -den;
    }
}

Rat::Rat(const Int &n, const Int &d, const Int &i): num(n + i * d), den(d)
{
	validate();
}

Rat::Rat(double d)
{
	// I wonder is there a function like 'is_integer'?
	// WARNING: this function fails if d is nearly infinity!!!
    for (den = 1; (long long)d != d; den *= 2)
		d *= 2;
	num = Int(d);
	reduce();
}

Rat::Rat(const std::string &str)
{
	size_t i, j;

	// i indicates '.' or '/' or end of str
	for (i = 0; i != str.size() && str[i] != '.' && str[i] != '/'; ++i) {}

	if (i == str.size()) {
		// take as integer
		// j indicates 'e' or 'E' or end of str
		for (j = 0; j != str.size() && str[j] != 'e' && str[j] != 'E'; ++j) {}
		num = Int(std::string(str, 0, j));
		den = 1;
		Int exp((j != str.size()) ? Int(std::string(str, j+1)) : 0);
		if (exp < 0)
			den *= Int(10).pow(-exp);
		else
			num *= Int(10).pow(exp);
	} else if (str[i] == '.') {
		// take as decimal
		// j indicates 'e' or 'E' or end of str, j-i >= 1
		for (j = i+1; j != str.size() && str[j] != 'e' && str[j] != 'E'; ++j) {}
		num = Int(std::string(str, 0, i) + std::string(str, i+1, j-i-1));
		den = 1;
		Int exp(j-i-1 - ((j != str.size()) ? Int(std::string(str, j+1)) : 0));
		if (exp < 0)
			num *= Int(10).pow(-exp);
		else
			den *= Int(10).pow(exp);
	} else { // if (str[i] == '/')
		// take as rational
		num = Int(std::string(str, 0, i));
		den = Int(std::string(str, i+1));
		validate();
	}
	reduce();
}

//--------------- reduce -----------------

void Rat::reduce()
{
	Int d = gcd(num, den);
	//std::cout << "num1: " << num << " den1: " << den << std::endl;
	//std::cout << "gcd(num, den): " << d << std::endl;
	num /= d;
	den /= d;
	//std::cout << "num2: " << num << " den2: " << den << std::endl;
}

//---------------- io ------------------

std::ostream &operator<<(std::ostream &os, const Rat &rhs)
{
	if (rhs.den == 1)
		os << rhs.num;
	else
		os << rhs.num << '/' << rhs.den;
	return os;
}

std::istream &operator>>(std::istream &is, Rat &rhs)
{
	std::string str;
	is >> str;
	try {
		rhs = Rat(str);
	} catch (const std::invalid_argument &) {
		rhs = 0;
		is.setstate(std::istream::failbit);
	}
	return is;
}

//--------------- relationship -----------------

bool operator==(const Rat &lhs, const Rat &rhs)
{
    return lhs.num * rhs.den == lhs.den * rhs.num;
}

bool operator!=(const Rat &lhs, const Rat &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Rat &lhs, const Rat &rhs)
{
	return lhs.num * rhs.den < lhs.den * rhs.num;
}

bool operator>(const Rat &lhs, const Rat &rhs)
{
	return rhs < lhs;
}

bool operator<=(const Rat &lhs, const Rat &rhs)
{
	return !(rhs < lhs);
}

bool operator>=(const Rat &lhs, const Rat &rhs)
{
	return !(lhs < rhs);
}

//-------------- arithmetic ---------------

Rat operator+(const Rat &lhs, const Rat &rhs)
{
    //Int d = Rat::gcd(lhs.den, rhs.den);
    //Rat ret(lhs.den / d * rhs.num + rhs.den / d * lhs.num, lhs.den / d * rhs.den);
	Rat ret(lhs.den * rhs.num + rhs.den * lhs.num, lhs.den * rhs.den);
	ret.reduce();
    return ret;
}

Rat operator-(const Rat &lhs, const Rat &rhs)
{
    //Int d = Rat::gcd(lhs.den, rhs.den);
    //Rat ret(-lhs.den/d * rhs.num + rhs.den/d * lhs.num, lhs.den/d * rhs.den);
	Rat ret(-lhs.den * rhs.num + rhs.den * lhs.num, lhs.den * rhs.den);
	ret.reduce();
    return ret;
}

Rat operator*(const Rat &lhs, const Rat &rhs)
{
	Rat a(rhs.num, lhs.den), b(lhs.num, rhs.den);
	a.reduce();
	b.reduce();
	return Rat(a.num * b.num, a.den * b.den);
}

Rat operator/(const Rat &lhs, const Rat &rhs)
{
	Rat a(lhs.num, rhs.num), b(lhs.den, rhs.den);
	a.reduce();
	b.reduce();
	Rat ret = Rat(a.num * b.num, a.den * b.den);
	return ret;
}

Rat &Rat::operator+=(const Rat &rhs)
{
    *this = *this + rhs;
    return *this;
}

Rat& Rat::operator-=(const Rat &rhs)
{
    *this = *this - rhs;
    return *this;
}

Rat& Rat::operator*=(const Rat &rhs)
{
    *this = *this * rhs;
    return *this;
}

Rat& Rat::operator/=(const Rat &rhs)
{
    *this = *this / rhs;
    return *this;
}

Rat Rat::operator+() const
{
	return *this;
}

Rat Rat::operator-() const
{
	return Rat(-num, den);
}

Int to_Int() const
{
	return num / den;
}

long double to_double() const
{
	return num.to_double() / den.to_double;
}

Int Rat::gcd(Int a, Int b)
{
	if (a < 0) a = -a;
	if (b < 0) b = -b;
	while (b != 0) {
		Int c = a % b;
		a = b;
		b = c;
	}
	return a;
}

//---------- test -------------

#ifdef DEBUG
using namespace std;

void Rat::test_io()
{
	Rat a;
	while (cin >> a)
		cout << a << endl;
}

void Rat::test_relation()
{
	Rat a, b;
	string op;
	while (cin >> a >> op >> b) {
		if (op == "==")			cout << (a == b) << endl;
		else if (op == "!=")	cout << (a != b) << endl;
		else if (op == "<")		cout << (a < b) << endl;
		else if (op == ">")		cout << (a > b) << endl;
		else if (op == "<=")	cout << (a <= b) << endl;
		else if (op == ">=")	cout << (a >= b) << endl;
	}
}

void Rat::test_arithmetic()
{
	Rat a, b; char op;
	while (cin >> a >> op >> b) {
		switch (op) {
			case '+': cout << a + b << endl; break;
			case '-': cout << a - b << endl; break;
			case '*': cout << a * b << endl; break;
			case '/': cout << a / b << endl; break;
		}
	}
}

#endif
