#ifndef Calendar_h
#define Calendar_h

#include <stdio.h>
#include <string.h>	// strtok()
#include <stdlib.h>	// atoi()
#include <ctype.h>	// tolower()

// typedefs
typedef int bool;
const bool true = 1;
const bool false = 0;

typedef int Date;				// 0 = Dec 31 -0001, 1 = Jan 1 0001
typedef int Year;
typedef int Month;
typedef int Day;				// day of month

// consts
extern const Date YEAR1;		// Mon Jan 1 0001  
extern const Date YEAR1970;		// Thu Jan 1 1970
extern const Date YEAR2001;		// Mon Jan 1 2001  
extern const int YEAR;			// days in a year
extern const int QUADYEAR;		// days in 4 years
extern const int CENTURY;		// days in 100 years
extern const int QUADCENTURY;	// days in 400 years

extern char *month_name[];
extern char *Month_name[];
extern char *weekday_name[];

// helper functions
extern bool is_leap(const Year);
extern Month str2month(const char*);
extern void month2str(char*, const Month);
extern int month_length(const Month, const Year);

// constructors of date
extern Date int2date(const Year, const Month, const Day);
// format: monthname day year
extern Date str2date(const char*);

// input
extern void date_scanf(Date *);

// getters
extern void parse_year(const Date, Year*, Day*);
extern Year get_year(const Date);
extern Day get_doy(const Date);	// day of year

extern void parse_month(const Date, Year*, Month*, Day*);
extern Month get_month(const Date);
extern Day get_day(const Date);

extern void get_weekday(char*, const Date);

// output
extern void date2str(char*, const Date);
extern void date_printf(const Date);
extern void cal2str(char*, const Date);
extern void cal_printf(const Date);

// operators
// you can add or subtract Date with int freely

// use system time to tell what day is today
extern Date buildday();

// test
#ifdef DEBUG
void test_int2date()
{
	Year y; Month m; Day d;
	while (scanf("%d %d %d", &y, &m, &d) == 3) {
		Date date = int2date(y, m, d);
		date_printf(date);
		puts("");
	}
}

void test_str2date()
{
	char buf[256];
	while (scanf("%s", buf) == 1) {
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
	Date date = buildday();
	date_printf(date);
	puts("");
}
#endif // DEBUG

#endif // Calendar_h
