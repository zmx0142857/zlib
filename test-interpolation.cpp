#include <iostream>
#include "interpolation.h"
#include "test.h"
using namespace std;

Test tout;

int main()
{
	/*
	vector<double> x0{1, 2, 3, 4, 5};
	vector<double> y0{0, 2, 12, 42, 116};
	Lagrange L0(x0, y0);
	tout << L0;
	tout.test("0.5x^4 - 3x^3 + 9.5x^2 - 13x + 6");
	tout << L0(1.5);
	tout.test("0.28125");
	Newton N0(x0, y0);
	tout << N0;
	tout.test("0.5x^4 - 3x^3 + 9.5x^2 - 13x + 6");
	tout << N0(1.5);
	tout.test("0.28125");
	*/

	vector<double> x1{-1, 0, 0.5, 1};
	vector<double> y1{-1.5, 0, 0, 0.5};
	Lagrange L1(x1, y1);
	tout << L1;
	tout.test("x^3 - 0.5x^2");
	/*
	tout << L1(-1) << ' ' << L1(0) << ' ' << L1(0.5) << ' ' << L1(1);
	tout.test("-1.5 0 0 0.5");
	Newton N1(x1, y1);
	tout << N1;
	tout.test("x^3 - 0.5x^2");
	tout << N1(-1) << ' ' << N1(0) << ' ' << N1(0.5) << ' ' << N1(1);
	tout.test("-1.5 0 0 0.5");
	*/
	return 0;
}
