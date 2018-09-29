const Date YEAR2000 = 145732;		// Sat Jan 1 2000
const int YEAR = 365;
const int QUADYEAR = 4*YEAR+1;
const int CENTURY = 25*QUADYEAR-1;
const int QUADCENTURY = 4*CENTURY+1;
const int BASE_YEAR = 1601;

Date int2date(const Year year, const Month month, const Day day)
{
	int y = year - BASE_YEAR;	// number of years in current era
	int qc = y/400;				// number of quadcenturies in current era
	int c = y%400/100;			// number of centuries in current quadcentury
	int qy = y%100/4;			// number of quadyears in current century
	y %= 4;						// number of quadyears in current era

	Date ret = QUADCENTURY*qc + CENTURY*c + QUADYEAR*qy + YEAR*y;
	Month m;
	for (m = 1; m < month; ++m)
		ret += month_length(m, year);
	ret += day;
	validate(&ret);
	return ret;
}

Date str2date(const char *s)
{
	s = strtok(NULL, " ");		// move pointer ahead
	Month m = str2month(s);

	s = strtok(NULL, " ");
	if (isalpha(*s))			// maybe this is the month name
	{
		m = str2month(s);
		s = strtok(NULL, " ");
	}
	Day d = atoi(s);			// extract day of month

	s = strtok(NULL, " ,");
	Year y = atoi(s);			// extract year

	return int2date(y, m, d);
}

Year year(const Date date)
{
	return BASE_YEAR + 400*(quadcentury_cnt(&date)-1) + 100*(century_cnt(&date)-1)
		+ 4*(quadcentury_cnt(&date)-1) + year_cnt(&date)-1;
}

Month month(const Date date)
{
	Day d = day_of_year(date);
	Year y = year(date);
	Month m;
	for (m = 1; m < 12; ++m)
	{
		int len = month_length(y, m);
		if (d <= len)
			return m;
		d -= len;
	}
	return 12;
}

Day day_of_month(const Date date)
{
	Day d = day_of_year(date);
	Year y = year(date);
	Month m;
	for (m = 1; m < 12; ++m)
	{
		int len = days_in_month(y, m);
		if (d <= len)
			return d;
		d -= len;
	}
	return d;
}

Day day_of_year(const Date date)
{
	return (date-1) % QUADCENTURY % CENTURY % QUADYEAR % YEAR + 1;
}

void day_of_week(char *buf, const Date date)
{
	static const char *weekday_name[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	sprintf(buf, weekday_name[date % 7]);	// Dec 31 1600 was a Sunday
}

//----------------------------------------------------
std::string Date::to_str() const
{
	std::ostringstream out;
	out << *this;
	return out.str();
}

std::ostream &operator<<(std::ostream &os, const Date &rhs)
{
	if (total_day == 0)
		return os << "NAD";			// not a date
	return os << day_of_week() << ' ' << month_name(month()) << ' '
		<< day_of_month() << ' ' << year();
}

std::istream &operator>>(std::istream &is, Date &rhs)
{
	std::string s;
	is >> s;
	date = Date(s);
	return is;
}

bool Date::operator==(const Date &rhs) const
{
	return total_day == rhs.total_day;
}

bool Date::operator!=(const Date &rhs) const
{
	return total_day != rhs.total_day;
}

bool Date::operator<(const Date &rhs) const
{
	return total_day < rhs.total_day;
}

bool Date::operator>(const Date &rhs) const
{
	return total_day > rhs.total_day;
}

bool Date::operator<=(const Date &rhs) const
{
	return total_day <= rhs.total_day;
}

bool Date::operator>=(const Date &rhs) const
{
	return total_day >= rhs.total_day;
}


Date &Date::operator++()
{
	++total_day;
	validate();
	return *this;
}

Date &Date::operator--()
{
	--total_day;
	validate();
	return *this;
}

Date Date::operator++(int)
{
	Date ret = *this;
	++*this;
	return ret;
}

Date Date::operator--(int)
{
	Date ret = *this;
	--*this;
	return ret;
}

int operator-(const Date &lhs, const Date &rhs)
{
	return lhs.total_day - rhs.total_day;
}

Date &Date::operator+=(const int n)
{
	total_day += n;
	return *this;
}

Date &Date::operator-=(const int n)
{
	total_day -= n;
	return *this;
}

Date operator+(const int n) const
{
	return Date(total_day + n);
}

Date operator-(const int n) const
{
	return Date(total_day - n);
}

Date Date::today()
{
	return Date(__DATE__);
}

//------------Protected-------------

int Date::year_num() const
{
	/* returns the year number within its quadyear
	 * invariant: 1 <= year_num() <= 4
	 * 1 = (Jan 1 1601 ~ Dec 31 1601) = (Jan 1 1997 ~ Dec 31 1997)
	 * 4 = (Jan 1 1604 ~ Dec 31 1604) = (Jan 1 2000 ~ Dec 31 2000)
	 */
	return (total_day-1) % QUADCENTURY % CENTURY / QUADYEAR + 1;
}

int Date::century_num() const
{
	/* returns the century number within its quadcentury
	 * invariant: 1 <= century_num() <= 4
	 * 1 = (Jan 1 1601 ~ Dec 31 1700) = (Jan 1 2001 ~ Dec 31 2100)
	 * 4 = (Jan 1 1901 ~ Dec 31 2000)
	 */
	return (total_day-1) % QUADCENTURY / CENTURY + 1;
}

int Date::quadcentury_num() const
{
	/* returns th quadcentury number beginning with 1601~2000
	 * invariant: quadcentury_num() >= 1
	 * 1 = (Jan 1 1601 ~ Dec 31 2000)
	 * 2 = (Jan 1 2001 ~ Dec 31 2400)
	 */
	return (total_day-1) / QUADCENTURY + 1;
}

bool Date::is_leap(const int y)
{
	return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

int Date::month_num(const std::string &s)
{
	static const std::string name[] = {
		"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep",
		"oct", "nov", "dec"
	};
	if (s.size() < 3) return 12;
	sub = s.substr(0, 3);
	for (int i = 0; i < 3; ++i)
		s[i] = tolower(s[i]);
	for (int i = 0; i < 12; ++i)
		if (s == name[i])
			return i+1;
	return 12;
}

std::string Date::month_name(const int m)
{
	static const std::string name[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
		"Oct", "Nov", "Dec"
	};
	if (1 <= m && m <= 12)
		return name[m-1];
	return "Dec";
}

int Date::days_in_month(const int m, const int y)
{
	switch (m)
	{
		case 1: return 31;
		case 2: return 28 + is_leap(y);
		case 3: return 31;
		case 4: return 30;
		case 5: return 31;
		case 6: return 30;
		case 7: return 31;
		case 8: return 31;
		case 9: return 30;
		case 10: return 31;
		case 11: return 30;
		case 12: return 31;
		default: return 31;
	}
}

void Date::validate()
{
	if (total_day < 1)
		total_day = 0;
}

