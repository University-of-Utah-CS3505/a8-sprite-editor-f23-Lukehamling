/*
    Author:     Ryan Dalrymple
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#include "sprite.h"
#include <QColor>
#include <QJsonObject>
#include <vector>

using std::vector;
using std::swap;

Sprite::Sprite(unsigned short int spriteWidth, unsigned short int spriteHeight) : width{spriteWidth}, height{spriteHeight}
{
    for(int i = 0; i < spriteWidth; i++)
    {
        vector<QColor> column;
        grid.push_back(column);

        for(int j = 0; j < spriteHeight; j++)
            grid[i].push_back(DEFAULT_COLOR);
    }
}

Sprite::Sprite(QJsonObject loadedSprite)
{
    //TODO: Implement this constructor after deserializing sprite
}

Sprite::Sprite(const Sprite& spriteToCopy)
{
    this->height    = spriteToCopy.height;
    this->width     = spriteToCopy.width;
    this->grid      = spriteToCopy.grid;
}

Sprite& Sprite::operator=(Sprite otherSprite)
{
    swap(this->height, otherSprite.height);
    swap(this->width, otherSprite.width);
    swap(this->grid, otherSprite.grid);

    return *this;
}

QColor Sprite::getColor(unsigned short int x, unsigned short int y)
{
    return grid[x][y];
}

void Sprite::setColor(unsigned short int x, unsigned short int y, QColor desiredColor)
{
    grid[x][y] = desiredColor;
}

void Sprite::fill(unsigned short int x, unsigned short int y, QColor fillColor)
{
    QColor originalColor = getColor(x,y);
    fillRecursive(x,y, originalColor, fillColor);
}

void Sprite::fillRecursive(unsigned short int x, unsigned short int y, QColor originalColor, QColor fillColor)
{
    if(getColor(x,y) == fillColor)
        return;

    if(getColor(x,y) != originalColor)
        return;

    setColor(x, y, fillColor);

    if(x > 0)
        fill(x-1, y, fillColor);

    if(x < width)
        fill(x+1, y, fillColor);

    if(y > 0)
        fill(x, y-1, fillColor);

    if(y < height)
        fill(x, y+1, fillColor);
}

void Sprite::drawCircle(unsigned short int startX, unsigned short int startY,
                        unsigned short int endX, unsigned short int endY)
{
    //TODO: Implement this method
}

void Sprite::drawRectangle(unsigned short int startX, unsigned short int startY,
                unsigned short int endX, unsigned short int endY)
{
    //TODO: Implement this method
}
