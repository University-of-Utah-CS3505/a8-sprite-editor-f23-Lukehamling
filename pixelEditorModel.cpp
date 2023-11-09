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
    //todo
}

void pixelEditorModel::addToUndo()
{
    //todo
}

void pixelEditorModel::changePixel()
{
    //TODO
}

void pixelEditorModel::selectColor(QColor color)
{
    //emit some signal
}

void pixelEditorModel::createJSON()
{
    spriteJSON.insert("height", sprites[0].height);
    spriteJSON.insert("width", sprites[0].width);
    spriteJSON.insert("numberOfFrames", (int)sprites.size());

    QJsonObject frame;
    for(unsigned int spriteIndex = 0; spriteIndex < sprites.size(); spriteIndex++)
    {
        QJsonArray rows;
        // loops through row of pixel
        for(int rowIndex = 0; rowIndex < sprites[spriteIndex].height; rowIndex++)
        {
            QJsonArray pixels;
            // loops through each pixel in said row
            for(int pixelIndex = 0; pixelIndex < sprites[spriteIndex].width; pixelIndex++)
            {
                QJsonArray colors;
                QColor currentPixelColor = sprites[spriteIndex].getColor(pixelIndex, rowIndex);
                colors.push_back(currentPixelColor.red());
                colors.push_back(currentPixelColor.green());
                colors.push_back(currentPixelColor.blue());
                colors.push_back(currentPixelColor.alpha());
                pixels.push_back(colors);
            }
            // Each rows will hold an array of each pixel in that row
            rows.push_back(pixels);
        }
        // Each frames holds an array of rows that describe a sprite
        frame.insert("frame" + QString::number(spriteIndex), rows);
    }
    spriteJSON.insert("frame", frame);
}

void pixelEditorModel::save(QString filename)
{
    qDebug() << "save clicked";
    createJSON();
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly))
        return;
    QJsonDocument saveDoc(spriteJSON);
    saveFile.write(saveDoc.toJson());
}

void pixelEditorModel::load(QString filename)
{
    try
    {
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly))
            qWarning("unable to load file");

        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDocument(QJsonDocument::fromJson(saveData));
        QJsonObject savedData = loadDocument.object();

        QJsonValue height = savedData.value("height");
        int spriteHeight = height.toInt();
        QJsonValue width = savedData.value("width");
        int spriteWidth = width.toInt();
        QJsonValue numberOfFrames = savedData.value("numberOfFrames");
        int numberOfSprites = numberOfFrames.toInt();

        QJsonObject frames = savedData.value("frames").toObject();
        sprites.clear();

        for(int imageIndex = 0; imageIndex < numberOfSprites; imageIndex++)
        {
            Sprite img = Sprite(spriteWidth, spriteHeight);
            sprites.push_back(img);

            QString rowName = "frame" + QString::number(imageIndex);
            QJsonArray rows = frames.value(rowName).toArray();

            for(int rowIndex = 0; rowIndex < spriteHeight; rowIndex++)
            {
                QJsonArray pixels = rows.at(rowIndex).toArray();
                for(int pixelIndex = 0; pixelIndex < spriteWidth; pixelIndex++)
                {
                    QJsonArray colors = pixels.at(pixelIndex).toArray();
                    int red = colors.at(0).toInt();
                    int green = colors.at(1).toInt();
                    int blue = colors.at(2).toInt();
                    int alpha = colors.at(3).toInt();
                    QColor currentPixelColor(red, green, blue, alpha);
                    sprites[imageIndex].setColor(pixelIndex, rowIndex, currentPixelColor);
                }
            }
        }

        // Emits signals to the UI to reflect the images of the loaded file rather than its current state.
//        emit setUpCanvasSize(spriteWidth, spriteHeight);
//        emit updateMainCanvas(sprites.at(0));
//        emit updatePreviewWindow(sprites.at(0));
//        int currentSprite = 0;
//        if (numberOfSprites > 1)
//            changeFPS();
    }
    catch (...)
    {
        // emit a signal to create a message popup saying an error occurred when reading
    }
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
