// useful functions, like sto<T>() are defined here.
#ifndef Cheat_h
#define Cheat_h

#include <string>		// isdigit()
#include <stdexcept>	// std::invalid_argument
#include <cmath>		// pow()

// convert a string to a number, like int, unsigned, double...
template <typename T>
inline T sto(const std::string &s)
{
    auto beg = s.cbegin();
	return sto<T>(beg, s.cend());
}

// save the unconverted pos in 'it' for further processes
template <typename T>
T sto(std::string::const_iterator &it, const std::string::const_iterator ce, bool sign = true)
{
	T ret = 0;
	bool dot_appeared = false;
	double fac = 1.0;					// fac: factor

	for (auto cb = it; it != ce; ++it)
	{
		if (sign && it == cb)
		{
			if (*it == '+')				// unary operator +
			{
				if ( ++it == ce || !isdigit(*it) )
					throw std::invalid_argument("Missing value after '+'.");
			}
			else if (*it == '-')			// unary operator -
			{
				if ( ++it == ce || !isdigit(*it) )
					throw std::invalid_argument("Missing value after '-'.");
				fac = -fac;
			}
		}

		if (isspace(*it))
		{
			continue;
		}
		else if (*it == '.')
		{
			if (!dot_appeared)			// first time meet a dot
				dot_appeared = true;	// set the flag
			else						// if its not the first time
				break;					// end the loop
		}
		else if (*it == 'e' || *it == 'E')
		{
			if (++it != ce)				// 'e' is not the last char in str
				fac *= pow(10, sto<int>(it, ce, true));
			break;
		}
		else if (isdigit(*it))
		{
			ret = 10 * ret + (*it) - '0';
			if (dot_appeared)
				fac *= 0.1;
		}
		else
			break;
	} // for ends

	return ret*fac;
}

// [C only] convert a c_str to unsigned
unsigned stou(const char *c)
{
	unsigned ret = 0;
	while (isdigit(*c))
	{
		ret = 10 * ret + *c - '0';
		++c;
	}
	return ret;
}

#endif
