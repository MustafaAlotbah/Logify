#include <iostream>
#include <Logify/Logify.h>
#include <Logify/ScopedLogger.h>


// Define the global logger instance
Logify::Logger logger;

// Example Functions
void initializeResources();
void processData();

int main(int argc, char* argv[]) {
	// Display the version of the Logify library.
	std::cout << "Logify Example, Version: " << Logify::getVersion() << std::endl;

	// Initialize the logger instance with the default log level (INFO).
	logger.setLogLevel(Logify::LogLevel::INFO);

	// Activate Indentation inside scopes
	logger.setIndentation(true);

	// Add an output stream to the logger, directing logs to the console (std::cout).
	logger.addOutputStream(std::cout);

	// Add file streams to the logger, logging to both a text file and an HTML file with rotation.
	// The HTML file uses a color scheme defined in the library.
	logger.addFileStream("application.log");
	logger.addFileStream("application.html", 5 * 1024 * 1024, Logify::DefaultDarkScheme);

	// Set a custom time format for the logs.
	logger.setTimeFormat("%Y-%m-%d %H:%M:%S");

	// Start of the main process.
	logger.info("Starting the application...");

	// Call function to simulate initialization of resources.
	initializeResources(); // Logs entry and exit using the default log level (INFO)

	// Call function to simulate processing some data.
	processData(); // Logs entry and exit using the DEBUG log level

	// Simulate a critical section where important decisions are made.
	{
		LOGIFY_SCOPED_LOGGER();

		logger.info("Evaluating decision criteria...");
		// Simulate a critical error
		logger.fatal("Critical error: insufficient data to make a decision. Aborting operation.");

		logger.warn("Attempting to recover from critical error...");
		// Simulate recovery attempt
		logger.info("Recovery successful. Proceeding with limited functionality.");

		logger.trace("Critical decision-making phase completed.");
	} // Logs the end of the critical decision-making scope automatically with duration.

	// Simulate the completion of the main process.
	logger.info("Finalizing the application...");

	logger.debug("Releasing resources...");
	// Simulate a successful resource release
	logger.info("Resources released successfully.");

	// Additional logging examples for other levels:
	logger.trace("This is a TRACE level message for very fine-grained logging.");
	logger.debug("This is a DEBUG level message to help during debugging.");
	logger.info("This is an INFO level message to indicate regular operation.");
	logger.warn("This is a WARN level message to indicate a potential problem.");
	logger.error("This is an ERROR level message to indicate a serious issue.");
	logger.fatal("This is a FATAL level message to indicate a critical problem causing the application to abort.");

	// End of the main process.
	logger.info("Application has completed its execution.");
	logger.debug("Exiting with code 0.");

	return 0; // End of the program.
}

void initializeResources()
{
	// Using the macro with the default INFO log level
	LOGIFY_SCOPED_LOGGER();

	logger.trace("Entering initialization phase...");
	logger.info("Loading configuration...");
	logger.debug("Configuration file: config.json");

	logger.info("Initializing database connection...");
	// Simulate a successful connection
	logger.info("Database connection established successfully.");

	logger.info("Setting up application environment...");
	// Simulate a warning during setup
	logger.warn("Environment variable 'APP_MODE' not set. Defaulting to 'production'.");

	logger.trace("Initialization phase completed.");
}

void processData()
{
	// Using the macro with a specific DEBUG log level
	LOGIFY_SCOPED_LOGGER_LEVEL(Logify::LogLevel::DEBUG);

	logger.info("Starting data processing...");

	logger.debug("Reading data from source A...");
	// Simulate a delay
	logger.info("Data successfully read from source A.");

	logger.debug("Processing data...");
	// Simulate an error during data processing
	logger.error("Data processing failed due to invalid format in record 42.");

	logger.info("Applying fallback processing...");
	// Simulate fallback success
	logger.info("Fallback processing completed successfully.");

	logger.trace("Data processing phase completed.");
}
