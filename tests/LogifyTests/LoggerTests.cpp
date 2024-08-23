#include <catch2/catch.hpp>
#include <Logify/Logify.h>
#include <sstream>
#include <string>


TEST_CASE("Logify Logger", "[Logger]")
{
	using namespace Logify;

	std::stringstream logStream;
	Logger            logger(LogLevel::INFO);
	logger.addOutputStream(logStream);


	SECTION("Logging at INFO level")
	{
		logger.info("Application has completed its execution.");

		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[INFO ]: Application has completed its execution.") != std::string::npos);
	}


	SECTION("Logging at WARN level")
	{
		logger.warn("This is a WARN level message to indicate a potential problem.");

		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[WARN ]: This is a WARN level message to indicate a potential problem.")
					!= std::string::npos);
	}

	SECTION("Logging at ERROR level")
	{
		logger.error("This is an ERROR level message to indicate a serious issue.");

		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[ERROR]: This is an ERROR level message to indicate a serious issue.")
					!= std::string::npos);
	}

	SECTION("Logging at FATAL level")
	{
		logger.fatal("This is a FATAL level message to indicate a critical problem causing the application to abort.");

		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find(
			"[FATAL]: This is a FATAL level message to indicate a critical problem causing the application to abort."
		) != std::string::npos);
	}

	SECTION("Setting LogLevel to DEBUG and logging at lower levels")
	{
		logger.setLogLevel(LogLevel::DEBUG);

		logger.debug("This is a DEBUG level message to provide detailed information.");
		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[DEBUG]: This is a DEBUG level message to provide detailed information.")
					!= std::string::npos);

		logger.trace("This is a TRACE level message for in-depth debugging.");
		logOutput = logStream.str();
		REQUIRE_FALSE(
			logOutput.find("[TRACE]: This is a TRACE level message for in-depth debugging.") != std::string::npos);
	}

	SECTION("Setting LogLevel to TRACE and logging at TRACE level")
	{
		logger.setLogLevel(LogLevel::TRACE);

		logger.trace("This is a TRACE level message for in-depth debugging.");
		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[TRACE]: This is a TRACE level message for in-depth debugging.") != std::string::npos);
	}

	SECTION("Time format setting")
	{
		logger.setTimeFormat("%Y-%m-%d %H:%M:%S");
		logger.info("Checking time format setting.");

		std::string logOutput = logStream.str();
		REQUIRE(logOutput.find("[INFO ]: Checking time format setting.") != std::string::npos);
		// Further checks could be added here to validate the timestamp format.
	}

	SECTION("Multiple output streams")
	{
		std::stringstream additionalStream;
		logger.addOutputStream(additionalStream);

		logger.info("This message should appear in both streams.");
		std::string logOutput1 = logStream.str();
		std::string logOutput2 = additionalStream.str();

		REQUIRE(logOutput1.find("[INFO ]: This message should appear in both streams.") != std::string::npos);
		REQUIRE(logOutput2.find("[INFO ]: This message should appear in both streams.") != std::string::npos);
	}


	// TODO File Streams

}