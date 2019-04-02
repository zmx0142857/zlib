#include "poly.h"
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

void test_io2()
{
	Term<double>::multiply_sign = "*";
	Term<double>::exp_sign = "**";

	tout << Poly<double>{0};
	tout.test("0");

	tout << Poly<double>();
	tout.test("x");

	tout << Poly<double>{
		Term<double>(3, 0),
		Term<double>(2, 1),
		Term<double>(1, 2)
	};
	tout.test("x**2 + 2*x + 3");

	tout << Poly<double>{1, 2, 3};
	tout.test("x**2 + 2*x + 3");

	tout << Poly<double>{1, -1, -1};
	tout.test("x**2 - x - 1");

	tout << Poly<double>{1, -4, -10};
	tout.test("x**2 - 4*x - 10");

	tout << Poly<double>{-1, -4, -10};
	tout.test("-x**2 - 4*x - 10");

	tout << Poly<double>{-1, -4, 0};
	tout.test("-x**2 - 4*x");

	tout << Poly<double>{-1, 0, -90};
	tout.test("-x**2 - 90");

	tout << Poly<double>(
		[](int i){ return double(-i*i); }, 3
	);
	tout.test("-9*x**3 - 4*x**2 - x");
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

void help()
{
	cout << "usage: poly-test [option]\n"
			"-i   test_io\n"
			"-1   test_arithmetic1\n"
			"-2   test_arithmetic2\n"
			"-o   test_other_method\n";
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		help();

	switch (argv[1][1]) {
		case 'i':	test_io();			break;
		case 'I':	test_io2();			break;
		case '1':	test_arithmetic1();	break;
		case '2':	test_arithmetic2();	break;
		case 'o':	test_other_method();break;
		default:	help();
	}
	return 0;
}
