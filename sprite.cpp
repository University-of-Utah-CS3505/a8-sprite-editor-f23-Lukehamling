/*
    Author:     Ryan Dalrymple
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#include "sprite.h"
#include <QColor>
#include <QJsonObject>
#include <vector>
#include <math.h>
#include <cmath>

using std::vector;
using std::swap;
using std::abs;

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
        fillRecursive(x-1, y, originalColor, fillColor);

    if(x < width - 1)
        fillRecursive(x+1, y, originalColor, fillColor);

    if(y > 0)
        fillRecursive(x, y-1, originalColor, fillColor);

    if(y < height - 1)
        fillRecursive(x, y+1, originalColor, fillColor);
}

void Sprite::drawLine(unsigned short int startX, unsigned short int startY,
              unsigned short int endX, unsigned short int endY, QColor color) {
    // Bresenham's line algorithm. Low high logic
    int dx = endX - startX;
    int dy = endY - startY;
    if (abs(dy) < abs(dx)) {
        if (startX > endX) {
            drawLineLow(endX, endY, startX, startY, color);
        } else {
            drawLineLow( startX, startY, endX, endY, color);
        }
    } else {
        if (startY > endY) {
            drawLineHigh(endX, endY, startX, startY, color);
        } else {
            drawLineHigh( startX, startY, endX, endY, color);
        }
    }
}
void Sprite::drawLineLow(unsigned short int startX, unsigned short int startY,
                      unsigned short int endX, unsigned short int endY, QColor color) {
    int dx = endX - startX;
    int dy = endY - startY;
    int y;
    for (int x = startX; x < endX; x++) {
        y = startY + dy * (x - startX) / dx;
        grid[x][y] = color;
    }
}
void Sprite::drawLineHigh(unsigned short int startX, unsigned short int startY,
                      unsigned short int endX, unsigned short int endY, QColor color) {
    int dx = endX - startX;
    int dy = endY - startY;
    int x;
    for (int y = startY; y < endY; y++) {
        x = startX + dx * (y - startY) / dy;
        grid[x][y] = color;
    }
}

void Sprite::drawCircle(unsigned short int startX, unsigned short int startY,
                        unsigned short int endX, unsigned short int endY, QColor color)
{
    //Bresnem's Circle Algorithm
    float radius = sqrt((pow(startX - endX, 2)) + (pow(startY - endY, 2)));
    int xIncrement = 0;
    int yIncrement = radius;
    int decisionParamater = 3 - (2 * radius);
    placePixelsInCircle(startX, startY, xIncrement, yIncrement, color);

    while (yIncrement >= xIncrement)
    {
        xIncrement++;
        if(decisionParamater > 0)
        {
            yIncrement--;
            decisionParamater = decisionParamater + (4 *(xIncrement - yIncrement)) + 10;
        }
        else
        {
            decisionParamater = decisionParamater + (4 *xIncrement) + 6;
        }
        placePixelsInCircle(startX, startY, xIncrement, yIncrement, color);
    }
}

void Sprite::placePixelsInCircle(unsigned short int centerX, unsigned short int centerY, unsigned short int xIncrement, unsigned short int yIncrement, QColor color)
{
    if((centerX + xIncrement < width) && (centerY + yIncrement < height))
    {
        setColor(centerX + xIncrement, centerY + yIncrement, color);
    }
    if((centerX - xIncrement >= 0) && (centerY + yIncrement < height))
    {
        setColor(centerX - xIncrement, centerY + yIncrement, color);
    }
    if((centerX + xIncrement < width) && (centerY - yIncrement >= 0))
    {
        setColor(centerX + xIncrement, centerY - yIncrement, color);
    }
    if((centerX - xIncrement >= 0) && (centerY - yIncrement >= 0))
    {
        setColor(centerX - xIncrement, centerY - yIncrement, color);
    }
    if((centerX + yIncrement < width) && (centerY + xIncrement < height))
    {
        setColor(centerX + yIncrement, centerY + xIncrement, color);
    }
    if((centerX - yIncrement >= 0) && (centerY + xIncrement < height))
    {
        setColor(centerX - yIncrement, centerY + xIncrement, color);
    }
    if((centerX + yIncrement < width) && (centerY - xIncrement >= 0))
    {
        setColor(centerX + yIncrement, centerY - xIncrement, color);
    }
    if((centerX - yIncrement >= 0) && (centerY - xIncrement >= 0))
    {
        setColor(centerX - yIncrement, centerY - xIncrement, color);
    }
}

void Sprite::drawRectangle(unsigned short int startX, unsigned short int startY,
                unsigned short int endX, unsigned short int endY, QColor color)
{
//    qDebug() << "X:" << startX << "," << endX << "Y:" << startY << "," << endX;
    if (startX > endX) {
        std::swap(startX, endX);
    }
    if (startY > endY) {
        std::swap(startY, endY);
    }
    for (int x = startX; x <= endX; x++) {
        grid[x][startY] = color;
        grid[x][endY] = color;
    }
    for (int y = startY; y <= endY; y++) {
        grid[startX][y] = color;
        grid[endX][y] = color;
    }
}
