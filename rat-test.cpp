// g++ -c int.cpp rat.cpp -Wall
// g++ int.o rat.o rat-test.cpp -o rat-test -Wall
#include "rat.h"
#include "test.h"
using namespace std;

void test_io()
{
	Rat a;
	while (cin >> a)
		cout << a << endl;
}

void test_relation()
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

void test_arithmetic()
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

void help()
{
	printf(	"usage: rat-test [option]\n"
			"-i   test_io\n"
			"-r   test_relation\n"
			"-a   test_arithmetic\n"
	);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		help();

	switch (argv[1][1]) {
		case 'i':	test_io();			break;
		case 'r':	test_relation();	break;
		case 'a':	test_arithmetic();	break;
		default:	help();
	}
	return 0;
}
