#include "pixelEditorModel.h"
#include "sprite.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVector>
#include <iostream>
#include <QFileDialog>

pixelEditorModel::pixelEditorModel(QObject *parent)
{
    setParent(parent);
    QColor currentColor = QColor(0, 0, 0);
}

void pixelEditorModel::redo()
{
    qDebug() << "redo clicked";
    // TODO
}

void pixelEditorModel::undo()
{
    qDebug() << "undo clicked";
    // TODO
}

void pixelEditorModel::addToRedo()
{

}

void pixelEditorModel::addToUndo()
{

}

void pixelEditorModel::changePixel()
{
    //TODO
}

void pixelEditorModel::selectColor(QColor color)
{
    //TODO
}

void pixelEditorModel::writeJSON()
{
    spriteJSON.insert("height", height);
    spriteJSON.insert("width", width);
    spriteJSON.insert("numberOfFrames", numberOfSprites);

    QJsonObject frames;
    // Accesses each frame
    for(int spriteIndex = 0; spriteIndex < numberOfSprites; spriteIndex++)
    {
        QJsonArray rows;
        // Accesses each row
        for(int rowIndex = 0; rowIndex < height; rowIndex++)
        {
            QJsonArray pixels;
            // Accesses each pixel in a different row
            for(int pixelIndex = 0; pixelIndex < width; pixelIndex++)
            {
                QJsonArray colors;
                // Retrieves the color of a particular pixel and saves its r,g,b,alpha values
                QColor currentPixelColor = grid[spriteIndex].pixelColor(pixelIndex, rowIndex);
                colors.push_back(currentPixelColor.red());
                colors.push_back(currentPixelColor.green());
                colors.push_back(currentPixelColor.blue());
                colors.push_back(currentPixelColor.alpha());
                pixels.push_back(colors);
            }
            // Each rows holds an array of each pixel in that row
            rows.push_back(pixels);
        }
        // Each frames holds an array of rows that describe a sprite
        frames.insert("frame" + QString::number(spriteIndex), rows);
    }
    spriteJSON.insert("frames", frames);
}

void pixelEditorModel::save(QString filename)
{
    qDebug() << "save clicked";
    writeJSON();
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly))
        return;
    QJsonDocument saveDoc(spriteJSON);
    saveFile.write(saveDoc.toJson());
}

void pixelEditorModel::load(QString filepath)
{
    //TODO
}

void pixelEditorModel::panUp()
{
    //TODO
}

void pixelEditorModel::panDown()
{
    //TODO
}

void pixelEditorModel::panLeft()
{
    //TODO
}

void pixelEditorModel::panRight()
{
    //TODO
}

void pixelEditorModel::addFrame()
{
    //TODO
}

void pixelEditorModel::deleteFrame()
{
    //TODO
}

void pixelEditorModel::selectFrame()
{
    //TODO
}

void pixelEditorModel::changeFPS()
{
    //TODO
}
