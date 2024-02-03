#pragma once
#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <type_traits>
#include <concepts>
#include <source_location>

#include "fmt/ranges.h"
#include "tabulate/table.hpp"
#include "ANSI.h"

// DECLARATION OF utility NAMESPACE (AS INLINE)
namespace m0st4fa {
	inline namespace utility {}
}

// EXCEPTIONS
namespace m0st4fa::utility {

	/**
	 * @brief An exception to be thrown by a conversion function in case it failed to convert an object to another.
	 */
	struct ConversionError : std::exception {

		std::string msg{};

		const char* what() const noexcept(true) override {
			return "Conversion error.";
		}


		ConversionError(const std::string& msg)
			: msg(msg)
		{
		}
	};

}

// CONCEPTS
namespace m0st4fa::utility {

	/**
	 * @brief Checks whether an object of type `T` is convertible to string by having an `operator std::string` conversion function.
	 * @tparam T The type of the object to be checked.
	 */
	template <typename T>
	concept ConvertableToString = std::is_convertible_v<T, std::string>;

	/**
	 * @brief Checks whether an object of type `T` is convertible to string by calling a function named `stringfy`.
	 * @tparam T The type of the object to be checked.
	 */
	template <typename T>
	concept Stringfyble = requires (T a) {
		stringfy(a);
	};

	/**
	 * @brief Checks whether a numerical object of type `T` is convertible to string via the `std::to_string` function.
	 * @tparam T The type of the numerical object to be checked.
	 */
	template <typename T>
	concept NumConvertableToString = requires (T a) { std::to_string(a); };

	/**
	 * @brief Checks for whether an iterable has a method named `contains`.
	 * @tparam T The type of the iterable object to be checked.
	 */
	template <typename T>
	concept HasContains = requires (T iterable) { 
		iterable.contains(iterable.at(0)); 
	};

	///**
	// * @brief Checks whether an object type `T` supports comparison with less than operator (<).
	// * @tparam T The type of the object to be checked.
	// */
	//template <typename T>
	//concept HasLessThan = requires (T a, T b) {
	//	a < b;
	//};

}

// STRING
namespace m0st4fa::utility {

	/**
	 * @brief Converts general iterables to strings.
	 * @attention The elements of the iterable must be convertible to strings.
	 * @tparam T The type of the iterable.
	 * @param[in] iterable The iterable to be converted to a string.
	 * @param[in] asList Whether or not to format the iterable graphically as a list. If set to false, each element appears on a new line.
	 * @return The string representation of the iterable.
	 */
	template <typename T>
	std::string toString(const T& iterable, bool asList = true) {
		using ElementType = decltype(T{}.at(0));

		std::string separator = asList ? ", " : "\n";

		std::string temp = "{ ";

		if (iterable.empty())
			return temp += " }";

		if constexpr (NumConvertableToString<ElementType>)
			temp += std::to_string(*iterable.begin());
		else if (Stringfyble<ElementType>)
			temp += stringfy(*iterable.begin());
		else
			temp += (std::string)*iterable.begin();

		for (size_t i = 0; const auto & element : iterable) {
			// skip the first element
			if (!i) {
				i++;
				continue;
			}

			if constexpr (NumConvertableToString<ElementType>)
				temp += separator + std::to_string(element);
			else if (Stringfyble<ElementType>)
				temp += separator + stringfy(element);
			else
				temp += separator + (std::string)element;
		}

		return temp += " }";
	}

	/**
	 * @brief Converts a 2D array to a string.
	 * @attention The elements of the array must be convertible to strings.
	 * @tparam T The type of objects of the 2D array.
	 * @tparam xdim The size of the first dimension (x dimension).
	 * @tparam ydim The size of the second dimension (y dimension).
	 * @param[in] array The 2D array to be converted.
	 * @param[in] asList Whether or not to format the array graphically as a list. If set to false, each element appears on a new line.
	 * @returns The string representation of `array`.
	 * 
	 */
	template <NumConvertableToString T, size_t xdim, size_t ydim>
	std::string toString(const std::array<std::array<T, ydim>, xdim>& array, bool asList = true) {
			std::string separator = asList ? ", " : "\n";

			std::string temp = "{ ";

			if (array.empty())
				return temp += " }";

			// if the array is not empty

			for (size_t x = 0; const std::array<T, ydim>&subarr : array) {

				if (subarr.empty()) {
					x++;
					continue;
				}

				for (size_t y = 0; T element : subarr) {
					if (!element) {
						y++;
						continue;
					}

					temp += fmt::format("[{}][{}] = {}\n", x, y, subarr.at(y));
					y++;
				}

				x++;
			}

			return temp += " }";
		}

	/**
	 * @brief Converts a map to a string.
	 * @param[in] map The map to be converted to a string.
	 * @tparam K The type of map keys.
	 * @tparam V The type of map values.
	 * @return The string representation of `map`.
	 */
	template<typename K, typename V>
	std::string toString(const std::map<K, V>& map) {
			using MapType = std::map<K, V>;
			std::string res;

			for (const auto& pair : map)
				// FORMAT:
				// Key : Value
				res += fmt::format("{} : {}\n", pair.first, toString(pair.second));

			return res;
		};

	std::string toString(const std::source_location&, bool = false);
	
	/**
	 * @brief Formats a 2D table (represented by vectors) as a string.
	 * @todo Make this more general. Now it is designed to convert only FSMTable objects.
	 * @tparam E The type of objects within the 2D table.
	 * @param[in] table2D The 2D table to be formatted as a string.
	 * @param[in] getNonEmptyColumns A function that should filter columns according to which one is empty and which is not. The criterion is defined by this callback function itself. Empty columns will not exist in the formatted table. The two inputs the function takes are 
	 * @return A string representation of `table2D`.
	 */
	template<typename E>
	std::string toString(const std::vector<std::vector<E>>& table2D, std::function<std::set<size_t>(const std::vector<std::vector<E>>&)> getNonEmptyColumns) {

		using TableType = const std::vector<std::vector<E>>;

		using namespace tabulate;

		// TODO: MAKE THIS MORE GENERAL. NOW IT WILL BE ONLY FOR FSMTABLES

		/* ALGORITHM:
		* Idea: The idea is simple. Each row in the original vector table will have a corresponding row in the string table. However, some of the columns in the original table have no vale in any row. We call these "EMPTY COLUMNS". These columns should not be represented in the final string, as they will clog up the view. Therefore, we must start be determining which columns are EMPTY and which are not. Then we can put them in the string.
		* Inputs: table2D, columnFilterFunction
		* Steps:
			* For every row in `table2D`:
				* Record the index of the columns in that table that have a value (i.e., loop through every column and record those that are not empty).
			* Take the list of columns of each row and combine it into a single list. This is the list of all columns that must be converted to string. All other columns must be ignored.
			* Add a header row for each NON-EMPTY column.
			* For every row in `table2D`:
				* For every column (column index):
					* If that column has a value within `table2D`, add it.
					* If that column doesn't have a value or doesn't even exist within `table2D`, add empty space.
		*/

		// Helper lambdas

		// Gets the number of columns in each row, i.e., the size of each row
		auto get_column_num_in_each_row = [](const TableType& table2D) {
			std::vector<size_t> rowSizes{};
			for (const auto& row : table2D) {
				rowSizes.push_back(row.size());
			}

			return rowSizes;
			};

		// Get the indicies of non-empty columns
		std::set<size_t> nonEmptyColumns = getNonEmptyColumns(table2D);

		// Set up the header rows
		
		Table fmtTable{};
		Table::Row_t headerRow{};

		headerRow.push_back("State");

		for (const size_t col : nonEmptyColumns)
			headerRow.push_back(std::string{char(col)});

		fmtTable.add_row(headerRow);

		// Add the columns of each row
		for (size_t rowIndex = 1; const auto & row : table2D) {
			Table::Row_t currentRow{};

			const size_t rowSize = row.size();

			currentRow.push_back(std::to_string(rowIndex - 1));

			for (const size_t col : nonEmptyColumns) {
				
				if (col >= row.size())
					currentRow.push_back("");
				else {
					const auto it = row.at(col).begin();

					if (it == row.at(col).end())
						currentRow.push_back("");
					else {
						const size_t currState = *it;
						currentRow.push_back(std::to_string(currState));
					}

				}

			}

			fmtTable.add_row(currentRow);
			rowIndex++;
		}

		return fmtTable.str();

	};

}

// INTEGER
namespace m0st4fa::utility {

	
	size_t toInteger(const std::string&);

	size_t pow(size_t, size_t);

}

// ITERABLE
namespace m0st4fa::utility {

	/**
	* @brief Inserts elements from an iterable to another, filtering for some element (avoiding its insertion).
	* @tparam IterableT The type of both iterables (the iterable to insert from and the one to insert to).
	* @attention `IterableT` must have a method named `insert`.
	* @tparam ExceptT The type of the object to filter by. This object will never be inserted from `from` to `to`.
	* @note If the type of `except` is nullptr_t, it is ignored and all elements are added.
	* @param[in] from The iterable whose elements will be inserted to `to`.
	* @param[out] to The iterable into which `from` elements will be inserted.
	* @return `true` if at least one element has been inserted from `from` into `to`; `false` otherwise.
	*/
	template <typename IterableT, typename ExceptT>
	bool insertAndAssert(const IterableT& from, IterableT& to, ExceptT except) {
			bool added = false;

			for (auto element : from) {
				if (element == except)
					continue;

				auto p = to.insert(element);

				if (p.second)
					added = true;
			}

			return added;
		};

	/**
	* @brief Inserts elements from an iterable to another.
	* @tparam IterableT The type of both iterables (the iterable to insert from and the one to insert to).
	* @attention `IterableT` must have a method named `insert`.
	* @param[in] from The iterable whose elements will be inserted to `to`.
	* @param[out] to The iterable into which `from` elements will be inserted.
	* @return `true` if at least one element has been inserted from `from` into `to`; `false` otherwise.
	*/
	template <typename IterableT>
	bool insertAndAssert(const IterableT& from, IterableT& to) {
			bool added = false;

			for (auto element : from) {
				auto p = to.insert(element);

				if (p.second)
					added = true;
			}

			return added;
		};

	/**
	 * @brief Checks for whether a given element is in (is an element of) a given iterable. 
	 * @details Intended to implement the "containment/is element of" operator in set theory.
	 * @tparam ElementT The type of the element to check for whether it exists in `iterable` or not.
	 * @tparam IterableT The type of the iterable that will be checked for containment of `element`.
	 * @param[in] element The element whose existence in `iterable` will be checked.
	 * @param[in] iterable The iterable that will be checked for containment of `element`.
	 * @return `true` if `element` is in iterable; `false` otherwise.
	 */
	template <typename ElementT, typename IterableT>
	bool isIn(const ElementT element, const IterableT& iterable) {
			for (const ElementT& e : iterable)
				if (e == element)
					return true;

			return false;
		}

	/**
	* @brief Checks for whether two iterables are "equal" as in set theory.
	* @details The definition of equality of iterables used by this function is the same as that in set theory (assuming the iterables are sets). Two iterables are equal iff every element in one of them is an element in the other. The order is not important here.
	* @attention Elements of the iterable must support comparison using "==".
	* @tparam IterableT The type of the iterables to be compared.
	* @param[in] lhs The left-hand-side of the equality.
	* @param[in] rhs The right-hand-side of the equality.
	* @return `true` if both iterables are equal; `false` otherwise.
	*/
	template<typename IterableT>
		requires requires (IterableT a) {
			a.at(0) == a.at(1);
		}
	bool operator==(const IterableT& lhs, const IterableT& rhs) {

		const size_t lhsSize = lhs.size();
		const size_t rhsSize = rhs.size();

		// if they have different sizes, they are not equal
		if (lhsSize != rhsSize)
			return false;

		// if they have the same number of elements
		if constexpr (HasContains<IterableT>) {

			// if, for every element `a` in `lhs`, `a` is also an element
			// of `rhs`, then they are coequal

			for (const auto& lhsElement : lhs) {
				// check whether `rhs` contains that element
				if (rhs.contains(lhsElement))
					continue;

				// if `rhs` does not contain that element
				return false;
			}

		}
		else {
			// if there exists at least a single element in `lhs`
			// that does not exist in `rhs`, they are not equal
			for (size_t i = 0; i < lhsSize; i++) {
				const auto& lhsElement = lhs.at(i);

				for (size_t j = 0; j < rhsSize; j++) {
					const auto& rhsElement = rhs.at(j);

					// if the current elements are not equal, the two sets are unequal
					if (lhsElement == rhsElement)
						break;


					if (j == rhsSize - 1)
						return false;
				}

			}
		}

		// if we reach here, it means the two iterables are not `not-equal`
		// hence they must be equal
		return true;
	}

}

// INTERVALS
namespace m0st4fa::utility {

	/**
	 * @brief Checks for whether `element` is within (an element of) the interval `(lb, ub)` or `[lb, ub]`.
	 * @tparam T The type of the objects contained in the interval. These must support comparison.
	 * @attention The objects in the interval must support comparison.
	 * @note The word "interval" as used here in this context is very general. It doesn't refer only to a subset of real numbers, however, it is more general and can refer, for example, to subsets of integers.
	 * @param[in] element The element that will be checked for falling within a given range.
	 * @param[in] lb The lower bound of the interval.
	 * @param[in] ub The upper bound of the interval.
	 * @param[in] closed Whether the interval is closed or not.
	 * @return `true` if `element` falls within range; `false` otherwise.
	 */
	template<typename T>
		requires requires(T a, T b) {
			a < b;
			a > b;
			a >= b;
			a <= b;
		}
	inline bool withinInterval(T element, T lb, T ub, bool closed = false) {
		if (closed)
			return (element >= lb && element <= ub);

		return (element > lb && element < ub);
	};

}