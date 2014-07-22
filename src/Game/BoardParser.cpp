#include <Game/BoardParser.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

#include <fstream>
#include <vector>
#include <string>

Board* BoardParser::load(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (!(file.is_open()))
	    throw BoardParserException("Can't open file '" + filename + "'");

    // Tells what's the current line on the file
    // (independent of comments and empty lines)
    int line_count = 0;

    // This will get used to the end of the
    // function.
    std::string current_line = "";

    // Those store the level metadata
    std::string level_name    = "";
    std::string level_author  = "";
    std::string level_date    = "";
    std::string level_comment = "";

    // First we get the metadata (name, comment, etc),
    // then we focus on the level itself
    while (true)
    {
	    if (!std::getline(file, current_line))
	    {
		    // End-of-file...
		    // Something wrong happened
		    throw BoardParserException(
			    "Abrupt ending of file at line " +
			    Utils::String::toString(line_count)
			    );
	    }
	    line_count++;

	    // Ignoring comments and empty lines
	    if ((current_line[0] == COMMENT_CHAR) ||
	        (current_line.empty()))
		    continue;

	    // Look for identifiers and get everything after
	    // them.
	    // For example:
	    //
	    //     name=This is the name
	    //
	    // We find `name=` and get everything after it
	    // to the end of line.
	    // And so on for other identifiers.
	    //
	    // TODO make this thing more flexible
	    //      (like handling whitespace between
	    //      the identifier and '=')

	    // Tells where we find a substring
	    // If it doesn't exist, will be `std::string::npos`
	    std::size_t pos;

	    pos = current_line.find("name=");
        if (pos != std::string::npos)
        {
            level_name = current_line.substr(pos + 5, std::string::npos);
            continue;
        }

        pos = current_line.find("author=");
        if (pos != std::string::npos)
        {
            level_author = current_line.substr(pos + 7, std::string::npos);
            continue;
        }

        pos = current_line.find("date=");
        if (pos != std::string::npos)
        {
            level_date = current_line.substr(pos + 5, std::string::npos);
            continue;
        }

        pos = current_line.find("comment=");
        if (pos != std::string::npos)
        {
            level_comment = current_line.substr(pos + 8, std::string::npos);
            continue;
        }

        // Yay, let's start it!
        if (current_line == "start")
	        break;
    }

    int player_start_x = 1; // It's (1, 1) because if it somehow starts
    int player_start_y = 1; // at (0, 0) it will always end up in a wall
                            // and die right at the beginning

    // Finally, when we read the level we have
    // two states for each tile - "wall" or "not wall"
    std::vector<std::vector<bool> > rawBoard;

    while (true)
    {
	    if (!std::getline(file, current_line))
	    {
		    throw BoardParserException(
			    "Abrupt ending of file at line " +
			    Utils::String::toString(line_count)
			    );
	    }
	    line_count++;

	    // Ignoring comments and empty lines
	    if ((current_line[0] == COMMENT_CHAR) ||
	        (current_line.empty()))
		    continue;

        if (current_line == "end")
	        // Finally, we can finish it all!
	        break;

        std::vector<bool> rawBoardLine;

        // TODO Remove \n or \r\n from line
        //      (create Utils::String::chomp)

        // And now we go through each char on the line
        // checking if it's a wall, blank space or the
        // player's starting point.
        //
        for (size_t i = 0; i < current_line.size() - 2; i++)
        {
	        if (current_line[i] == SNAKE_CHAR)
	        {
		        player_start_x = i;
		        player_start_y = rawBoard.size();

		        // It IS an empty space, after all...
		        rawBoardLine.push_back(false);
	        }
	        else
		        rawBoardLine.push_back(current_line[i] == WALL_CHAR);
        }

        // Commit this line to the level
        rawBoard.push_back(rawBoardLine);
    }

    // I know it's counter-intuitive, but the width
    // and height is just like this
    int board_width  = rawBoard[0].size();
    int board_height = rawBoard.size();

    Board* board = new Board(board_width,
                             board_height,
                             ((Globals::Game::teleport) ?
                              Board::TELEPORT :
                              Board::SOLID));

    board->setBoard(rawBoard);

    board->setStartX(player_start_x);
    board->setStartY(player_start_y);

    if (! level_name.empty())    board->setMetadata("name",    level_name);
    if (! level_author.empty())  board->setMetadata("author",  level_author);
    if (! level_date.empty())    board->setMetadata("date",    level_date);
    if (! level_comment.empty()) board->setMetadata("comment", level_comment);

    return board;
}
std::vector<std::string> BoardParser::listLevels()
{
	std::vector<std::string> levels = Utils::File::ls("./levels");

	// We'll go through each file, removing from
	// the vector if it doesn't end with ".nsnake"
	//
	for (std::vector<std::string>::iterator it = levels.begin();
	     it != levels.end();
	     ++it)
	{
		if ((*it).find(".nsnake") == std::string::npos)
			levels.erase(it);
	}

	return levels;
}

