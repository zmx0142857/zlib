#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <sstream>

class Test: public std::ostringstream {

protected:
	int count = 0;

public:
	bool test(const std::string &s="") {
		++count;
		std::string this_str(this->str());
		str("");					// clear inner string
		if (this_str == s)
			return true;
		std::cerr << "-------- test #" << count << " --------\n"
					 "expected:\n"
				  << s << "\n\n"
					 "got:\n"
				  << this_str << "\n\n";
		return false;
	}
};

#endif // TEST_H
