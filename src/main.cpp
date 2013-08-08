
#include "Ncurses.hpp"
#include "Log.hpp"

int main(int argc, char* argv[])
{
    // Shut up, compiler!
    UNUSED(argc);
    UNUSED(argv);

    Log::logToFiles();
    Log::debugMode(true);

    Ncurses::init(80, 20);
    Log::debug("Started Ncurses");

    Ncurses::print("this is awesome", 20, 10);
    Ncurses::refresh();

    Ncurses::delay_us(1000000);
    Log::debug("Delay sent");

    Ncurses::exit();

    Log::logToFiles(false);
    Log::log("test");

    return 0;
}

