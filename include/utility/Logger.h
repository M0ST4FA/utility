#pragma once

#include <iostream>
#include <cstdarg>
#include <format>
#include <source_location>

#include "common.h"
//#define _TRACE


// EXCEPTIONS
namespace m0st4fa {

	struct UnknownLogLevel : std::exception {

		UnknownLogLevel() = default;

		const char* what() const noexcept(true) override {
			return "Unknown log level. Make sure you used one of the following log levels: LL_FATAL_ERROR, LL_ERROR, LL_WARNING, LL_INFO or LL_DEBUG.";
		}

	};

}


// DECLARATIONS
namespace m0st4fa {
	
	enum class LOG_LEVEL {
		LL_FATAL_ERROR,
		LL_ERROR,
		LL_WARRNING,
		LL_INFO,
		LL_DEBUG,
		LL_LOG_LEVEL_COUNT
	};

	struct LoggerInfo {
		LOG_LEVEL level;

		static const LoggerInfo LL_ERROR, LL_WARNING, LL_INFO, LL_DEBUG, LL_FATAL_ERROR;
	};
	
	class Logger {

		// static assertions to remind me of making some changing related to the number of 
		// these enumerators
		static_assert((int)LOG_LEVEL::LL_LOG_LEVEL_COUNT == 5 && "Change LOG_LEVEL_STRING");

		static constexpr const volatile char* LOG_LEVEL_STRING[(int)LOG_LEVEL::LL_LOG_LEVEL_COUNT] = {
			"FATAL ERROR",
			"ERROR",
			"WARNING",
			"INFO",
			"DEBUG",
		};

	public:

		void log(const LoggerInfo, const std::string&, std::source_location = std::source_location::current()) const;
		
		void logDebug(const std::string&, std::source_location = std::source_location::current()) const;

		inline std::string getCurrSourceLocation(std::source_location location = std::source_location::current()) const {
			std::string messageStr;
			messageStr += std::string("\nFile Name: ") + location.file_name() + std::string("\n");
			messageStr += std::string("Line: ") + std::to_string(location.line()) + std::string(", Character: ") + std::to_string(location.column()) + "\n";
			messageStr += std::string("Function: ") + location.function_name() + "\n";

			return messageStr;
		};

	};


}
