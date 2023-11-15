/*
    Team:       Coders for Christ
    Authors:    Ryan Dalrymple, Vincentio Dane, Luke Hamling, August O'Rourke
    Class:      CS3505
    Assignment: 8 - Sprite Editor
    Reviewed by Luke Hamling
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <QColor>
#include <QJsonObject>
#include <vector>
#include <math.h>
#include <cmath>
#include <stack>

using std::vector;

class Sprite
{
private:

    vector<vector<QColor>> grid;
    const QColor DEFAULT_COLOR = Qt::transparent;

public:
    ///@brief Constructor for Sprite object. The user will pass in a width and height for the spite that
    ///       will used to set the size of the editing grid in the view. The grid will be represented
    ///       in the Sprite object as a 2d vector of QColors. Indexing into the grid will work as expected
    ///       EX. pixelColor = grid[10][50] -> pixel at (10, 50).
    ///@param spriteWidth is the width of the Sprite set by the user.
    ///@param spriteHeight is the height of the Sprite set by the user.
    Sprite(unsigned short int spriteWidth, unsigned short int spriteHeight);

    ///@brief Creates a Sprite object from a previously saved Sprite JSON.
    ///@param loadedSprite is a Sprite that was created in the past and saved as a JSON file.
    Sprite(QJsonObject loadedSprite);

    ///@brief Copy constructor for Sprite class. Performs deep copy of data members.
    ///@param spriteToCopy is the Sprite whose values will be copied into this Sprite.
    Sprite(const Sprite& spriteToCopy);

    ///@brief Assignment overload. This will allow us to assign one sprite to another.
    ///@param otherSprite - the sprite being assigned to the new variable
    Sprite& operator=(Sprite otherSprite);

    ///@brief Retrieves a color from a certain pixel x,y location.
    ///@param x - the x location of the pixel in the Sprite
    ///@param y - the y location of the pixel in the Sprite
    ///@return the color of the pixel at (x,y)
    QColor getColor(unsigned short int x, unsigned short int y);

    ///@brief Sets the color of a pixel on the Sprite object. Will be used in conjunction with drawing in the view/model.
    ///@param x - the x location of the starting point
    ///@param y - the y location of the starting point
    ///@param desiredColor is the color that the pixel will change to.
    void setColor(unsigned short int x, unsigned short int y, QColor desiredColor);

    ///@brief Fills all pixels in with fillColor starting at (x,y) and going until a pixel with a different color
    ///       than the starting point is detected.
    ///@param x - the x location of the starting point
    ///@param y - the y location of the starting point
    ///@param fillColor is the color that the appropriate pixels will change to.
    void fill(unsigned short int x, unsigned short int y, QColor fillColor);

    ///@brief Creates a line on the sprite. Usefull to fill in gaps.
    ///@param startX - the x part of the starting coordinate
    ///@param startY - the y part of the starting coordinate
    ///@param endX - the x part of the ending coordinate
    ///@param endy - the y part of the ending coordinate
    ///@param color - the color we drawing with
    void drawLine(unsigned short int startX, unsigned short int startY,
                  unsigned short int endX, unsigned short int endY, QColor color);

    ///@brief Creates a circle in the grid so the user can use pre-defined shapes. The user will click a starting pixel on the
    ///       sprite editor and an ending pixel. The circle will be drawn with one edge being on start(x,y) and the other
    ///       being on end (x,y) with a center point in the middle. The diameter can be calculated from starting (x,y) and ending (x,y).
    ///@param startX - the starting x location that the user clicked
    ///@param startY - the starting y location that the user clicked
    ///@param endX - the ending x location that the user clicked
    ///@param endY - the ending y location that the user clicked
    void drawCircle(unsigned short int startX, unsigned short int startY,
                    unsigned short int endX, unsigned short int endY, QColor color);

    ///@brief Creates a square in the grid so the user can use pre-defined shapes. The user will click a starting pixel on the
    ///       sprite editor and an ending pixel. The square will be drawn with one edge being on start(x,y) and the other
    ///       being on end (x,y).
    ///@param startX - the starting x location that the user clicked
    ///@param startY - the starting y location that the user clicked
    ///@param endX - the ending x location that the user clicked
    ///@param endY - the ending y location that the user clicked
    void drawRectangle(unsigned short int startX, unsigned short int startY,
                       unsigned short int endX, unsigned short int endY, QColor color);

    ///@brief Getter for the Sprite width.
    ///@return Width of Sprite
    unsigned short int getWidth();

    ///@brief Getter for the Sprite height.
    ///@return Height of Sprite
    unsigned short int getHeight();
private:
    unsigned short int width;
    unsigned short int height;

    ///@brief This is a small helper method that lets us place pixels while doing Bresenham's circle algoritm
    ///@param centerX - the x of the centerpoint of the circle
    ///@param centerY - the y of the centerpoint of the circle
    ///@param xIncrement - the increment for X
    ///@param yIncrement - the increment for Y
    ///@param color - the color we are drawing for the circle
    void placePixelsInCircle(unsigned short int centerX, unsigned short int centerY,
                             unsigned short int xIncrement, unsigned short int yIncrement, QColor color);


    ///@brief draws a line left to right with a horizontal slope.
    void drawLineLow(unsigned short int startX, unsigned short int startY,
                     unsigned short int endX, unsigned short int endY, QColor color);
    ///@brief draws a line bottom to top with a vertical slope.
    void drawLineHigh(unsigned short int startX, unsigned short int startY,
                      unsigned short int endX, unsigned short int endY, QColor color);
};

#endif // SPRITE_H
