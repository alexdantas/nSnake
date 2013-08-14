#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <string>

/// A visual representation of a thing.
///
/// Note that in ncurses, everything is characters.
/// So a sprite is a string, just in the way that it'll be shown
/// on the screen.
/// You can customize it's color.
///
/// Make sure the width and height are accurate, otherwise
/// unforeseen consequences may happen.
///
class Sprite
{
public:
    /// Creates a sprite with appearance of *sprite* and *color*.
    ///
    /// @note You must provide it's *width* and *height*; otherwise
    ///       they will be assumed as 1.
    Sprite(std::string sprite, unsigned long color, int width=1, int height=1);

    /// Shows sprite on *x* *y*.
    void render(int x, int y);

    /// Sets the current color of the sprite to *style*.
    void setStyle(unsigned long style);

private:
    std::string sprite;  /// The sprite's appearance onscreen.
    int width;           /// It's width
    int height;          /// It's height
    unsigned long color; /// It's color
};

#endif //SPRITE_H_DEFINED

