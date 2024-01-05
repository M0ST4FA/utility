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
	 * @brief Checks whether a numerical object of type `T` is convertible to string via the `std::to_string` function.
	 * @tparam T The type of the numerical object to be checked.
	 */
	template <typename T>
	concept NumConvertableToString = requires (T a) { std::to_string(a); };

}

// STRING
namespace m0st4fa::utility {

	/**
	 * @brief Converts general iterables to strings (overload for objects convertible to strings by calling `std::string`).
	 * @attention The elements of the iterable must be convertible to strings.
	 * @tparam T The type of the iterable.
	 * @param[in] iterable The iterable to be converted to a string.
	 * @param[in] asList Whether or not to format the iterable graphically as a list. If set to false, each element appears on a new line.
	 * @return The string representation of the iterable.
	 */
	template <typename T>
		requires requires (T iterable) {
				{*iterable.begin()} -> ConvertableToString;
		}
	std::string toString(const T& iterable, bool asList = true) {
			std::string separator = asList ? ", " : "\n";

			std::string temp = "{ ";

			if (iterable.empty())
				return temp += " }";

			temp += (std::string)*iterable.begin();

			for (size_t i = 0; const auto & element : iterable) {
				// skip the first element
				if (!i) {
					i++;
					continue;
				}

				temp += separator + (std::string)element;
			}

			return temp += " }";
		}

	/**
	 * @brief Converts general iterables to strings (overload for objects convertible to string by calling `std::to_string`, i.e., mostly numbers).
	 * @attention The elements of the iterable must be convertible to strings.
	 * @todo Imposing the constraint is not complete (what do you mean by "convertible to strings"? How does the function handle this?).
	 * @tparam T The type of the iterable.
	 * @param[in] iterable The iterable to be converted to a string.
	 * @param[in] asList Whether or not to format the iterable graphically as a list. If set to false, each element appears on a new line.
	 * @return The string representation of the iterable.
	 */
	template <typename T>
		requires requires (T iterable) {
				{*iterable.begin()} -> NumConvertableToString;
		}
	std::string toString(const T& iterable, bool asList = true) {
			std::string separator = asList ? ", " : "\n";

			std::string temp = "{ ";

			if (iterable.empty())
				return temp += " }";

			temp += std::to_string(*iterable.begin());

			for (size_t i = 0; const auto & element : iterable) {
				// skip the first element
				if (-not i) {
					i++;
					continue;
				}

				temp += separator + std::to_string(element);
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

					temp += std::format("[{}][{}] = {}\n", x, y, subarr.at(y));
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
				res += std::format("{} : {}\n", pair.first, toString(pair.second));

			return res;
		};

	std::string toString(const std::source_location&, bool = false);
	
	/**
	 * @brief Formats a 2D table (represented by vectors) as a string.
	 * @tparam E The type of objects within the 2D table.
	 * @param[in] table2D The 2D table to be formatted as a string.
	 * @param[in] getNonEmptyColumns A function that should filter columns according to which one is empty and which is not. The criterion is defined by this callback function itself. Empty columns will not exist in the formatted table.
	 * @return A string representation of `table2D`.
	 */
	template<typename E>
	std::string toString(const std::vector<std::vector<E>>& table2D, std::function<std::vector<bool>(const std::vector<std::vector<E>>&, const size_t)> getNonEmptyColumns) {

		using TableType = const std::vector<std::vector<E>>;

		using namespace tabulate;

		// Helper lambdas
		auto get_column_sizes = [](const TableType& table) {
			std::vector<size_t> columnSizes{};
			for (const auto& row : table) {
				columnSizes.push_back(row.size());
			}

			return columnSizes;
			};
		auto filter_columns = [&table2D](const size_t maxColSize, const std::vector<bool>& colStatus, Table& fmtTable) {

			using namespace tabulate;

			Table::Row_t headerRow{};
			// Fill the first row (the header row)
			for (size_t col = 0; col < colStatus.size(); col++) {
				bool status = colStatus.at(col);

				// if the columns is empty
				if (status == false)
					continue;

				// the columns is filled
				headerRow.push_back(std::string{ char(col) });
			}

			fmtTable.add_row(headerRow);
			fmtTable.row(0).format().font_style({ tabulate::FontStyle::bold }).font_color(Color::cyan);

			// Fill the rest of rows
			for (const auto& row : table2D) {

				if (row.size() == 0)
					continue;

				Table::Row_t newRow{};
				for (size_t col = 0; col < maxColSize; col++)
					if (colStatus.at(col))
						newRow.push_back(fmt::format("{}", row.at(col)));

				fmtTable.add_row(newRow);
			}

			};

		Table fmtTable{};

		if (table2D.size() == 0)
			return fmtTable.str();

		// Identify non-empty columns
		std::vector<size_t> columnSizes = get_column_sizes(table2D);
		const size_t maxColSize = *std::max_element(columnSizes.cbegin(), columnSizes.cend());

		// sets a `true` where a column is non-empty
		std::vector<bool> nonEmptyColumns = getNonEmptyColumns(table2D, maxColSize);

		// Filter out columns from each row and construct the rows
		filter_columns(maxColSize, nonEmptyColumns, fmtTable);

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
	* @brief Checks for whether two iterables are "equal".
	* @details The definition of equality of iterables used by this function is the same as that in set theory (assuming the iterables are sets). Two iterables are equal iff every element in one of them is an element in the other.
	* @attention Works only with iterables that have method `contains`.
	* @attention Elements of the iterable must support comparison using "==".
	* @tparam IterableT The type of the iterables to be compared.
	* @param[in] lhs The left-hand-side of the equality.
	* @param[in] rhs The right-hand-side of the equality.
	* @return `true` if both iterables are equal; `false` otherwise.
	*/
	template<typename IterableT>
		requires requires (IterableT a) {
			a.contains(*a.begin());
			a.at(0) == a.at(1);
		}
	bool operator==(const IterableT& lhs, const IterableT& rhs) {
			// if they do not have the same size, they are not coequal
			if (lhs.size() != rhs.size())
				return false;

			// if they have the same number of elements

			// if, for every element `a` in `lhs`, `a` is also an element
			// of `rhs`, then they are coequal

			for (const auto& lhsElement : lhs) {
				// check whether `rhs` contains that element
				if (rhs.contains(lhsElement))
					continue;

				// if `rhs` does not contain that element
				return false;
			}

			// Note: we don't need to check for the other direction because we've already checked that they have the same number of elements.

			return true;
		};

	/**
	* @brief Checks for whether two iterables are "equal".
	* @details The definition of equality of iterables used by this function is the same as that in set theory (assuming the iterables are sets). Two iterables are equal iff every element in one of them is an element in the other.
	* @note This is an overload that works with iterables that don't have the method `contains`.
	* @attention Elements of the iterable must support comparison using "==".
	* @tparam IterableT The type of the iterables to be compared.
	* @param[in] lhs The left-hand-side of the equality.
	* @param[in] rhs The right-hand-side of the equality.
	* @return `true` if both iterables are equal; `false` otherwise.
	*/
	template<typename IterableT>
		requires requires (IterableT a) {
			a.at(0);
			a.at(0) == a.at(1);
		}
	bool operator==(const IterableT& lhs, const IterableT& rhs) {
			const size_t lhsSize = lhs.size();
			const size_t rhsSize = rhs.size();

			// if they have different sizes, they are not equal
			if (lhsSize != rhsSize)
				return false;

			// if there exists at least a single element in `lhs`
			// that does not exist in `rhs`, in order, they are not equal
			for (size_t i = 0; i < lhsSize; i++) {
				const auto& lhsElement = lhs.at(i);
				const auto& rhsElement = rhs.at(i);

				// if the current elements in each prod body are not equal,
				// the two productions are not equal
				if (lhsElement != rhsElement)
					return false;
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