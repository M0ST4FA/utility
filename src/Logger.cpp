#include "utility/Logger.h"

// FUNCTIONS
namespace m0st4fa {

	// IMPLEMENTATIONS OF Logger FUNCTIONS
	void Logger::log(LoggerInfo loggerInfo, const std::string& message, std::source_location location) const
	{

		// if logging a debug message
		if (loggerInfo.level == LOG_LEVEL::LL_DEBUG) {
			this->logDebug(message, location);
			return;
		};

		// if logging any other message
		const std::string logLevelStr = (const char *)LOG_LEVEL_STRING[static_cast<int>(loggerInfo.level)];
		/*const std::string errorTypeStr = (const char* )ERROR_TYPE_NAMES[static_cast<int>(loggerInfo.info.errorType)];*/

		std::string messageStr;
		
		if (loggerInfo.level == LOG_LEVEL::LL_ERROR)
			messageStr += std::format(ANSI_ERR_COLOR"[{:s}]: {:s}\n" ANSI_RESET_ALL, logLevelStr, message);
		else if (loggerInfo.level == LOG_LEVEL::LL_FATAL_ERROR)
			messageStr += std::format(ANSI_FATAL_COLOR"[{:s}]: {:s}\n" ANSI_RESET_ALL, logLevelStr, message);
		else
			messageStr += std::format(ANSI_INFO_COLOR"[{:s}]: {:s}\n" ANSI_RESET_ALL, logLevelStr, message);

#ifdef _DEBUG 
#ifdef _TRACE
		messageStr += this->getCurrSourceLocation(location);
#endif
#endif

		switch (loggerInfo.level) {
			case LOG_LEVEL::LL_FATAL_ERROR:
			case LOG_LEVEL::LL_ERROR:
				std::cerr << messageStr << "\n";
				return;
			case LOG_LEVEL::LL_WARRNING:
				std::cout << messageStr << "\n";
				return;
			case LOG_LEVEL::LL_INFO:
				std::clog << messageStr << "\n";
				return;
			default:
				throw UnknownLogLevel{};
		};

	}

#ifdef _DEBUG
	void Logger::logDebug(const std::string& message, std::source_location location) const
	{
		const std::string logLevelStr = (const char* ) LOG_LEVEL_STRING[(int)LOG_LEVEL::LL_DEBUG];
		std::string messageStr = std::format(ANSI_DEBUG_COLOR"[{:s}]: {:s}" ANSI_RESET_ALL, logLevelStr, message);
		
#ifdef _TRACE
		messageStr += this->getCurrSourceLocation(location);
#endif
		
		std::cout << messageStr << "\n";

		return;
	}
#else 
	void Logger::logDebug(const std::string&, std::source_location) const {};
#endif

}

// DEFINITIONS
namespace m0st4fa {

	const LoggerInfo LoggerInfo::DEBUG = { LOG_LEVEL::LL_DEBUG };
	const LoggerInfo LoggerInfo::ERROR = { LOG_LEVEL::LL_ERROR };
	const LoggerInfo LoggerInfo::FATAL_ERROR = { LOG_LEVEL::LL_FATAL_ERROR };
	const LoggerInfo LoggerInfo::INFO = { LOG_LEVEL::LL_INFO };
	const LoggerInfo LoggerInfo::WARNING = { LOG_LEVEL::LL_WARRNING };

}
