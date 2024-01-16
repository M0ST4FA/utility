String Functions
================

String Conversion for Iterables
-------------------------------

.. doxygenfunction:: m0st4fa::utility::toString(const T &iterable, bool asList = true)

.. doxygenfunction:: m0st4fa::utility::toString(const std::array<std::array<T, ydim>, xdim> &array, bool asList = true)

.. doxygenfunction:: m0st4fa::utility::toString(const std::map<K, V> &map)

.. doxygenfunction:: m0st4fa::utility::toString(const std::vector<std::vector<E>> &table2D, std::function<std::vector<bool>(const std::vector<std::vector<E>>&, const size_t)> getNonEmptyColumns)

String Conversion for Non-iterables
-----------------------------------

.. doxygenfunction:: m0st4fa::utility::toString(const std::source_location &srcloc, bool full)
  

