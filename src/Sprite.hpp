#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <string>

/// A visual representation of a thing.
///
class Sprite
{
public:
    ///
    Sprite(std::string sprite, int width, int height, unsigned long color);

    /// Shows sprite at *x* *y*.
    void render(int x, int y);

    void setStyle(unsigned long style);

private:
    std::string sprite;
    int width;
    int height;
    unsigned long color;
};

#endif //SPRITE_H_DEFINED

