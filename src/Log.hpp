#ifndef LOG_H_DEFINED
#define LOG_H_DEFINED

#include <iostream>
#include <fstream>
#include <string>

/// Logs information on standard output streams or files.
///
/// By default this class outputs all logging to standard
/// output (_stdout_) and standard error output (_stderr_).
/// You can change that with *logToFiles()*.
///
/// With this class you can log different levels of information.
/// Currently we have _Error_, _Warning_, _Debug_ and _normal_ logs.
/// Each get printed with it's prefix.
///
/// ## Usage
///     Log::error("Class foo Method bar");
///     Log::warning("You shouldn't do that!");
///
///     // To log debugging information, you must call this
///     Log::debugMode(true);
///     Log::debug("This will be shown");
///
///     Log::debugMode(false);
///     Log::debug("This will not be shown");
///
///     // If you want to log to files, do this:
///     Log::logToFiles();
///
///     // Or if you like customizing files...
///     Log::logToFiles(true, "OutFile.txt", "ErrFile.txt");
///
///     Log::error("Error to a file");
///
///     // And to disable it
///     Log::logToFiles(false);
///
///     Log::warning("Warning to stderr");
///
/// ## For developers
///
/// This is a standalone class, can be inserted on any project
/// regardless of library (assuming standard C++ I/O classes).
///
class Log
{
public:
    /// Chooses to log to files instead of *stdout*/*stderr*.
    static void logToFiles(bool option=true, std::string filenameOut="stdout.txt", std::string filenameErr="stderr.txt");

    /// Logs *msg* with prefix "Error: ".
    ///
    /// @note By default logs to _stderr_.
    static void error(std::string msg);

    /// Logs *msg* with prefix "Warning: ".
    ///
    /// @note By default logs to _stderr_.
    static void warning(std::string msg);

    /// Logs *msg*.
    ///
    /// @note By default logs to _stdout_.
    static void log(std::string msg);

    /// Logs *msg* with prefis "Debug: ".
    ///
    /// @note Only gets logged if debug mode is on.
    /// @note By default logs to _stdout_.
    static void debug(std::string msg);

    /// Turns debug on/off.
    /// This tells if all debug messages will be ignored.
    ///
    /// @note By default it is turned off.
    static void debugMode(bool option);

    // static void useLogFiles(bool option);

private:
    ///< Tells if we're on debug mode.
    static bool isDebugMode;

    /// Tells if we're using files to log.
    static bool usingFiles;

    /// Tells if we've successfully opened the log files.
    static bool openedFiles;

    static std::ofstream fileout; /// Output file for messages.
    static std::ofstream fileerr; /// Output file for errors.
};

#endif /* LOG_H_DEFINED */

