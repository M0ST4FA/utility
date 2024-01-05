#include <string>
#include <set>
#include <type_traits>
#include <concepts>
#include <format>

#include "utility/common.h"
#include "utility/Logger.h"

namespace m0st4fa::utility {

	// STRING

	/**
	 * @brief Converts a `std::source_location` object to a string.
	 * @param[in] srcloc The `std::source_location` object to be converted.
	 * @param[in] full Whether or not to add information about the file and the function from which this function is called. The default is to include only information about the current line and character.
	 * @return The string representation of `srcloc`.
	 */
	std::string toString(const std::source_location& srcloc, bool full) {
			std::string msg;

			msg += full ? (srcloc.file_name()) : "";
			msg += std::format("(line: {}, char: {}): ", srcloc.line(), srcloc.column());
			msg += full ? (srcloc.function_name() + std::string("()")) : std::string("");

			return msg;
		}

	// INTEGER

	/**
	 * @brief Converts a string to an integer.
	 * @attention Right now it converts only to positive (unsigned) integers.
	 * @param str The string to be converted to an integer.
	 * @todo Implement `atBeginning` bool parameter. This should allow the function to parse and convert integers within a string, not just at its beginning.
	 * @todo Implement conversion of negative integers.
	 * @return An integer representation of the string.
	 */
	size_t toInteger(const std::string& str) {
		// find the first integer
		auto itr = std::find_if(str.begin(), str.end(), isalnum);

		if (itr == str.end()) {
			Logger logger;

			std::string msg = std::format("Could not convert the string `{}` into integer (Could not even extract an integer from it.)", str);

			logger.log(LoggerInfo::LL_ERROR, msg);
			throw ConversionError{msg};
		}

		size_t res = 0;

		for (; itr < str.end(); itr++) {
			const char currChar = *itr;

			// if the current character is not numeric
			if (not isalnum(currChar))
				break;

			size_t numVal = ((size_t)currChar - '0');
			res += numVal;
			res *= 10;
		}

		return res / 10;
	}

	/**
	 * @brief Raise `base` to the power of `p` and return the result.
	 * @todo Right now it is brute force...and it is very slow. Implement a more sophisticated algorithm.
	 * @param base The base of the exponentiation.
	 * @param p The power of the exponentiation.
	 * @return `base` raised to the power of `p`.
	 */
	size_t pow(size_t base, size_t p) {
			if (p == 0)
				return 1;

			size_t res = base;

			for (size_t i = 1; p > i; i++)
				res *= res;

			return res;
		}

}