// g++ -c int.cpp -Wall
// g++ int.o int-test.cpp -o int-test/int-test -Wall
#include "int.h"
#include "test.h"
using namespace std;

Test tout;
void test_io()
{
	Int a;
	while (cin >> a)
		cout << a << endl;
}

void test_arithmetic()
{
	Int a, b, r; char op;
	while (cin >> a >> op >> b)
	{
		switch (op)
		{
			case '<': cout << (a < b) << endl; break;
			case '+': cout << a + b << endl; break;
			case '-': cout << a - b << endl; break;
			case '*': cout << a * b << endl; break;
			case '/': cout << divmod(a, b, r) << ", ";
					  cout << r << endl; break;
			case '^': cout << a.pow(b) << endl; break;
		}
	}
}

void test_factorial()
{
	int n = 99;
	Int prod = 1;
	while (n != 0)
		prod *= (n--);
	tout << prod;
	tout.test("933262154439441526816992388562667004907159682643816214685929638952175999932299156089414639761565182862536979208272237582511852109168640000000000000000000000");
}

void test_bit()
{
	Int a, b; char op;
	while (cin >> a >> op >> b)
	{
		switch (op)
		{
			case '<': cout << (a << b.to_int()) << endl; break;
			case '>': cout << (a >> b.to_int()) << endl; break;
			case '^': cout << (a ^ b) << endl; break;
			case '&': cout << (a & b) << endl; break;
			case '|': cout << (a | b) << endl; break;
			case '~': cout << (~a) << endl; break;
		}
	}
}

void test_to_int()
{
	Int a;
	cout << "LLONG_MAX =\n" << LLONG_MAX << '\n';
	while (cin >> a)
		cout << a.to_int() << endl;
}

void test_add()
{
	Int a, b, dest;
	while (cin >> a >> b)
	{
		Int::add(a.data.begin(), a.data.end(), b.data.begin(), b.data.end(), dest);
		cout << dest << endl;
	}
}

void help()
{
	cout << "usage: int-test [option]\n"
			"-i   test_io\n"
			"-a   test_arithmetic\n"
			"-f   test_factorial\n"
			"-b   test_bit\n"
			"-t   test_to_int\n"
			"-A   test_add\n";
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		help();

	switch (argv[1][1]) {
		case 'i':	test_io();			break;
		case 'a':	test_arithmetic();	break;
		case 'f':	test_factorial();	break;
		case 'b':	test_bit();			break;
		case 't':	test_to_int();		break;
		case 'A':	test_add();			break;
		default:	help();
	}
	return 0;
}
