#include "polynomial.h"
#include "test.h"
using namespace std;

Test tout;

void test_io()
{
	tout << Poly<double>{0};
	tout.test("0");

	tout << Poly<double>();
	tout.test("x");

	tout << Poly<double>{
		Term<double>(3, 0),
		Term<double>(2, 1),
		Term<double>(1, 2)
	};
	tout.test("x^2 + 2x + 3");

	tout << Poly<double>{1, 2, 3};
	tout.test("x^2 + 2x + 3");

	tout << Poly<double>{1, -1, -1};
	tout.test("x^2 - x - 1");

	tout << Poly<double>{1, -4, -10};
	tout.test("x^2 - 4x - 10");

	tout << Poly<double>{-1, -4, -10};
	tout.test("-x^2 - 4x - 10");

	tout << Poly<double>{-1, -4, 0};
	tout.test("-x^2 - 4x");

	tout << Poly<double>{-1, 0, -90};
	tout.test("-x^2 - 90");

	tout << Poly<double>(
		[](int i){ return double(-i*i); }, 3
	);
	tout.test("-9x^3 - 4x^2 - x");
}

void test_arithmetic1()
{
	Poly<double> p, q;
	p = Poly<double>{1, -2, 3};
	tout << -p;
	tout.test("-x^2 + 2x - 3");

	p = Poly<double>{1, -2, 3};
	q = Poly<double>{9, 0, -1, 3};
	tout << p + q;
	tout.test("9x^3 + x^2 - 3x + 6");
	tout << p - q;
	tout.test("-9x^3 + x^2 - x");

	p = Poly<double>{1, 1, 1, 1, 1};
	q = Poly<double>{1, -1};
	tout << p*q;
	tout.test("x^5 - 1");

	tout << q*p;
	tout.test("x^5 - 1");

	tout << (q^2);
	tout.test("x^2 - 2x + 1");
}

void test_other_method()
{
    Poly<double> p{1, -3, 2};
    tout << p(5);
    tout.test("12");
    tout << p(2);
    tout.test("0");

	tout << Poly<double>{1, 0, 0}(Poly<double>{1, -1});
	tout.test("x^2 - 2x + 1");

    tout << Poly<double>{-1, 0, -2, 1}.derivate();
    tout.test("-3x^2 - 2");
}

void test_arithmetic2()
{
	Poly<double> x;
	tout << x;
	tout.test("x");
	tout << x+0;
	tout.test("x");
	tout << x+1;
	tout.test("x + 1");
	tout << x-2;
	tout.test("x - 2");
	tout << (x+1)*(x-2);
	tout.test("x^2 - x - 2");
	tout << (x-0)*(x-0.5)*(x-1)*0.5 + (x-(-1))*(x-0)*(x-0.5)*0.5;
	tout.test("x^3 - 0.5x^2");
	tout << (x^3) - 0.5*(x^2) + 0;
	tout.test("x^3 - 0.5x^2");
}

int main()
{
	//test_io();
	//test_arithmetic1();
	//test_other_method();
	test_arithmetic2();
	return 0;
}
