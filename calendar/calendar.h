#ifndef Calendar_h
#define Calendar_h

#include <stdio.h>
#include <stdlib.h>

typedef int Date;				// 0 = Dec 31 1600, 1 = Jan 1 1601
typedef int Year;
typedef int Month;
typedef int Day;

extern const Date YEAR2000;		// 145732
extern const int YEAR;			// 365
extern const int QUADYEAR;		// 1461
extern const int CENTURY;		// 36524
extern const int QUADCENTURY;	// 146097
extern const int BASE_YEAR;		// 1601

// constructors of date
extern Date int2date(const Year, const Month, const Day);

/* format:
 *   month_name day year
 *   day month_name year
 */
extern Date str2date(const char*);

// output
extern void date2str(char*, const Date);
extern void date_printf(const Date);
extern void cal2str(char*, const Date);
extern void cal_printf(const Date);

// operators
extern int date_cmp(Date,Date);	// return count of days between two dates

// extract infomation from type 'Date'
extern Year year(const Date);	// return current calendar year
extern Month month(const Date);
extern Day day_of_month(const Date);
extern Day day_of_year(const Date);
extern void day_of_week(char*, const Date);

// helper functions

extern int is_leap(const Year);	// return 1 if it is leap year
extern Month str2month(const char*);
extern void month2str(char*, const Month);
extern Day month_length(const Year, const Month);
extern void validate(Date*);	// return value >= 0

// counts since the base year
extern int year_cnt(const Date*);
extern int quadyear_cnt(const Date*);
extern int century_cnt(const Date*);
extern int quadcentury_cnt(const Date*);

// use system time to tell what day is today
extern Date today();

#endif // Calendar_h
