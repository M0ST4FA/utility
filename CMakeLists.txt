# CMakeList.txt : CMake project for utility, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# DECLARE THE PROJECT
project ("utility")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(${PROJECT_NAME}_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/")

# ADD THE DEPENDENCY LIBRARIES
add_subdirectory("${PROJECT_SOURCE_DIR}/external/fmt")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/tabulate")

# ADD THE TARGETS
add_library(${PROJECT_NAME} 
"${PROJECT_SOURCE_DIR}/src/common.cpp" 
"${PROJECT_SOURCE_DIR}/src/Logger.cpp"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/ANSI.h"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/common.h"
"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/Logger.h"
)
target_include_directories(${PROJECT_NAME} PUBLIC 
"${${PROJECT_NAME}_INCLUDE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt tabulate::tabulate)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC ${PROJECT_NAME})

if (${BUILD_DOCUMENTATION})
	add_subdirectory("${PROJECT_SOURCE_DIR}/docs/")
endif()