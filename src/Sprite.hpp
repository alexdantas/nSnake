#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED

#include <string>

/// A visual representation of stuff.
class Sprite
{
public:
    Sprite(std::string sprite, int width, int height);

    void draw(int x, int y);

private:
    std::string sprite;
    int width;
    int height;
};

#endif //SPRITE_H_DEFINED

