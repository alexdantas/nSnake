#ifndef DIALOG_H_DEFINED
#define DIALOG_H_DEFINED

#include <string>

///
namespace Dialog
{
	/// Spawns a Dialog box asking for a yes-or-no #question.
	///
	/// @note If the user presses 'q' to quit,
	///       it'll return false.
	///
	bool askBool(std::string question);
};

#endif //DIALOG_H_DEFINED

