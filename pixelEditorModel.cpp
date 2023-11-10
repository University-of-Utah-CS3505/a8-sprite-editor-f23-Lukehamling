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
#include <QColorDialog>

pixelEditorModel::pixelEditorModel(QObject *parent)
{
    setParent(parent);
    currentColor = QColor(0, 0, 0);
    currentTool = Pen;
    currentFrameIndex = 0;
    Sprite initial(16,16);
    frames.push_back(initial);
}

Sprite* pixelEditorModel::getSelectedSprite()
{
    return &frames[currentFrameIndex];
}

void pixelEditorModel::redo()
{
    qDebug() << "redo clicked";

    if(redoStack.size() > 0)
    {
        addToUndo();
        frames.at(currentFrameIndex) = redoStack.pop();

        emit updateCanvas();
    }
}

void pixelEditorModel::undo()
{
    qDebug() << "undo clicked";
    if (undoStack.size() > 0) {
        Sprite currentSprite = frames[currentFrameIndex];
        Sprite oldSprite = undoStack.pop();
        redoStack.push(currentSprite);
        frames[currentFrameIndex] = oldSprite;

        emit updateCanvas();
    }
}

Sprite pixelEditorModel::addToUndo()
{
    Sprite tempSprite = frames[currentFrameIndex];
    undoStack.push(tempSprite);
    return tempSprite;
}

void pixelEditorModel::clickPixel(int x, int y)
{
    addToUndo();
    switch (currentTool) {
    case Pen:
        frames[currentFrameIndex].setColor(x, y, currentColor);
        break;
    case Erase:
        frames[currentFrameIndex].setColor(x, y, Qt::transparent);
        break;
    case Fill:
        frames[currentFrameIndex].fill(x, y, currentColor);
        break;
    case Rectangle:
        // TODO store xy
        break;
    case Circle:
        // TODO store xy
        break;
    default:
        break;
    }
}

void pixelEditorModel::movePixel(int x, int y)
{
    switch (currentTool) {
    case Pen:
        // todo draw a line from point to point
        frames[currentFrameIndex].setColor(x, y, currentColor);
        break;
    case Erase:
        frames[currentFrameIndex].setColor(x, y, Qt::transparent);
        break;
    default:
        break;
    }
}
void pixelEditorModel::releasePixel(int x, int y)
{
    switch (currentTool) {
    default:
    case Rectangle:
//        frames[currentFrameIndex].drawRectangle(storedX, storedY, x, y, currentColor);
        break;
    case Circle:
//        frames[currentFrameIndex].drawCircle(storedX, storedY, x, y, currentColor);
        break;
    }
    // we dont have to clear storedX,Y
    //addToUndo();
}

void pixelEditorModel::selectColor()
{
    QColor newColor = QColorDialog::getColor("Select Brush Color");

    if(newColor.isValid())
        currentColor = newColor;
}

void pixelEditorModel::createJSON()
{
    spriteJSON.insert("height", frames[0].height);
    spriteJSON.insert("width", frames[0].width);
    spriteJSON.insert("numberOfFrames", (int)frames.size());

    QJsonObject frame;
    for(unsigned int spriteIndex = 0; spriteIndex < frames.size(); spriteIndex++)
    {
        QJsonArray rows;
        // loops through row of pixel
        for(int rowIndex = 0; rowIndex < frames[spriteIndex].height; rowIndex++)
        {
            QJsonArray pixels;
            // loops through each pixel in said row
            for(int pixelIndex = 0; pixelIndex < frames[spriteIndex].width; pixelIndex++)
            {
                QJsonArray colors;
                QColor currentPixelColor = frames[spriteIndex].getColor(pixelIndex, rowIndex);
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
        int numberOfframes = numberOfFrames.toInt();

        QJsonObject savedSprites = savedData.value("frames").toObject();
        frames.clear();

        for(int imageIndex = 0; imageIndex < numberOfframes; imageIndex++)
        {
            Sprite img = Sprite(spriteWidth, spriteHeight);
            frames.push_back(img);

            QString rowName = "frame" + QString::number(imageIndex);
            QJsonArray rows = savedSprites.value(rowName).toArray();

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
                    frames[imageIndex].setColor(pixelIndex, rowIndex, currentPixelColor);
                }
            }
        }

        // Emits signals to the UI to reflect the images of the loaded file rather than its current state.
//        emit setUpCanvasSize(spriteWidth, spriteHeight);
//        emit updateMainCanvas(frames.at(0));
//        emit updatePreviewWindow(frames.at(0));
//        int currentSprite = 0;
//        if (numberOfframes > 1)
//            changeFPS();
    }
    catch (...)
    {
        // emit a signal to create a message popup saying an error occurred when reading
    }
}

void pixelEditorModel::addFrame()
{
    //TODO: implement this method
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

void pixelEditorModel::createInitialSprite(unsigned short int x, unsigned short int y)
{
    this->spriteWidth   = x;
    this->spriteHeight  = y;

    frames.pop_back(); // remove the 0,0 sprite
    Sprite initial(x,y);
    frames.push_back(initial);
}

