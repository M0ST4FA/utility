

# Executable
add_executable(UtilityTests "tests.cpp" "iterable.cpp" "toString.cpp" "testIncludes.h")
target_link_libraries(UtilityTests PRIVATE utility)