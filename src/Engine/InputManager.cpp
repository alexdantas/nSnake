#include <Engine/InputManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>

#include <ncurses.h>

int InputManager::pressedKey = ERR; // Starting with blank value
std::map<std::string, int> InputManager::binds;

void InputManager::bind(std::string name, int key)
{
	if (name.empty() || key == ERR)
		return;

	InputManager::binds[name] = key;
}

void InputManager::unbind(std::string name)
{
	InputManager::binds.erase(name);
}

int InputManager::getBind(std::string name)
{
	// If #key is not binded to anything...
	if (InputManager::binds.find(name) == InputManager::binds.end())
		return ERR;

	return (InputManager::binds[name]);
}
void InputManager::update(int delay_ms)
{
	InputManager::pressedKey = Ncurses::getInput(delay_ms);
}

bool InputManager::noKeyPressed()
{
	return (InputManager::pressedKey == ERR);
}

bool InputManager::isPressed(int key)
{
	return (InputManager::pressedKey == key);
}

bool InputManager::isPressed(std::string key)
{
	// If #key is not binded to anything, will return false
	if (InputManager::binds.find(key) == InputManager::binds.end())
		return false;

	return (InputManager::isPressed(InputManager::binds[key]));
}

std::string InputManager::keyToString(int value)
{
	// Is character inside the ASCII table?
	if (value >= 0 && value <= 127)
	{
		if (value == ' ')
			return "space";

		// The "printable" part of the ASCII table - easy
		if (value > ' ' && value <=  '~')
		{
			// Converting (int -> char -> char* -> std::string)
			char c[2] = { (char)value, '\0' };

			return std::string(c);
		}

		// Non-printable, then...
		// Let's get some names
		switch (value)
		{
		case 0:   return "null";
		case 27:  return "escape";
		case 127: return "delete";
		}
	}

	// If not, then this character is a special Ncurses value.
	// Those things were directy taken from <ncurses.h>
	//
	// NOTE: Wont use KEY_BREAK, KEY_SRESET, KEY_RESET, KEY_F0
	//       and KEY_EIC beucase they're strange..
	// NOTE: Also not using KEY_MOUSE, KEY_RESIZE and KEY_EVENT
	//       because they're Ncurses' flags for other things
	//       than the keyboard.
	//
	switch (value)
	{
		// Special case - value for "no key pressed"
	case ERR: return "undefined";

	case KEY_DOWN:      return "down";
	case KEY_UP:        return "up";
	case KEY_LEFT:      return "left";
	case KEY_RIGHT:     return "right";
	case KEY_HOME:      return "home";
	case KEY_BACKSPACE: return "backspace";
	case KEY_F(1):      return "f1";
	case KEY_F(2):      return "f2";
	case KEY_F(3):      return "f3";
	case KEY_F(4):      return "f4";
	case KEY_F(5):      return "f5";
	case KEY_F(6):      return "f6";
	case KEY_F(7):      return "f7";
	case KEY_F(8):      return "f8";
	case KEY_F(9):      return "f9";
	case KEY_F(10):     return "f10";
	case KEY_F(11):     return "f11";
	case KEY_F(12):     return "f12";
	case KEY_DL:        return "delete-line";
	case KEY_IL:        return "insert-line";
	case KEY_DC:        return "delete-char";
	case KEY_IC:        return "insert";
	case KEY_CLEAR:     return "clear";
	case KEY_EOS:       return "clear-to-end-of-screen";
	case KEY_EOL:       return "clear-to-end-of-line";
	case KEY_SF:        return "scroll-forward";
	case KEY_SR:        return "scroll-backward";
	case KEY_NPAGE:     return "page-down";
	case KEY_PPAGE:     return "page-up";
	case KEY_STAB:      return "set-tab";
	case KEY_CTAB:      return "clear-tab";
	case KEY_CATAB:     return "clear-all-tabs";
	case KEY_ENTER:     return "enter";
	case KEY_PRINT:     return "print";
	case KEY_LL:        return "home-down"; // wtf?
	case KEY_A1:        return "keypad-upper-left";
	case KEY_A3:        return "keypad-upper-right";
	case KEY_B2:        return "keypad-center";
	case KEY_C1:        return "keypad-lower-left";
	case KEY_C3:        return "keypad-lower-right";
	case KEY_BTAB:      return "back-tab";
	case KEY_BEG:       return "begin";
	case KEY_CANCEL:    return "cancel";
	case KEY_CLOSE:     return "close";
	case KEY_COMMAND:   return "command"; // not mac/osx's
	case KEY_COPY:      return "copy";
	case KEY_CREATE:    return "create";
	case KEY_END:       return "end";
	case KEY_EXIT:      return "exit";
	case KEY_FIND:      return "find";
	case KEY_HELP:      return "help";
	case KEY_MARK:      return "mark";
	case KEY_MESSAGE:   return "message";
	case KEY_MOVE:      return "move";
	case KEY_NEXT:      return "next";
	case KEY_OPEN:      return "open";
	case KEY_OPTIONS:   return "options";
	case KEY_PREVIOUS:  return "previous";
	case KEY_REDO:      return "redo";
	case KEY_REFERENCE: return "reference";
	case KEY_REFRESH:   return "refresh";
	case KEY_REPLACE:   return "replace";
	case KEY_RESTART:   return "restart";
	case KEY_RESUME:    return "resume";
	case KEY_SAVE:      return "save";
	case KEY_SBEG:      return "shift-begin";
	case KEY_SCANCEL:   return "shift-cancel";
	case KEY_SCOMMAND:  return "shift-command";
	case KEY_SCOPY:     return "shift-copy";
	case KEY_SCREATE:   return "shift-create";
	case KEY_SDC:       return "shift-delete-char";
	case KEY_SDL:       return "shift-delete-line";
	case KEY_SELECT:    return "select";
	case KEY_SEND:      return "shift-end";
	case KEY_SEOL:      return "shift-clear-to-end-of-line";
	case KEY_SEXIT:     return "shift-exit";
	case KEY_SFIND:     return "shift-find";
	case KEY_SHELP:     return "shift-help";
	case KEY_SHOME:     return "shift-home";
	case KEY_SIC:       return "shift-insert";
	case KEY_SLEFT:     return "shift-left";
	case KEY_SMESSAGE:  return "shift-message";
	case KEY_SMOVE:     return "shift-move";
	case KEY_SNEXT:     return "shift-next";
	case KEY_SOPTIONS:  return "shift-options";
	case KEY_SPREVIOUS: return "shift-previous";
	case KEY_SPRINT:    return "shift-print";
	case KEY_SREDO:     return "shift-redo";
	case KEY_SREPLACE:  return "shift-replace";
	case KEY_SRIGHT:    return "shift-right";
	case KEY_SRSUME:    return "shift-resume";
	case KEY_SSAVE:     return "shift-save";
	case KEY_SSUSPEND:  return "shift-suspend";
	case KEY_SUNDO:     return "shift-undo";
	case KEY_SUSPEND:   return "suspend";
	case KEY_UNDO:      return "undo";

	default: break;
	}
	return "undefined";
}

int InputManager::stringToKey(std::string string)
{
	if (string == "space")
		return ' ';

	// Let's hope it's a single char from the ASCII table
	if (string.size() == 1)
	{
		char c = string.c_str()[0];

		if (c > ' ' && c <= '~')
			return c;

		// undefined, sorry :(
		return ERR;
	}

	// Special case, unknown key
	if (string == "undefined") return ERR;

	if (string == "down")                       return KEY_DOWN;
	if (string == "up")                         return KEY_UP;
	if (string == "left")                       return KEY_LEFT;
	if (string == "right")                      return KEY_RIGHT;
	if (string == "home")                       return KEY_HOME;
	if (string == "backspace")                  return KEY_BACKSPACE;
	if (string == "f1")                         return KEY_F(1);
	if (string == "f2")                         return KEY_F(2);
	if (string == "f3")                         return KEY_F(3);
	if (string == "f4")                         return KEY_F(4);
	if (string == "f5")                         return KEY_F(5);
	if (string == "f6")                         return KEY_F(6);
	if (string == "f7")                         return KEY_F(7);
	if (string == "f8")                         return KEY_F(8);
	if (string == "f9")                         return KEY_F(9);
	if (string == "f10")                        return KEY_F(10);
	if (string == "f11")                        return KEY_F(11);
	if (string == "f12")                        return KEY_F(12);
	if (string == "delete-line")                return KEY_DL;
	if (string == "insert-line")                return KEY_IL;
	if (string == "delete-char")                return KEY_DC;
	if (string == "insert")                     return KEY_IC;
	if (string == "clear")                      return KEY_CLEAR;
	if (string == "clear-to-end-of-screen")     return KEY_EOS;
	if (string == "clear-to-end-of-line")       return KEY_EOL;
	if (string == "scroll-forward")             return KEY_SF;
	if (string == "scroll-backward")            return KEY_SR;
	if (string == "page-down")                  return KEY_NPAGE;
	if (string == "page-up")                    return KEY_PPAGE;
	if (string == "set-tab")                    return KEY_STAB;
	if (string == "clear-tab")                  return KEY_CTAB;
	if (string == "clear-all-tabs")             return KEY_CATAB;
	if (string == "enter")                      return KEY_ENTER;
	if (string == "print")                      return KEY_PRINT;
	if (string == "home-down")                  return KEY_LL;
	if (string == "keypad-upper-left")          return KEY_A1;
	if (string == "keypad-upper-right")         return KEY_A3;
	if (string == "keypad-center")              return KEY_B2;
	if (string == "keypad-lower-left")          return KEY_C1;
	if (string == "keypad-lower-right")         return KEY_C3;
	if (string == "back-tab")                   return KEY_BTAB;
	if (string == "begin")                      return KEY_BEG;
	if (string == "cancel")                     return KEY_CANCEL;
	if (string == "close")                      return KEY_CLOSE;
	if (string == "command")                    return KEY_COMMAND;
	if (string == "copy")                       return KEY_COPY;
	if (string == "create")                     return KEY_CREATE;
	if (string == "end")                        return KEY_END;
	if (string == "exit")                       return KEY_EXIT;
	if (string == "find")                       return KEY_FIND;
	if (string == "help")                       return KEY_HELP;
	if (string == "mark")                       return KEY_MARK;
	if (string == "message")                    return KEY_MESSAGE;
	if (string == "move")                       return KEY_MOVE;
	if (string == "next")                       return KEY_NEXT;
	if (string == "open")                       return KEY_OPEN;
	if (string == "options")                    return KEY_OPTIONS;
	if (string == "previous")                   return KEY_PREVIOUS;
	if (string == "redo")                       return KEY_REDO;
	if (string == "reference")                  return KEY_REFERENCE;
	if (string == "refresh")                    return KEY_REFRESH;
	if (string == "replace")                    return KEY_REPLACE;
	if (string == "restart")                    return KEY_RESTART;
	if (string == "resume")                     return KEY_RESUME;
	if (string == "save")                       return KEY_SAVE;
	if (string == "shift-begin")                return KEY_SBEG;
	if (string == "shift-cancel")               return KEY_SCANCEL;
	if (string == "shift-command")              return KEY_SCOMMAND;
	if (string == "shift-copy")                 return KEY_SCOPY;
	if (string == "shift-create")               return KEY_SCREATE;
	if (string == "shift-delete-char")          return KEY_SDC;
	if (string == "shift-delete-line")          return KEY_SDL;
	if (string == "select")                     return KEY_SELECT;
	if (string == "shift-end")                  return KEY_SEND;
	if (string == "shift-clear-to-end-of-line") return KEY_SEOL;
	if (string == "shift-exit")                 return KEY_SEXIT;
	if (string == "shift-find")                 return KEY_SFIND;
	if (string == "shift-help")                 return KEY_SHELP;
	if (string == "shift-home")                 return KEY_SHOME;
	if (string == "shift-insert")               return KEY_SIC;
	if (string == "shift-left")                 return KEY_SLEFT;
	if (string == "shift-message")              return KEY_SMESSAGE;
	if (string == "shift-move")                 return KEY_SMOVE;
	if (string == "shift-next")                 return KEY_SNEXT;
	if (string == "shift-options")              return KEY_SOPTIONS;
	if (string == "shift-previous")             return KEY_SPREVIOUS;
	if (string == "shift-print")                return KEY_SPRINT;
	if (string == "shift-redo")                 return KEY_SREDO;
	if (string == "shift-replace")              return KEY_SREPLACE;
	if (string == "shift-right")                return KEY_SRIGHT;
	if (string == "shift-resume")               return KEY_SRSUME;
	if (string == "shift-save")                 return KEY_SSAVE;
	if (string == "shift-suspend")              return KEY_SSUSPEND;
	if (string == "shift-undo")                 return KEY_SUNDO;
	if (string == "suspend")                    return KEY_SUSPEND;
	if (string == "undo")                       return KEY_UNDO;

	// Undefined key :(
	return ERR;
}

