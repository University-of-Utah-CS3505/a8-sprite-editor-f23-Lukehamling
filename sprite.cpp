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
#include <stack>

using std::vector;
using std::swap;
using std::abs;
using std::stack;
using std::get;
using std::tuple;
using std::make_tuple;

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

    tuple<int, int> currentPixel;
    stack<tuple<int, int>> pixels;
    pixels.push(make_tuple(x, y));

    while(!pixels.empty())
    {
        currentPixel = pixels.top();
        pixels.pop();

        int currentX = get<0>(currentPixel);
        int currentY = get<1>(currentPixel);

        if(getColor(currentX, currentY) == fillColor)
            continue;

        if(getColor(currentX, currentY) != originalColor)
            continue;

        setColor(currentX, currentY, fillColor);

        if(currentX > 0)
            pixels.push(make_tuple(currentX - 1, currentY));

        if(currentX < width - 1)
            pixels.push(make_tuple(currentX + 1, currentY));

        if(currentY > 0)
            pixels.push(make_tuple(currentX, currentY - 1));

        if(currentY < height - 1)
            pixels.push(make_tuple(currentX, currentY + 1));
    }
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
    //TODO: Implement this method
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
