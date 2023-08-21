# CMakeList.txt : CMake project for utility, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# DECLARE THE PROJECT
project ("Utility")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ADD THE TARGETS
add_library(utility 
"${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp")
target_include_directories(utility PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")