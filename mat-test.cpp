#include "mat.h"
#include "test.h"
using namespace std;

Test tout;
istringstream tin;

void test_solve(const string &in, const string &out)
{
	tin.str(in);
	Mat m(tin);
	tout << m.solve() << '\n';
	tout.test(out);
}

int main()
{
	test_solve(
		"1 -1 1 -1 1\n"
		"1 -1 -1 1\n"
		"1 -1 -2 2 -0.5\n"
		,
		"  T1   T2   X0\n"
		"  -0    1  0.5\n"
		"   0    1    0\n"
		"   1    0    0\n"
		"   1    0 -0.5\n"
	);

	/* [got]
	 *         T1          X0
	 *   -4.12121 5.20302e+14
	 *   -1.93939 1.62061e+14
	 *   -2.39394 2.13239e+14
	 *         -2 2.81475e+14
	 *          1           0
	 */
	test_solve(
		"1 2 0 -3 2 1\n"
		"1 -1 -3 1 -3 2\n"
		"2 -3 4 -5 2 7\n"
		"9 -9 6 -16 2 25\n"
		,
		"no solution!\n"
	);
	return 0;
}

