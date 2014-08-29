#ifndef NCURSES_H_DEFINED
#define NCURSES_H_DEFINED

/// Low-level interactions with the terminal library Ncurses.
///
/// The whole point of this Engine is to abstract away
/// direct Ncurses use.
///
/// Most of ncurses graphic manipulation moved to the `Window`
/// class. Check there for a great ncurses function fest.
///
/// ## For developers
///
/// You shouldn't change this module at all.
/// All the other modules depend on it and they work alright.
///
namespace Ncurses
{
	/// Initializes Ncurses mode.
	///
	/// The whole Engine depends on this.
	bool init();

	/// Quits Ncurses mode.
	///
	/// Make sure to call it at the end of the game,
	/// otherwise the terminal will be left at a
	/// strange state.
	void exit();

	/// Returns a pressed character within a
	/// timespan of #delay_ms (milliseconds).
	///
	/// @note If you send -1, it'll block the execution,
	///       waiting for the input indefinitely.
	///
	/// @note It's #int because nCurses uses some other
	///       values that don't fit on a #char variable.
	int getInput(int delay_ms=-1);
}

#endif //NCURSES_H_DEFINED

