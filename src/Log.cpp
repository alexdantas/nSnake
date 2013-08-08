#include "Log.hpp"

bool Log::isDebugMode = false;
bool Log::usingFiles  = false;
bool Log::openedFiles = false;
std::ofstream Log::fileout;
std::ofstream Log::fileerr;

// Terminal escape codes.
// They're used to make special effects on terminals.
// Don't use this to print on files!
std::string bold_red     = "\e[1;31m";
std::string bold_cyan    = "\e[1;36m";

std::string bold_green   = "\e[1;32m";
std::string green        = "\e[0;32m";

std::string bold_yellow  = "\e[1;33m";
std::string reset_color  = "\e[0m";
std::string clear_screen = "[H[2J";

void Log::logToFiles(bool option, std::string filenameOut, std::string filenameErr)
{
    if (option) // Will open files
    {
        Log::fileout.open(filenameOut.c_str(), std::ios::out | std::ios::trunc);
        Log::fileerr.open(filenameErr.c_str(), std::ios::out | std::ios::trunc);
        Log::usingFiles = true;

        if (!(Log::fileout.is_open()) ||
            !(Log::fileerr.is_open()))
            Log::openedFiles = false;
        else
            Log::openedFiles = true;
    }
    else // Will close files
    {
        if (!(Log::openedFiles))
            return;

        Log::fileout.close();
        Log::fileerr.close();

        Log::usingFiles = false;
    }
}
void Log::error(std::string msg)
{
    if (usingFiles)
    {
        Log::fileerr << "Error:   " << msg <<  std::endl;
        Log::fileerr.flush();
    }
    else
        std::cerr << bold_red << "Error:   " << msg << reset_color << std::endl;
}
void Log::warning(std::string msg)
{
    if (usingFiles)
    {
        Log::fileerr << "Warning: " << msg << std::endl;
        Log::fileerr.flush();
    }
    else
        std::cerr << bold_yellow << "Warning: " << msg << reset_color << std::endl;
}
void Log::log(std::string msg)
{
    if (usingFiles)
    {
        Log::fileout << msg << std::endl;
        Log::fileout.flush();
    }
    else
        std::cout << bold_green << msg << reset_color << std::endl;
}
void Log::debug(std::string msg)
{
    if (isDebugMode)
        Log::log("Debug:   " + msg);
}
void Log::debugMode(bool option)
{
    if (option == false)
    {
        isDebugMode = false;
        return;
    }

    // Prints a nice logo
    isDebugMode = true;
    Log::debug("     #         #                                             #        ");
    Log::debug("  mmm#   mmm   #mmm   m   m   mmmm         mmmmm   mmm    mmm#   mmm  ");
    Log::debug(" #\" \"#  #\"  #  #\" \"#  #   #  #\" \"#         # # #  #\" \"#  #\" \"#  #\"  # ");
    Log::debug(" #   #  #\"\"\"\"  #   #  #   #  #   #         # # #  #   #  #   #  #\"\"\"\" ");
    Log::debug(" \"#m##  \"#mm\"  ##m#\"  \"mm\"#  \"#m\"#         # # #  \"#m#\"  \"#m##  \"#mm\" ");
    Log::debug("                              m  #                                    ");
    Log::debug("                               \"\"                                     ");
}
// void Log::useLogFiles(bool option)
// {
//     if (option)
//     {
//         isUsingFiles = true;

//         std::ofstream tmp;
//         tmp.open("stdout.txt");

//         std::streambuf* buf = tmp.rdbuf();


//         std::ostream tmp2(buf);
//         std::ostream tm
//         stdout = tmp2;

//         stderr = (tmp.open("stderr.txt"));
//     }
//     // else
//     // {
//     //     isUsingFiles = false;
//     //     stdout       = std::cout;
//     //     stderr       = std::cerr;
//     // }
// }

