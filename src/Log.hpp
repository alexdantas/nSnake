#ifndef LOG_H_DEFINED
#define LOG_H_DEFINED

#include <iostream>
#include <fstream>
#include <string>

///TODO make method to set output to files, useful for ncurses
///     (like stderr.txt)

/// Logs information on standard output streams or files.
///
/// By default this class outputs all logging to standard
/// output (_stdout_) and standard error output (_stderr_).
/// You can change that with *setOutput()*.
///
/// With this class you can log different levels of information.
/// Currently we have _Error_, _Warning_, _Debug_ and _normal_ logs.
/// Each get printed with it's prefix.
///
/// ## Usage
///     // This is a "global" static class.
///     Log::error("Class foo Method bar");
///     Log::warning("You shouldn't do that!");
///
///     Log::debugMode(true);
///
/// ## For developers
/// This is a standalone class, can be inserted on any project
/// regardless of library (assuming standard C++ I/O classes).
///
class Log
{
public:
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
    static bool isDebugMode;     ///< Tells if we're on debug mode.
    // static std::ostream& stdout; ///< Standard output stream
    // static std::ostream& stderr; ///< Standard error stream
    // static bool isUsingFiles;
};

#endif /* LOG_H_DEFINED */
