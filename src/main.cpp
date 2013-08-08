
#include "Ncurses.hpp"

int main(int argc, char* argv[])
{
    // Shut up, compiler!
    UNUSED(argc);
    UNUSED(argv);

    Ncurses::init(80, 20);

    Ncurses::exit();

    return 0;
}

