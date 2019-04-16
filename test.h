#ifndef Test_h
#define Test_h

#ifdef __cplusplus

#include <iostream>
#include <sstream>
#include <cstdlib>

class Test: public std::ostringstream {

protected:
	int count = 0;

public:
	bool test(const std::string &s="") {
		++count;
		if (this->str() == s) {
			str("");					// clear inner string
			return true;
		} else {
			std::cerr << "-------- test #" << count << " --------\n"
					 "expected:\n"
				  << s << "\n\n"
					 "got:\n"
				  << this->str() << "\n\n";
			str("");					// clear inner string
			return false;
		}
	}
};

#else // __cplusplus

int test_count = 0;
char test_buffer_pool[100000];
char *test_buffer = test_buffer_pool;

#define tprintf(args) test_buffer = sprintf(test_buffer, args)

int test(const char *str)
{
	++test_count;
	if (strcmp(test_buffer, str) == 0) {
		test_buffer = test_buffer_pool;
		return 1;
	} else {
		printf("-------- test #%d --------\n"
			   "expected:\n"
			   "%s\n\n"
			   "got:\n"
			   "%s\n\n", test_count, str, test_buffer);
		test_buffer = test_buffer_pool;
		return 0;
	}
}

#endif // __cplusplus

#endif // Test_h
