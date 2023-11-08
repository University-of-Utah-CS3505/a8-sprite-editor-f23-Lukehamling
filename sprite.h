/*
    Author:     Ryan Dalrymple
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <QColor>
#include <QJsonObject>
#include <vector>
//#include <array>
using std::vector;
//using std::array;

class Sprite
{
private:

    //TODO: declaring arrays like this is problematic for what we are trying to do
    //array<array<QColor, 32>, 32> grid;
    vector<vector<QColor>> grid;
    const QColor DEFAULT_COLOR = Qt::transparent;
public:
    unsigned short int width;
    unsigned short int height;
    /// @brief Constructor for Sprite object. The user will pass in a width and height for the spite that
    ///         will used to set the size of the editing grid in the view. The grid will be represented
    ///         in the Sprite object as a 2d vector of QColors. Indexing into the grid will work as expected
    ///         EX. pixelColor = grid[10][50] -> pixel at (10, 50).
    /// @param spriteWidth is the width of the Sprite set by the user.
    /// @param spriteHeight is the height of the Sprite set by the user.
    Sprite(unsigned short int spriteWidth, unsigned short int spriteHeight);

    /// @brief Creates a Sprite object from a previously saved Sprite JSON.
    /// @param loadedSprite is a Sprite that was created in the past and saved as a JSON file.
    Sprite(QJsonObject loadedSprite);

    /// @brief Retrieves a color from a certain pixel x,y location.
    /// @param x is the x location of the pixel in the Sprite
    /// @param y is the y location of the pixel in the Sprite
    QColor getColor(unsigned short int x, unsigned short int y);

    /// @brief Sets the color of a pixel on the Sprite object. Will be used in conjunction with drawing in the view/model.
    /// @param x is the x location of the starting point
    /// @param y is the y location of the starting point
    /// @param desiredColor is the color that the pixel will change to.
    void setColor(unsigned short int x, unsigned short int y, QColor desiredColor);

    /// @brief Fills all pixels in with fillColor starting at (x,y) and going until a pixel with a different color
    ///         than the starting point is detected.
    /// @param x is the x location of the starting point
    /// @param y is the y location of the starting point
    /// @param fillColor is the color that the appropriate pixels will change to.
    void fill(unsigned short int x, unsigned short int y, QColor fillColor);

    /// @brief Creates a circle in the grid so the user can use pre-defined shapes. The user will click a starting pixel on the
    ///         sprite editor and an ending pixel. The circle will be drawn with one edge being on start(x,y) and the other
    ///         being on end (x,y) with a center point in the middle. The diameter can be calculated from starting (x,y) and ending (x,y).
    /// @param startX is the starting x location that the user clicked
    /// @param startY is the starting y location that the user clicked
    /// @param endX is the ending x location that the user clicked
    /// @param endY is the ending y location that the user clicked
    void drawCircle(unsigned short int startX, unsigned short int startY,
                    unsigned short int endX, unsigned short int endY);

    /// @brief Creates a square in the grid so the user can use pre-defined shapes. The user will click a starting pixel on the
    ///         sprite editor and an ending pixel. The square will be drawn with one edge being on start(x,y) and the other
    ///         being on end (x,y).
    /// @param startX is the starting x location that the user clicked
    /// @param startY is the starting y location that the user clicked
    /// @param endX is the ending x location that the user clicked
    /// @param endY is the ending y location that the user clicked
    void drawSquare(unsigned short int startX, unsigned short int startY,
                    unsigned short int endX, unsigned short int endY);
};

#endif // SPRITE_H
