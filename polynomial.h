#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <list>
#include <initializer_list>
#include <queue> // priority_queue

template <class T>
struct Term {

	static char sym;
	static bool highest_term;

	T coef;		// coefficient
	int exp;	// exponent

	Term(const T &c, int e): coef(c), exp(e) {}

	// return derivate of *this
	Term derivate() const
	{
		return Term(coef * exp, exp - 1);
	}

	void sign(std::ostream &os) const
	{
		if (highest_term) {
			highest_term = false;
			os << (coef < 0 ? "-" : "");
		} else {
			os << (coef < 0 ? " - " : " + ");
		}
	}

	void coefficient(std::ostream &os) const
	{
		T abscoef = coef < 0 ? -coef : coef;
		if (exp == 0 || abscoef != 1)
			os << abscoef;
	}

	void exponent(std::ostream &os) const
	{
		if (exp == 0)
			;
		else if (exp == 1)
			os << sym;
		else
			os << sym << '^' << exp;
	}
};

template <class T> char Term<T>::sym = 'x';
template <class T> bool Term<T>::highest_term = true;

template <class T>
std::ostream &operator<<(std::ostream &os, const Term<T> &rhs)
{
	rhs.sign(os);
	rhs.coefficient(os);
	rhs.exponent(os);
	return os;
}

// pre-declaration
template <class T> class Poly;
template <class T>
	std::ostream &operator<<(std::ostream &, const Poly<T> &);

template <class T>
class Poly {

protected:
	// p(x)				data
	// 0				[]
	// x				[(1, 1)]
	// a x^3 + b x + c	[(c, 0), (b, 1), (a, 3)]
	// a x^9102 + b x	[(b, 1), (a, 9102)]
	std::list<Term<T>> data;

friend std::ostream &operator<< <T>(std::ostream &, const Poly<T> &);
public:
	// ---- constructors ----

	// default init: p(x) = x
	Poly(): data({Term<T>(1, 1)}) {}

	// convert type T to zero-degree polynomial
	Poly(const T &x): data({Term<T>(x, 0)})
	{
		strip();
	}

	// init with terms
	explicit Poly(const std::initializer_list<Term<T>> &L):
		data(L.begin(), L.end()) { strip(); }

	// init with coefs only
	explicit Poly(const std::initializer_list<T> &L): data()
	{
		int exp = 0;
		// reversed iteration
		auto it = L.end();
		while (it != L.begin())
			data.push_back(Term<T>(*--it, exp++));
		strip();
	}

	// init with a function of exp
	Poly(T lambda(int), int deg): data()
	{
		for (int exp = 0; exp <= deg; ++exp)
			data.push_back(Term<T>(lambda(exp), exp));
		strip();
	}

	// ---- methods ----

	// return the degree of *this.
	// degree of zero polynomial is -1.
	int degree() const
	{
		if (data.empty())
			return -1;
		return data.back().exp;
	}

	// return derivate of *this
	Poly derivate() const
	{
		Poly ret{0};
		if (data.empty())
			return ret;
		auto it = data.cbegin();
		if (it->exp != 0)
			ret.data.push_back(it->derivate());
		while (++it != data.cend())
			ret.data.push_back(it->derivate());
		return ret;
	}

	// ---- operators ----
	
	// assign a value to polynomial, using Horner's rule
	T operator()(const T &x) const
	{
		if (data.empty())
			return 0;
		auto rit = data.crbegin();
		T ret = rit->coef;
		while (++rit != data.crend())
			ret = ret * x + rit->coef;
		return ret;
	}

	// assign a polynomial to polynomial
	Poly operator()(const Poly &rhs) const
	{
		if (data.empty())
			return 0;
		auto rit = data.crbegin();
		Poly ret = rit->coef;
		while (++rit != data.crend())
			ret = ret * rhs + rit->coef;
		return ret;
	}

	bool operator==(const Poly &rhs) const
	{
		return data == rhs.data;
	}

	bool operator!=(const Poly &rhs) const
	{
		return !(*this == rhs);
	}

	Poly operator+() const
	{
		return *this;
	}

	Poly operator-() const
	{
		Poly ret(*this);
		for (auto it = ret.data.begin(); it != ret.data.end(); ++it)
			it->coef = -it->coef;
		return ret;
	}

	// note that the precedence of operator ^ is lower than +
	//! lack efficiency
	Poly operator^(int n) const
	{
		if (n < 0)
			throw std::invalid_argument("exponent must be non-negative!");
		Poly ret{1};
		while (n--)
			ret *= *this;
		return ret;
	}

	// merge
	friend Poly operator+(const Poly &lhs, const Poly &rhs)
	{
		Poly ret{0};
		auto lit = lhs.data.begin(), rit = rhs.data.begin();
		while (lit != lhs.data.end() && rit != rhs.data.end()) {
			if (lit->exp < rit->exp)
				ret.data.push_back(*lit++);
			else if (lit->exp > rit->exp)
				ret.data.push_back(*rit++);
			else { // lit->exp == rit->exp
				Term<T> tmp(lit->coef + rit->coef, lit->exp);
				if (tmp.coef != 0)
					ret.data.push_back(tmp);
				++lit, ++rit;
			}
		}
		while (lit != lhs.data.end())
			ret.data.push_back(*lit++);
		while (rit != rhs.data.end()) {
			ret.data.push_back(*rit++);
		}
		return ret;
	}
	
	friend Poly operator-(const Poly &lhs, const Poly &rhs)
	{
		return lhs + (-rhs);
	}

	friend Poly operator*(const Poly &lhs, const Poly &rhs)
	{
		Poly ret{0};
		if (lhs.data.empty() || rhs.data.empty())
			return ret;
		// call a pair (u,v) concave, if (u-1,v) and (u,v-1) are visited,
		// and (u,v), (u+1,v), (u,v+1) are not.

		// Using lambda to compare elements.
		typedef decltype(lhs.data.begin()) It;
		typedef std::pair<It, It> Pair;
		auto cmp = [](const Pair &x, const Pair &y) {
			int xexp = x.first->exp + x.second->exp;
			int yexp = y.first->exp + y.second->exp;
			return xexp > yexp
				|| (xexp == yexp && x.first->exp > y.first->exp);
		};
		std::priority_queue<Pair, std::vector<Pair>, decltype(cmp)>
			concave(cmp);
		concave.push(Pair(lhs.data.begin(), rhs.data.begin()));
		while (!concave.empty())
		{
			auto lit = concave.top().first, rit = concave.top().second;
			// pop duplicates
			do
				concave.pop();
			while (!concave.empty()
					&& concave.top().first == lit
					&& concave.top().second == rit);
			T coef = lit->coef * rit->coef;
			int exp = lit->exp + rit->exp;
			//std::cout << "exp: " << exp << "=" << lit->exp << '+' << rit->exp << "\n";
			//std::cout << "coef: " << coef << "\n";
			if (!ret.data.empty() && ret.data.back().exp == exp) {
				ret.data.back().coef += coef;
				if (ret.data.back().coef == 0)
					ret.data.pop_back();
			} else if (coef != 0) {
				ret.data.push_back(Term<T>(coef, exp));
			}
			//std::cout << "ret: " << ret << "\n\n";

			// update the concave pairs
			if (++rit != rhs.data.end())
				concave.push(Pair(lit, rit));
			--rit;
			if (++lit != lhs.data.end())
				concave.push(Pair(lit, rit));
		}
		return ret;
	}

	void divmod(Poly &quotient, Poly &residue,
			const Poly &lhs, const Poly &rhs) 
	{
		// not implemented
		/*
        if rhs.iszero():
            raise ZeroDivisionError
        elif lhs.deg() < rhs.deg():
            return Poly(0, sym=lhs.sym), lhs
        elif rhs.deg() == 0:
            return 1 / rhs[0] * lhs, 0
        	considering M / N: first have their coef reversed, so N0 is
			the highest degree coef for N
						N0                  N1      N2
			q0 = c0/N0  c0 = M0             q0N1    q0N2
			q1 = c1/N0  c1 = M1-q0N1        q1N1    q1N2
			q2 = c2/N0  c2 = M2-q0N2-q1N1   q2N1    q2N2
        m, n = lhs.deg(), rhs.deg()
        q = []
        for i in range(m-n+1):
            q.append( lhs[m-i] - sum(q[j] * rhs[n-i+j] for j in\
                range(max(0, i-n), i)) )
            q[i] /= rhs[n]
        r = (lhs[i] - sum(q[m-n-i+j] * rhs[j] for j in range(max(0, n+i-m),\
                i+1)) for i in range(n))
        return Poly(reversed(q), sym=lhs.sym), Poly(r, sym=lhs.sym)
		*/
	}

	friend Poly operator/(const Poly &lhs, const Poly &rhs)
	{
		// not implemented
		return lhs;
	}
	
	friend Poly operator%(const Poly &lhs, const Poly &rhs)
	{
		// not implemented
		return lhs;
	}

	Poly &operator+=(const Poly &rhs)
	{
		*this = *this + rhs;
		return *this;
	}
	
	Poly &operator-=(const Poly &rhs)
	{
		*this = *this - rhs;
		return *this;
	}
	
	Poly &operator*=(const Poly &rhs)
	{
		*this = *this * rhs;
		return *this;
	}
	
	Poly &operator/=(const Poly &rhs)
	{
		// not implemented
		*this = *this / rhs;
		return *this;
	}
	
	Poly &operator%=(const Poly &rhs)
	{
		// not implemented
		*this = *this % rhs;
		return *this;
	}
	
protected:
	// remove terms with zero coef
	void strip()
	{
		const double eps = 1e-8;
		for (auto it = data.begin(); it != data.end(); ++it)
			if (-eps < it->coef && it->coef < eps)
				it = data.erase(it);
	}
};

template <class T>
std::ostream &operator<<(std::ostream &os, const Poly<T> &rhs)
{
	if (rhs.data.empty())
		return os << '0';
	Term<T>::highest_term = true;
	auto rit = rhs.data.crbegin();
	while (rit != rhs.data.crend())
		os << *rit++;
	return os;
}

#endif // POLYNOMIAL_H
