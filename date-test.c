#include "date.h"

void test_int2date()
{
	Year y; Month m; Day d;
	while (scanf("%d %d %d", &y, &m, &d) == 3) {
		Date date = int2date(y, m, d);
		date_printf(date);
		puts("");
	}
}

size_t mygetline(char *buf)
{
	size_t cnt = 0;
	char ch;
	do {
		ch = getchar();
		*buf++ = ch;
		++cnt;
	} while (ch != EOF && ch != '\n');
	*--buf = '\0';
	return --cnt;
}

void test_str2date()
{
	char buf[256];
	while (mygetline(buf)) {
		Date date = str2date(buf);
		date_printf(date);
		puts("");
	}
}

void test_cal_printf()
{
	Year y; Month m; Day d;
	while (scanf("%d %d %d", &y, &m, &d) == 3) {
		Date date = int2date(y, m, d);
		cal_printf(date);
	}
}

void test_buildday()
{
	date_printf(buildday());
	puts("");
}

void test_today()
{
	date_printf(today());
	puts("");
}

void test_birthday()
{
	printf("%d\n", today() - BIRTHDAY);
}

void help()
{
	printf(	"usage: date-test [option]\n"
			"-B   test_birthday\n"
			"-t   test_today\n"
			"-b   test_buildday\n"
			"-c   test_cal_printf\n"
			"-s   test_str2date\n"
			"-i   test_int2date\n"
	);
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		help();

	switch (argv[1][1]) {
		case 'B':	test_birthday();	break;
		case 't':	test_today();		break;
		case 'b':	test_buildday();	break;
		case 'c':	test_cal_printf();	break;
		case 's':	test_str2date();	break;
		case 'i':	test_int2date();	break;
		default:	help();
	}
	return 0;
}
