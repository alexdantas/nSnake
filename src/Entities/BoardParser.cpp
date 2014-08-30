#include <Entities/BoardParser.hpp>
#include <Config/Globals.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>

#include <fstream>
#include <vector>
#include <string>

// HACK This will be initialized at `Globals::init()`
std::string BoardParser::directory = "";

std::string BoardParser::extension = "nsnake";




Board* BoardParser::load(std::string name)
{
	std::string filename = (BoardParser::directory +
	                        name +
	                        "." +
	                        BoardParser::extension);

	return BoardParser::loadFile(filename);
}

Board* BoardParser::loadFile(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (!(file.is_open()))
	    throw BoardParserException("Can't open file '" + filename + "'");

    // Tells what's the current line on the file
    // (independent of comments and empty lines)
    int line_count = 0;

    // The whole file has two parts: metadata and level definition.
    //
    // Let's read the whole file, line by line, adding the respective
    // parts to each of these buffers.
    //
    // We'll handle them separately later.
    std::string metadata_buffer;
    std::string level_buffer;

    // This will get used to the end of the
    // function.
    std::string current_line = "";

    while (std::getline(file, current_line))
    {
	    ++line_count;

	    current_line = Utils::String::trim(current_line);

	    // We only care for the line that tells a level
	    // definition will start.
	    if (current_line != "start")
		    metadata_buffer += (current_line + '\n');

	    else
        {
	        // Yay, start of the level definition!
	        bool parsed_level = false;

	        while (std::getline(file, current_line))
	        {
		        ++line_count;

		        current_line = Utils::String::trim(current_line);

		        if (current_line == "end")
		        {
			        parsed_level = true;
			        break;
		        }

		        level_buffer += (current_line + '\n');
	        }

	        if (! parsed_level)
	        {
		        // End-of-file...
		        // Something wrong happened
		        throw BoardParserException(
			        "Abrupt ending of file while parsing level at line " +
			        Utils::String::toString(line_count)
			        );
	        }
	        // Finished parsing the level!
	        // Back to the metadata.
        }
    }

    // Now we'll analyze the level definition we just got from the file

    int player_start_x = 1; // It's (1, 1) because if it somehow starts
    int player_start_y = 1; // at (0, 0) it will always end up in a wall
                            // and die right at the beginning

    // Finally, when we read the level we have
    // two states for each tile - "wall" or "not wall"
    std::vector<std::vector<bool> > rawBoard;


    std::vector<std::string> level_lines = Utils::String::split(level_buffer, '\n');

    for (size_t i = 0; i < level_lines.size(); ++i)
	    level_lines[i] = Utils::String::trim(level_lines[i]);

    for (size_t j = 0; j < (level_lines.size()); j++)
    {
	    current_line = level_lines[j];

	    if (current_line.empty())
		    continue;

        std::vector<bool> rawBoardLine;

        // And now we go through each char on the line
        // checking if it's a wall, blank space or the
        // player's starting point.
        //
        for (size_t i = 0; i < current_line.size(); i++)
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

    // Remember that metadata up there?
    // Let's get every present metadata through an INI parser
    std::stringstream stream;
    stream << metadata_buffer;
    INI::Parser parser(stream);

    board->setMetadata("name",    parser["name"]);
    board->setMetadata("author",  parser["author"]);
    board->setMetadata("date",    parser["date"]);
    board->setMetadata("comment", parser["comment"]);

    return board;
}
std::vector<std::string> BoardParser::listLevels()
{
	std::vector<std::string> levels = Utils::File::ls(BoardParser::directory);

	// Remove files that doesn't end with the default file extension
	//
	// Also, don't store the full path, only it's basename
	// (like "file" and not "/path/to/file")
	//
	for (std::vector<std::string>::iterator it = levels.begin();
	     it != levels.end();
	     ++it)
	{
		if (Utils::File::extension(*it) == BoardParser::extension)
			(*it) = (Utils::File::dropExtension(Utils::File::basename((*it))));

		else
		{
			// When we remove an element of a vector
			// it points to the next element.
			it = levels.erase(it);

			// We need to decrement it because the `for`
			// will increment at the end
			--it;
		}
	}

	return levels;
}

