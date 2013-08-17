/// @file Shapes.hpp
//
//  This file defines a set of basic shapes that can be used
//  for... pretty much whatever

#ifndef SHAPES_H_DEFINED
#define SHAPES_H_DEFINED

struct Point
{
    int x;
    int y;

    Point();
    Point(int x, int y);
};

struct Rectangle
{
    int x;
    int y;
    int w;
    int h;
    Point center;

    Rectangle();
    Rectangle(int x, int y, int w, int h);
};

struct Circle
{
    int x;
    int y;
    int r;
    int centerX;
    int centerY;

    Circle(int x, int y, int r);
};

// struct Line
// struct Triangle

#endif /* SHAPES_H_DEFINED */

