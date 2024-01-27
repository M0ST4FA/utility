#include <vector>

#include "fmt/ranges.h"
#include "utility/common.h"
#include "testIncludes.h"

using namespace m0st4fa::utility;
int iterableTests() {

	auto comp = [](const std::vector<size_t>& it1, const std::vector<size_t>& it2) {

		if (operator==<std::vector<size_t>>(it1, it2))
			return "==";

		return "!=";
		};

	std::vector<size_t> it1{ 1, 2, 3, 4, 5, 6 };
	std::vector<size_t> it2{ 1, 2, 4, 3, 6, 5 };
	std::vector<size_t> it3{ 1, 5, 6, 7 };


	// Write more tests for more iterables
	std::cout << fmt::format("{} {} {}\n", toString(it1), comp(it1, it2), toString(it2));
	std::cout << fmt::format("{} {} {}\n", toString(it1), comp(it1, it3), toString(it3));

	return 0;
}
