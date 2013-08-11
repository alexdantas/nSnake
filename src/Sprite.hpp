#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <string>

/// A visual representation of a thing.
///
class Sprite
{
public:
    Sprite(std::string sprite, int width, int height, unsigned long color);

    void draw(int x, int y);

private:
    std::string sprite;
    int width;
    int height;
    unsigned long color;
};

#endif //SPRITE_H_DEFINED

