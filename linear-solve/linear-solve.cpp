#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

class Mat
{
	protected:
		typedef std::vector<double> Vec_t;
		typedef std::vector<Vec_t> Mat_t;

	public:
		// io
		friend std::ostream &operator<<(std::ostream &, const Mat &);

		// constructors
		Mat();					// empty matrix
		Mat(size_t, size_t);	// init with 0s
		Mat(std::istream &);

		// operators
		const Vec_t &operator[](size_t) const;
		Vec_t &operator[](size_t);

		// methods
		size_t rows() const;
		size_t cols() const;
		Mat solve() const;
		void primary(std::istream &);

	protected:
		Mat_t data;
		std::vector<std::string> title;

		static std::ostream &print_vec(std::ostream &, const Vec_t &, size_t width);
		void pr1(size_t, double);
		void pr2(size_t, size_t, double);
		void pr3(size_t, size_t);
		void pc1(size_t, double);
		void pc2(size_t, size_t, double);
		void pc3(size_t, size_t);
		size_t pivot(size_t);
		bool p1(size_t, size_t);
		bool p2(size_t, size_t);
		bool p3(size_t, size_t);
		bool swap_col(size_t, size_t, std::vector<size_t> &);
		void simplify(std::vector<size_t> &, bool=false);
		void to_ref();
		void to_urref();
		size_t rank(bool simplified=false, bool with_last_col=true);
};

//-------------- io -----------------

std::ostream &Mat::print_vec(std::ostream &os, const Vec_t &rhs, size_t width)
{
	// output first line
	if (!rhs.empty())
		os << std::setw(width) << rhs.front();
	// output the rest
	for (Mat::Vec_t::const_iterator
			it = ++rhs.begin(); it != rhs.end(); ++it)
		os << ' ' << std::setw(width) << *it;
	return os;
}

std::ostream &operator<<(std::ostream &os, const Mat &rhs)
{
	// find proper width for elements
	size_t width = 0, len = 0;
	std::ostringstream oss;
	for (Mat::Mat_t::const_iterator
			it = rhs.data.begin(); it != rhs.data.end(); ++it)
	{
		for (Mat::Vec_t::const_iterator jt = it->begin();
				jt != it->end(); ++jt)
		{
			oss << *jt;
			size_t tmp = oss.str().size();
			if (tmp - len > width)
				width = tmp - len;
			len = tmp;
		}
	}
	
	// output title
	if (!rhs.title.empty())
	{
		os << std::setw(width) << rhs.title.front();
		for (std::vector<std::string>::const_iterator
				it = ++rhs.title.begin(); it != rhs.title.end(); ++it)
			os << ' ' << std::setw(width) << *it;
		os << '\n';
	}

	// output first line
	if (!rhs.data.empty())
		Mat::print_vec(os, rhs.data.front(), width);

	// output the rest
	for (Mat::Mat_t::const_iterator it = ++rhs.data.begin();
			it != rhs.data.end(); ++it)
	{
		os << '\n';
		Mat::print_vec(os, *it, width);
	}
	return os;
}

//---------- constructors ------------

Mat::Mat() {}

Mat::Mat(size_t r, size_t c):
	data(Mat_t(r, Vec_t(c, 0))), title() {}

Mat::Mat(std::istream &is)
{
	std::string line;
	std::vector<double> row;
	double elem;
	size_t row_cnt = 0;
	while (getline(is, line))
	{
		std::istringstream iss(line);
		while (iss >> elem)
			row.push_back(elem);
		if (row.size() > row_cnt)
			row_cnt = row.size();
		data.push_back(row);
		row.clear();
	}

	// fill blanks with 0s
	for (Mat_t::iterator it = data.begin(); it != data.end(); ++it)
		if (it->size() < row_cnt)
			it->resize(row_cnt);
	is.clear();
}

//---------- operators ----------

const std::vector<double> &Mat::operator[](size_t i) const
{
	return data[i];
}

std::vector<double> &Mat::operator[](size_t i)
{
	return data[i];
}

//---------- methods --------------

inline size_t Mat::rows() const
{
	return data.size();
}

inline size_t Mat::cols() const
{
	if (data.empty())
		return 0;
	return data.front().size();
}

/* solve the system of equations, the result
 *    k1 T1 + k2 T2 + ... + kn Tn + X0
 * is returned as Mat, where T1, ..., Tn, X0 be the columns.
 * an empty Mat m (aka, m.data.empty() == true) is returned
 * if the system has no solution.
 */
Mat Mat::solve() const
{
	// make a copy so that *this will stay unchanged.
	Mat m(*this);
	std::vector<size_t> vars;
	for (size_t i = 0; i != m.cols()-1; ++i)
		vars.push_back(i);
	m.simplify(vars, true);
	//std::cout << "simplified:\n" << m << std::endl;

	// if no solution, returns an empty matrix
	if (m.rank(true, false) != m.rank(true, true))
	{
		Mat ret;
		ret.title.push_back("no solution!");
		return ret;
	}

	// compute rank of m
	size_t r = 0;
	while (r != m.rows() && r != m.cols())
	{
		if (m[r][r] == 0)
			break;
		++r;
	}

	Mat ret(m.cols()-1, m.cols()-r);

	// set title
	for (size_t i = 1; i != ret.cols(); ++i)
	{
		std::ostringstream oss;
		oss << 'T' << i;
		ret.title.push_back(oss.str());
	}
	ret.title.push_back("X0");

	// set elements
	for (size_t i = 0; i != ret.rows(); ++i)
	{
		for (size_t j = 0; j != ret.cols(); ++j)
		{
			if (vars[i] < r && r + j < m.cols())
			{
				double elem = m[vars[i]][r+j];
				ret[i][j] = (r + j == m.cols()-1 ? elem : -elem);
			}
			else
				ret[i][j] = (vars[i] == r+j && vars[i] != m.cols()-1);
		}
	}

	return ret;
}

/* primary row operation of the 1st class: data[row] *= k */
void Mat::pr1(size_t row, double k)
{
	for (Vec_t::iterator
			it = data[row].begin(); it != data[row].end(); ++it)
		*it *= k;
}

/* primary row operation of the 2nd class: row += row2 * k */
void Mat::pr2(size_t row, size_t row2, double k)
{
	for (size_t col = 0; col != cols(); ++col)
		data[row][col] += data[row2][col] * k;
}

/* primary row operation of the 3rd class: swap row, row2 */
void Mat::pr3(size_t row, size_t row2)
{
	if (row != row2)
		data[row].swap(data[row2]);
}

/* primary column operation of the 1st class: data[col] *= k */
void Mat::pc1(size_t col, double k)
{
	for (size_t row = 0; row != rows(); ++row)
		data[row][col] *= k;
}

/* primary column operation of the 2nd class: col += col2 * k */
void Mat::pc2(size_t col, size_t col2, double k)
{
	for (size_t row = 0; row != rows(); ++row)
		data[row][col] += data[row][col2] * k;
}

/* primary column operation of the 3rd class: swap col, col2 */
void Mat::pc3(size_t col, size_t col2)
{
	if (col != col2)
	{
		for (size_t row = 0; row != rows(); ++row)
		{
			double tmp = data[row][col];
			data[row][col] = data[row][col2];
			data[row][col2] = tmp;
		}
	}
}

/* an interactive primary operation function */
void Mat::primary(std::istream &is)
{
	std::string line;
	while (getline(is, line))
	{
		char operation;
		unsigned operation_class;
		size_t idx, idx2;
		double k;
		std::istringstream iss(line);
		
		iss >> operation >> operation_class;
		if (operation == 'r')
			switch (operation_class)
			{
				case 1: iss >> idx >> k;			pr1(idx, k); break;
				case 2: iss >> idx >> idx2 >> k;	pr2(idx, idx2, k); break;
				case 3: iss >> idx >> idx2;			pr3(idx, idx2); break;
			}
		else if (operation == 'c')
			switch (operation_class)
			{
				case 1: iss >> idx >> k;			pc1(idx, k); break;
				case 2: iss >> idx >> idx2 >> k;	pc2(idx, idx2, k); break;
				case 3: iss >> idx >> idx2;			pc3(idx, idx2); break;
			}
		std::cout << *this << "\n\n";
	}
}

/* help to find proper pivot in col for number 1 */
size_t Mat::pivot(size_t col)
{
	size_t ret = col;
	while (ret)
	{
		for (size_t c = 0; c != col; ++c)
			if (data[ret-1][c])
				return ret;
		--ret;
	}
	return ret;
}

/* Search for 1 or -1 and swap it to desired position.
 * Returns true if success.
 */
bool Mat::p3(size_t col, size_t pivot)
{
	for (size_t row = pivot; row != rows(); ++row)
	{
		if (data[row][col] == 1)
		{
			pr3(row, pivot);
			return true;
		}
		else if (data[row][col] == -1)
		{
			pr1(row, -1);
			pr3(row, pivot);
			return true;
		}
	}
	return false;
}

/* Generate number 1 by adding/subtracting.
 * Returns true if success.
 */
bool Mat::p2(size_t col, size_t pivot)
{
	for (size_t row = pivot; row != rows(); ++row)
	{
		for (size_t find = pivot; find != rows(); ++find)
		{
			if (find == row)
				continue;
			double tmp = data[row][col] + data[find][col];
			if (tmp == 1 || tmp == -1)
			{
				pr2(row, find, 1);
				return true;
			}
			tmp = data[row][col] - data[find][col];
			if (tmp == 1 || tmp == -1)
			{
				pr2(row, find, -1);
				return true;
			}
		}
	}
	return false;
}

/* Generate number 1 by division.
 * Returns true if success.
 */
bool Mat::p1(size_t col, size_t pivot)
{
	for (size_t row = pivot; row != rows(); ++row)
	{
		double tmp = data[row][col];
		if (tmp)
		{
			pr1(row, 1 / tmp);
			return true;
		}
	}
	return false;
}

/* this function is needed when current column has only 0 for row >= target
 * Returns true if success.
 */
bool Mat::swap_col(size_t col, size_t pivot, std::vector<size_t> &vars)
{
	for (size_t c = cols()-2; c != col; --c)
	{
		for (size_t r = pivot; r != rows(); ++r)
		{
			if (data[r][c])
			{
				pc3(col, c);
				if (!vars.empty())
				{
					size_t tmp = vars[col];
					vars[col] = vars[c];
					vars[c] = tmp;
				}
				return true;
			}
		}
	}
	return false;
}

void Mat::simplify(std::vector<size_t> &vars, bool unique)
{
	size_t col = 0;
	while (col != rows() && col != cols())
	{
		size_t pi = pivot(col);

		if (p3(col, pi))
			;
		else if (p2(col, pi))
			p3(col, pi);
		else if (p1(col, pi))
			p3(col, pi);

		// now there are only 0s below pivot in this column
		else if (cols() >= 2)
		{
			if (!swap_col(col, pi, vars))
				++col;
			continue;
		}

		// for all rows below: goto 0!
		for (size_t row = pi+1; row != rows(); ++row)
			pr2(row, pi, -data[row][col]);
		++col;
	}

	// reversed iteration
	if (unique)
		for (size_t col = (cols() < rows() ? cols() : rows()) - 1;
				col != (size_t)-1; --col)
		{
			if (!data[col][col])
				continue;
			// for all rows above: goto 0!
			for (size_t row = 0; row != col; ++row)
				pr2(row, col, -data[row][col]);
		}
}

/* row echelon form */
void Mat::to_ref()
{
	std::vector<size_t> v;
	simplify(v);
}

/* unique reduced row echelon form */
void Mat::to_urref()
{
	std::vector<size_t> v;
	simplify(v, true);
}

size_t Mat::rank(bool simplified, bool with_last_col)
{
	Mat m(*this);
	if (!simplified)
		m.to_ref();

	size_t col = m.cols();
	if (!with_last_col)
		--col;

	size_t r = 0;
	for (; r != m.rows(); ++r)
	{
		bool all_zero = true;
		for (size_t c = r; c != col; ++c)
		{
			if (m[r][c])
			{
				all_zero = false;
				break;
			}
		}
		if (all_zero)
			return r;
	}
	return r;
}

using namespace std;
int main()
{
	Mat m(cin);
	cout << m << '\n';
	cout << "solution:\n";
	cout << m.solve() << '\n';
	return 0;
}

/* --------- test 1 ----------
 * [input]
 * 1 -1 1 -1 1
 * 1 -1 -1 1
 * 1 -1 -2 2 -0.5
 * 
 * [as expected]
 *   T1   T2   X0
 *   -0    1  0.5
 *    0    1    0
 *    1    0    0
 *    1    0 -0.5
 * --------- test 2 ------------
 * [input]
 * 1 2 0 -3 2 1
 * 1 -1 -3 1 -3 2
 * 2 -3 4 -5 2 7
 * 9 -9 6 -16 2 25
 * 
 * [expecting]
 * no solution!
 * 
 * [got]
 *          T1          X0
 *    -4.12121 5.20302e+14
 *    -1.93939 1.62061e+14
 *    -2.39394 2.13239e+14
 *          -2 2.81475e+14
 *           1           0
 */
