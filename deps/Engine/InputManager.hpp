#ifndef INPUTMANAGER_H_DEFINED
#define INPUTMANAGER_H_DEFINED

#include <string>
#include <map>

/// Handles input all over the game.
///
/// It's a collection of global functions to see
/// if some keys were pressed or not.
///
/// @note Make sure to call `InputManager::update()`
///       every frame!
///
/// ---
///
/// Bonus for being able to bind keys.
/// See for yourself:
///
///     // While initializing your game...
///     InputManager::bind("jump", 'w');
///     InputManager::bind("jump", KEY_UP);
///     // And then...
///     if (InputManager::isPressed("jump"))
///         // Do your magic...
///
/// ---
///
/// You can work with ASCII characters or ncurses
/// keys.
/// For a list of valid ncurses keys, check out
/// the `InputManager::keyToString()` function on
/// `InputManager.cpp` file.
///
namespace InputManager
{
	/// Attaches #name to #key.
	void bind(std::string name, int key);

	/// Removes all keybindings for #name.
	void unbind(std::string name);

	/// Returns the key that's bound to #name.
	int getBind(std::string name);

	/// Tells if no key was actually pressed on the last frame.
	bool noKeyPressed();

	/// Tells if #key was pressed.
	/// @note It's the Ncurses' internal value.
	bool isPressed(int key);

	/// Tells if #key was pressed.
	/// @note It's the user-defined key binding.
	bool isPressed(std::string key);

	/// Gets the input.
	///
	/// * If negative, will wait forever for an input.
	/// * If 0, will return immediately, wether a key
	///   was pressed or not.
	/// * If positive, will wait for #delay_ms milliseconds.
	///
	void update(int delay_ms=0);

	/// Returns human-readable name for internal value #key.
	std::string keyToString(int key);

	/// Returns the internal value for nameable #string key.
	int stringToKey(std::string string);

	/// The key that was pressed on the last frame.
	///
	/// It's an Ncurses internal value, being the ASCII
	/// value or some special others - like KEY_LEFT,
	/// KEY_RIGHT and such.
	///
	/// Avoid using it, prefer prefer #isPressed().
	extern int pressedKey;

	/// Contains all binds from names to internal Ncurses values.
	extern std::map<std::string, int> binds;
};

#endif //INPUTMANAGER_H_DEFINED

