#ifndef Date_h
#define Date_h

#include <stdio.h>
#include <string.h>	// strtok()
#include <stdlib.h>	// atoi()
#include <ctype.h>	// tolower()
#include <time.h>	// time()

// typedefs
typedef int bool;
const bool true = 1;
const bool false = 0;

typedef int Date;				// 0 = Dec 31 -0001, 1 = Jan 1 0001
typedef int Year;
typedef int Month;
typedef int Day;				// day of month

// consts
extern const int YEAR;			// days in a year
extern const int QUADYEAR;		// days in 4 years
extern const int CENTURY;		// days in 100 years
extern const int QUADCENTURY;	// days in 400 years

extern const Date YEAR1;		// Mon Jan 1 0001  
extern const Date YEAR1970;		// Thu Jan 1 1970
extern const Date YEAR2001;		// Mon Jan 1 2001  
extern const Date BIRTHDAY;		// Wed Feb 19 1997

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
extern void date_str(char*, const Date);
extern void date_printf(const Date);
extern void cal_str(char*, const Date);
extern void cal_printf(const Date);

// operators
// you can add or subtract Date with int freely

// use system time to tell what day is today
extern Date buildday();
extern Date today();

// -------- consts --------

const int YEAR = 365;						// 365
const int QUADYEAR = 4 * YEAR + 1;			// 1461
const int CENTURY = 25 * QUADYEAR - 1;		// 36524
const int QUADCENTURY = 4 * CENTURY + 1;	// 146097

const Date YEAR1 = 1;						// 1
const Date YEAR1970 = 719163;				// 719163
const Date YEAR2001 = 5 * QUADCENTURY + 1;	// 730486
const Date BIRTHDAY = 729074;				// 729074

// -------- helper functions --------

bool is_leap(const Year year)
{
	// didn't check year == 0
	int y = year < 0 ? year+1 : year;
	return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

Month str2month(const char *s)
{
	static const char *name[] = {
		"jan", "feb", "mar", "apr", "may", "jun",
		"jul", "aug", "sep", "oct", "nov", "dec"
	};

	// s not NULL and s.length >= 1
	if (s && s[0] != '\0') {
		// s.length >= 3
		if (s[1] != '\0' && s[2] != '\0') {
			// sub = s.substr(0,3).tolower()
			char sub[4] = "...";
			for (int i = 0; i < 3; ++i)
				sub[i] = tolower(s[i]);

			for (Month i = 0; i < 12; ++i) {
				if (strcmp(sub, name[i]) == 0)
					return i+1;
			}
		}
		//printf("invalid monthname %s\n", s);
	}
	return 0;
}

void month2str(char *buf, const Month m)
{
	static const char *name[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	if (1 <= m && m <= 12)
		strcpy(buf, name[m-1]);
	else
		strcpy(buf, "Dec");
}

int month_length(const Month m, const Year y)
{
	static int len[13] = {
		31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};

	if (m == 2)
		return len[m] + is_leap(y);
	else if (1 <= m && m <= 12)
		return len[m];
	else
		return len[0];
}

// -------- constructors --------

/* qc is the quadcentury count since AD
 * -1 = (Jan 1 -400 ~ Dec 31 -001)
 * 0 = (Jan 1 0001 ~ Dec 31 0400)
 * 1 = (Jan 1 0401 ~ Dec 31 0800)
 * 5 = (Jan 1 2001 ~ Dec 31 2400)
 *
 * c is the century count within its quadcentury
 * 0 = (Jan 1 0001 ~ Dec 31 0100) = (Jan 1 2001 ~ Dec 31 2100)
 * 3 = (Jan 1 1901 ~ Dec 31 2000)
 * 0 <= c <= 3
 *
 * qy is the quadyear count within its century
 * 0 <= qy <= 24
 *
 * y is the year count within its quadyear
 * 0 = (Jan 1 1601 ~ Dec 31 1601) = (Jan 1 1997 ~ Dec 31 1997)
 * 3 = (Jan 1 1604 ~ Dec 31 1604) = (Jan 1 2000 ~ Dec 31 2000)
 * 0 <= y <= 3
 */
Date int2date(const Year year, const Month month, const Day day)
{
	if (year == 0) {
		printf("invalid year %d\n", year);
		exit(1);
	}
	int y = year - (year > 0);
	int qc = y / 400;	int qcr = y % 400;
	// the negative division rounds to 0
	// now correct it for rounding downwards
	if (year < 0 && qcr != 0) {
		--qc;
		qcr += 400;
	}
	int c = qcr / 100;	int cr = qcr % 100;
	int qy = cr / 4;	y = cr % 4;	
	Date ret = QUADCENTURY*qc + CENTURY*c + QUADYEAR*qy + YEAR*y;

	if (month < 1 || month > 12) {
		printf("invalid month %d\n", month);
		exit(1);
	}
	for (Month m = 1; m < month; ++m)
		ret += month_length(m, year);

	if (day < 1 || day > month_length(month, year)) {
		printf("invalid day %d\n", day);
		exit(1);
	}
	ret += day;
	//printf("int2date: %d\n", ret);
	return ret;
}

Date str2date(const char *str)
{
	char cpy[256];
	strcpy(cpy, str);
	char *s = cpy;
	const char *delim = " ,.-/";

	s = strtok(s, delim);
	Month m = str2month(s);
	if (!m) m = atoi(s);
	s = strtok(NULL, delim);
	Day d = atoi(s);
	s = strtok(NULL, delim);
	Year y = atoi(s);
	//printf("str2date: %d %d %d\n", y, m, d);
	return int2date(y, m, d);
}

// -------- getters --------

// qc, c, qy and y are the same as int2date
// doy = day of year
void parse_year(const Date date, Year *year, Day *doy)
{
	Date d = date-1;
	int qc = d / QUADCENTURY;	int qcr = d % QUADCENTURY;
	if (d < 0 && qcr != 0) {
		--qc;
		qcr += QUADCENTURY;
	}
	int c = qcr / CENTURY;		int cr = qcr % CENTURY;
	int qy = cr / QUADYEAR;		int qyr = cr % QUADYEAR;
	int y = qyr / YEAR;			*doy = qyr % YEAR + 1;
	// meets the whole 400 years, e.g. Dec 31 2000
	if (c == 4) {
		c = y = 3;
		qy = 24;
		*doy = 366;
	}
	//printf("qc: %d, c: %d, qy: %d, y: %d\n", qc, c, qy, y);
	*year = 400*qc + 100*c + 4*qy + y + (d >= 0);
}

Year get_year(const Date date)
{
	Year year;
	parse_year(date, &year, &year);
	return year;
}

Day get_doy(const Date date) // day of year
{
	Day day;
	parse_year(date, &day, &day);
	return day;
}

void parse_month(const Date date, Year *year, Month *month, Day *day)
{
	parse_year(date, year, day);
	for (*month = 1; *month < 12; ++*month) {
		int len = month_length(*month, *year);
		if (*day <= len)
			return;
		*day -= len;
	}
	*month = 12;
}

Month get_month(const Date date)
{
	Month month;
	parse_month(date, &month, &month, &month);
	return month;
}

Day get_day(const Date date)
{
	Day day;
	parse_month(date, &day, &day, &day);
	return day;
}

void get_weekday(char *buf, const Date date)
{
	static const char *weekday_name[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	int res = date % 7;
	strcpy(buf, weekday_name[res < 0 ? res + 7 : res]);	// Dec 31 -0001 was a Sunday
}

// -------- output --------

void date_str(char *buf, const Date date)
{
	Year year; Month month; Day day;
	parse_month(date, &year, &month, &day);

	char weekdayname[10], monthname[10];
	get_weekday(weekdayname, date);
	month2str(monthname, month);

	sprintf(buf, "%s %s %d %d", weekdayname, monthname, day, year);
}

void date_printf(const Date date)
{
	char buf[256];
	date_str(buf, date);
	printf("%s", buf);
}

void cal_str(char *buf, const Date date)
{
	Year year; Month month; Day day;
	parse_month(date, &year, &month, &day);

	char monthname[10];
	month2str(monthname, month);
	sprintf(buf, "%s %d", monthname, year);
	int padding = (20 - strlen(buf)) / 2;
	for (int i = 0; i < padding; ++i)
		buf += sprintf(buf, " ");
	buf += sprintf(buf, "%s %d\n", monthname, year);
	buf += sprintf(buf, "Su Mo Tu We Th Fi Sa\n");

	int weekday = (date-day+1) % 7;
	int linebreak = 0;
	for (int i = 0; i < weekday; ++i)
		buf += sprintf(buf, "   ");
	int len = month_length(month, year);
	for (int i = 1; i <= len; ++i) {
		if (i == day) {
			// reverse color
			buf += sprintf(buf, "\033[7m%2d\033[0m ", i);
		} else {
			buf += sprintf(buf, "%2d ", i);
		}
		if ((weekday+i-1) % 7 == 6) {
			buf += sprintf(buf, "\n");
			++linebreak;
		}
	}
	if (linebreak < 5)
		buf += sprintf(buf, "\n");
	sprintf(buf, "\n");
}

void cal_printf(const Date date)
{
	char buf[1024];
	cal_str(buf, date);
	printf("%s", buf);
}

Date buildday()
{
	return str2date(__DATE__);
}

Date today()
{
	// 86400 = 60 * 60 * 24
	return YEAR1970 + time(NULL) / 86400;
}

#endif // Date_h
