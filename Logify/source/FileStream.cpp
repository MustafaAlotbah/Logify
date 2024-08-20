
#include "FileStream.h"
#include <sstream>
#include <iomanip>
#include <filesystem>


Logify::FileStream::FileStream(const std::string& filename, std::size_t maxFileSize, const ColorScheme& scheme)
	: maxFileSize_(maxFileSize), fileIndex_(0), colorScheme_(scheme)
{

	// Extract the filename and its extension.
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos)
	{
		logFileName_   = filename.substr(0, dotPos); // Set the base name of the file.
		extensionName_ = extractExtension(filename);        // Extract and store the file extension.
	}
	else
	{
		// If no extension is found, default to using the entire filename with a ".log" extension.
		logFileName_   = filename;
		extensionName_ = "log";
	}

	// Determine the type of file extension (LOG or HTML).
	extension_ = determineExtensionType(extensionName_);

	// Ensure that the file index is incremented until no rotation is needed.
	while (shouldRotate()) ++fileIndex_;

	// Open the initial file for writing.
	openFile();
}

Logify::FileStream::~FileStream()
{
	if (fileStream_ && fileStream_->is_open())
	{
		// Close HTML tags if the file is in HTML format.
		if (extension_ == FileExtension::HTML) (*fileStream_) << HTML_ending;

		// Close the file stream.
		fileStream_->close();
	}
}

void Logify::FileStream::write(
	const std::string& timestamp,
	const std::string& pid,
	const std::string& tid,
	const std::string& level,
	const std::string& message,
	size_t indent
)
{
	// Check if the file needs to be rotated due to exceeding the max file size.
	if (shouldRotate()) rotateFile();

	// Ensure the file stream is open and valid.
	if (fileStream_ && fileStream_->is_open())
	{
		// Generate the indentation string
		std::string indentation(indent * 2, ' ');

		if (extension_ == FileExtension::HTML)
		{
			// If writing to an HTML file, replace newlines in the message with <br> tags for formatting.
			auto message_ = replace(message, "\n", "<br>");

			// Create a span for indentation with a fixed width
			std::string htmlIndentation = "<span style=\"display:inline-block; width:" + std::to_string(indent * 20)
				+ "px;\"></span>";


			// Write the log entry in an HTML table row format.
			(*fileStream_) << "<tr class=\"log-entry\">"
						   << "<td class=\"timestamp\">" << timestamp << "</td>"
						   << "<td class=\"pid-tid\">[" << pid << "/" << tid << "]</td>"
						   << "<td class=\"level " << level << "\">" << level << "</td>"
						   << "<td class=\"message " << level << "\">" << htmlIndentation << message_ << "</td>"
						   << "</tr>\n";
		}
		else
		{
			// Write the log entry in the default LOG format.
			(*fileStream_) << "[" << timestamp << "][ID:" << pid << "/" << tid << "][" << level << "] "
						   << indentation << message
						   << std::endl;
		}
	}
}

bool Logify::FileStream::isFileIntact()
{
	std::string filePath   = generateFilePath();
	bool        fileExists = std::filesystem::exists(filePath);

	// If the file does not exist, it is considered intact.
	if (!fileExists) return true;

	// If the file is HTML, validate the ending tags.
	if (extension_ == FileExtension::HTML)
	{
		// If the ending tags are not correct, the file is not intact.
		if (!validateEndTags(filePath, HTML_ending)) return false;

		// Calculate the new size after removing the incorrect tags.
		std::streamoff newSize = std::filesystem::file_size(filePath) - HTML_ending.length();

		// Truncate the file to remove the invalid end.
		truncateFileEnd(filePath, newSize);

		// The file is now considered intact and ready for writing.
	}

	return true;
}

void Logify::FileStream::openFile()
{
	// Attempt to ensure the file is intact up to 5 times.
	for (int i = 0; i < 5; ++i)
	{
		if (isFileIntact()) break;
		fileIndex_++;
	}

	// Generate the file path using the current file index.
	std::string filePath   = generateFilePath();
	bool        fileExists = std::filesystem::exists(filePath);

	// Open the file stream for appending.
	fileStream_ = std::make_unique<std::ofstream>(filePath, std::ios::out | std::ios::app);
	if (!fileStream_->is_open())
	{
		throw std::runtime_error("Failed to open log file: " + filePath);
	}

	// If the file is HTML and new, write the initial HTML structure and styles.
	if (extension_ == FileExtension::HTML && !fileExists)
	{
		(*fileStream_) << "<!DOCTYPE html><html><head><style>"
					   << "body { background-color: " << colorScheme_.background << "; color: "
					   << colorScheme_.defaultColor << "; font-family: Arial, sans-serif; }"
					   << "table { width: 100%; border-collapse: collapse; }"
					   << "th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }"
					   << "th.timestamp, td.timestamp { width: fit-content; white-space: nowrap; }"
					   << "th.pid-tid, td.pid-tid { width: fit-content; white-space: nowrap; }"
					   << "th.level, td.level { width: fit-content; white-space: nowrap; }"
					   << "th.message, td.message { width: 90%; word-wrap: break-word; }"
					   << ".timestamp { font-weight: bold; color: " << colorScheme_.timestampColor << "; }"
					   << ".pid-tid { color: " << colorScheme_.pidTidColor << "; }"
					   << ".level.DEBUG { color: " << colorScheme_.debugColor << "; }"
					   << ".level.INFO { color: " << colorScheme_.infoColor << "; }"
					   << ".level.WARN { color: " << colorScheme_.warnColor << "; }"
					   << ".level.ERROR { color: " << colorScheme_.errorColor << "; }"
					   << ".level.FATAL { color: " << colorScheme_.errorColor << "; }"
					   << ".message { color: " << colorScheme_.defaultColor << "; }"
					   << ".message.FATAL { color: " << colorScheme_.errorColor << "; }"
					   << ".message.ERROR { color: " << colorScheme_.errorColor << "; }"
					   << ".message.WARN { color: " << colorScheme_.warnColor << "; }"
					   << "</style></head><body><h2>Logify Logs</h2><table>\n"
					   << "<tr><th class=\"timestamp\">Timestamp</th><th class=\"pid-tid\">PID/TID</th><th class=\"level\">Level</th><th class=\"message\">Message</th></tr>\n";
	}
}

// Generates the file path using the base name, file index, and extension.
std::string Logify::FileStream::generateFilePath() const
{
	std::ostringstream oss;
	oss << logFileName_ << "_" << std::setw(4) << std::setfill('0') << fileIndex_ << "." << extensionName_;
	return oss.str();
}

// Determines if the file needs to be rotated based on its size.
bool Logify::FileStream::shouldRotate() const
{
	std::string filePath = generateFilePath();
	return std::filesystem::exists(filePath) && std::filesystem::file_size(filePath) >= maxFileSize_;
}

// Rotates the log file by closing the current file and opening a new one.
void Logify::FileStream::rotateFile()
{
	if (fileStream_ && fileStream_->is_open())
	{
		// Close the HTML tags if the file is in HTML format.
		if (extension_ == FileExtension::HTML) (*fileStream_) << HTML_ending;

		// Close the current file stream.
		fileStream_->close();
	}

	// Increment the file index for the new file.
	++fileIndex_;
	openFile();
}

std::string Logify::FileStream::extractExtension(const std::string& filename)
{
	size_t dotPos = filename.find_last_of('.');

	// Return the extension after the last dot.
	if (dotPos != std::string::npos) return filename.substr(dotPos + 1);

	// Default to "log" if no extension is found.
	return "log";
}

Logify::FileExtension Logify::FileStream::determineExtensionType(const std::string& extension)
{
	// Return HTML type for "html" or "htm" extensions.
	if (extension == "html" || extension == "htm") return FileExtension::HTML;

	// Default to LOG type.
	return FileExtension::LOG;
}

bool Logify::FileStream::validateEndTags(const std::string& filePath, const std::string& expectedEnd)
{
	/*
	 * Validates that the file ends with the expected HTML ending tags.
	 */

	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if (!file.is_open()) return false;

	auto size = static_cast<long long>(expectedEnd.size());

	// Seek to the position of the expected end tags.
	file.seekg(-size, std::ios::end);
	std::string endTags(size, '\0');

	// Read the end of the file.
	file.read(&endTags[0], size);
	file.close();

	// Return true if the tags match.
	return endTags == expectedEnd;
}

void Logify::FileStream::truncateFileEnd(const std::string& filePath, std::streamoff truncatePosition)
{
	std::ofstream outFile(filePath, std::ios::in | std::ios::out | std::ios::binary);
	if (!outFile.is_open()) return;

	// Seek to the truncate position.
	outFile.seekp(truncatePosition);
	outFile.close();

	// Resize the file to the new size.
	std::filesystem::resize_file(filePath, truncatePosition);
}

std::string Logify::FileStream::replace(
	const std::string& text_,
	const std::string& to_replace,
	const std::string& replace_with
)
{
	std::string text = text_;

	// Find the first occurrence of the substring to replace.
	size_t pos = text.find(to_replace);

	// Loop until no more occurrences are found.
	while (pos != std::string::npos)
	{
		// Replace the substring with the desired string
		text.replace(pos, to_replace.length(), replace_with);

		// Find the next occurrence of the substring
		pos = text.find(to_replace, pos + replace_with.length());
	}

	return text;
}
