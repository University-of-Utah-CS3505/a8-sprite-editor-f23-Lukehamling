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
    //TODO: Implement this method
}

void Sprite::drawCircle(unsigned short int startX, unsigned short int startY,
                        unsigned short int endX, unsigned short int endY)
{
    //TODO: Implement this method
}

void Sprite::drawSquare(unsigned short int startX, unsigned short int startY,
                unsigned short int endX, unsigned short int endY)
{
    //TODO: Implement this method
}
