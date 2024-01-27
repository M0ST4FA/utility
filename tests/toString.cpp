#include <string>
#include "utility/common.h"
#include "testIncludes.h"

int toStringTests() {
	
	std::vector<int> someInts{1, 2, 3, 4, 5, 6};

	std::cout << fmt::format("Displaying the set of the first 6 positive integers: {}\n", m0st4fa::utility::toString(someInts));

	return 0;
}
