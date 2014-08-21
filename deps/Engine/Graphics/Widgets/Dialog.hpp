#ifndef DIALOG_H_DEFINED
#define DIALOG_H_DEFINED

#include <string>

/// Quick-and-dirty functions to show GUI-like dialogs
/// on the screen.
///
namespace Dialog
{
	/// Shows a message on the screen.
	/// Spawns a screen-centered dialog that respects multi-line
	/// strings, wrapping it's size around them.
	///
	/// @note The multi-line delimiter is `\n`
	///
	/// @param message     Which string to show.
	/// @param pressAnyKey If it should wait for pressing a single key
	///
	void show(std::string message, bool pressAnyKey=false);

	/// Spawns a Dialog box asking for a yes-or-no #question.
	/// Dialog optionally has a #title and a #default_value.
	///
	/// @note If the user presses 'q' to quit,
	///       it'll return false.
	///
	bool askBool(std::string question, std::string title="", bool default_value=false);
};

#endif //DIALOG_H_DEFINED

