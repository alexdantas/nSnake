//
//

#include <iostream>
#include "StateManager.hpp"
#include "Ncurses.hpp"
#include "Log.hpp"

// Shuts up the compiler about unused parameters.
#define UNUSED(x) ((void)(x))

int main(int argc, char* argv[])
{
    // Shut up, compiler!
    UNUSED(argc);
    UNUSED(argv);

    try
    {
        StateManager manager(80, 24);

        manager.run();
    }
    // Several ways of quitting and logging the error
    catch (const char* e)
    {
        Ncurses::exit(true);
        Log::error("Exception: " + std::string(e));
        return -1;
    }
    catch (std::string e)
    {
        Ncurses::exit(true);
        Log::error("Exception: " + e);
        return -1;
    }
    catch (...)
    {
        Ncurses::exit(true);
        Log::error("Exception caught! Quitting...");
        return -1;
    }
    return 0;
}

