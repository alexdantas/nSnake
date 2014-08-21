#ifndef ARGUMENTS_H_DEFINED
#define ARGUMENTS_H_DEFINED

/// Command-line argument parser.
///
/// This is a thin interface between yetris and the
/// `commander` CLI argument parser.
/// Highly game-specific.
///
/// Homepage: https://github.com/clibs/commander
namespace Arguments
{
	void parse(int argc, char* argv[]);
};

#endif //ARGUMENTS_H_DEFINED

