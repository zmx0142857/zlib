#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#if __cplusplus < 201101
#error "require c++11"
#endif

#include <vector>
#include <stdexcept>	// std::out_of_range, std::invalid_argument
#include <algorithm>	// sort
#include "polynomial.h"

const char *LIST_SIZE_NEQ = "The size of two lists are not equal!";

/*
	Build Lagrange interpolation polynomial and compute its value.
    The formula is:

    L(x) = sum_(i=1)^n y_i * prod_(1<=j!=i<=n) (x - x_j) / (x_i - x_j)
*/
class Lagrange {
protected:
		std::vector<double> x;
		std::vector<double> coef;
public:
	Lagrange(const std::vector<double> &x_list,
			const std::vector<double> &y_list):
			x(x_list), coef(y_list)
	{
		if (x_list.size() != y_list.size())
			throw std::invalid_argument(LIST_SIZE_NEQ);
		size_t n = coef.size();
		for (size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < n; ++j)
				if (j != i)
					coef[i] /= (x[i] - x[j]);
	}

	template <class S>
	S operator()(S s) const
	{
		S ret = 0;
		size_t n = coef.size();
		for (size_t i = 0; i < n; ++i) {
			S item = 1;
			for (size_t j = 0; j < n; ++j) {
				if (j != i)
					item *= (s - x[j]);
			}
			ret += item * coef[i];
		}
		return ret;
	}

	double operator()(int s) const
	{
		return this->operator()<double>(s);
	}

	double operator()(double s) const
	{
		return this->operator()<double>(s);
	}

	Poly<double> operator()(Poly<double> s) const
	{
		return this->operator()<Poly<double>>(s);
	}

	friend std::ostream &operator<<(std::ostream &os, const Lagrange &rhs)
	{
		return os << rhs(Poly<double>());
	}
};

/*
	Build Newton interpolation polynomial and compute its value.
    The formula is:

    N_0(x) = y0
    N_n(x) = N_(n-1)(x) + prod_(i=0)^(n-1) (x-x_j) * f[x0,x1,x2..xn]

    and

    N(x) = f[x0] + (x-x0)f[x0,x1]
                 + (x-x0)(x-x1)f[x0,x1,x2]
                 + (x-x0)(x-x1)(x-x2)f[x0,x1,x2,x3]
                 + ...
                 + (x-x0)(x-x1)...(x-x(n-1))f[x0..xn]
*/
class Newton {
protected:
	std::vector<std::vector<double> > diff;

	static bool compare(const std::vector<double> &lhs,
			const std::vector<double> &rhs)
	{
		return lhs[0] < rhs[0];
	}

	void compute_diff()
	{
		
		/*
        diff table
        x0 y0
        x1 y1 [x0,x1]
        x2 y2 [x1,x2] [x0,x1,x2]
        x3 y3 [x2,x3] [x1,x2,x3] [x0,x1,x2,x3]

		example:
		1 0
		2 2		2
		3 12	10
		4 42	30
		5 116	64
		*/
		for (size_t i = 1; i < diff.size(); ++i) {
			while (diff[i].size() < i + 2) {
				//dump();
				size_t j = diff[i].size() - 1;
				diff[i].push_back(
					(diff[i][j]-diff[i-1][j]) / (diff[i][0]-diff[i-j][0])
				);
			}
		}
	}

	// for debug
	void dump()
	{
		for (size_t i = 0; i < diff.size(); ++i) {
			for (size_t j = 0; j < diff[i].size(); ++j) {
				std::cout << diff[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}

public:
	Newton(const std::vector<double> &x_list,
			const std::vector<double> &y_list)
	{
		insert(x_list, y_list);
	}

	// insert new data
	void insert(const std::vector<double> &x_list,
			const std::vector<double> &y_list)
	{
		if (x_list.size() != y_list.size())
			throw std::invalid_argument(LIST_SIZE_NEQ);
		for (size_t i = 0; i < x_list.size(); ++i)
			diff.push_back(std::vector<double>{x_list[i], y_list[i]});
		sort(diff.begin(), diff.end(), compare);
		compute_diff();
	}

	// erase n of the existing data from the end
	void erase(size_t n=1)
	{
		if (diff.size() < n)
			throw std::out_of_range("has no enough data to erase!");
		for (size_t i = 0; i < n; ++i)
			diff.pop_back();
	}

	// return value by Horner's rule
	template <class S>
	S operator()(S s) const
	{
		const std::vector<double> &last_line = diff[diff.size()-1];
		S ret = last_line[last_line.size()-1];
		for (size_t i = diff.size()-2; i != size_t(-1); --i)
			ret = ret * (s - diff[i][0]) + diff[i][i+1];
		return ret;
	}

	double operator()(int s) const
	{
		return this->operator()<double>(s);
	}

	double operator()(double s) const
	{
		return this->operator()<double>(s);
	}

	Poly<double> operator()(Poly<double> s) const
	{
		return this->operator()<Poly<double>>(s);
	}

	friend std::ostream &operator<<(std::ostream &os, const Newton &rhs)
	{
		return os << rhs(Poly<double>());
	}
};

#endif // INTERPOLATION_H
