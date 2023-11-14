/*
    Team:       Coders for Christ
    Authors:    Ryan Dalrymple, Vincentio Dane, Luke Hamling, August O'Rourke
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#include "pixelEditorModel.h"


pixelEditorModel::pixelEditorModel(QObject *parent)
{
    setParent(parent);

    currentColor      = QColor(0, 0, 0);
    currentTool       = Pen;
    currentFrameIndex = 0;
    fps               = 24;

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
    switch (currentTool)
    {
    case Pen:
        frames[currentFrameIndex].setColor(x, y, currentColor);
        storedX = x;
        storedY = y;
        break;

    case Erase:
        frames[currentFrameIndex].setColor(x, y, Qt::transparent);
        break;

    case Fill:
        frames[currentFrameIndex].fill(x, y, currentColor);
        break;

    case Rectangle:
        storedX = x;
        storedY = y;
        break;

    case Circle:
        storedX = x;
        storedY = y;
        break;

    default:
        break;
    }
}

void pixelEditorModel::movePixel(int x, int y)
{
    switch (currentTool)
    {
    case Pen:
        frames[currentFrameIndex].drawLine(storedX, storedY, x, y, currentColor);
        storedX = x;
        storedY = y;
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
    switch (currentTool)
    {
    case Rectangle:
        frames[currentFrameIndex].drawRectangle(storedX, storedY, x, y, currentColor);
        break;

    case Circle:
        frames[currentFrameIndex].drawCircle(storedX, storedY, x, y, currentColor);
        break;

    default:
        break;
    }
    // we dont have to clear storedX,Y
    //addToUndo();
}

void pixelEditorModel::selectColor()
{
    QColor newColor = QColorDialog::getColor("Select Brush Color");

    if(newColor.isValid())
    {
        currentColor = newColor;
    }
}

void pixelEditorModel::createJSON()
{
    spriteJSON.insert("height", frames[0].getHeight());
    spriteJSON.insert("width", frames[0].getWidth());
    spriteJSON.insert("numberOfFrames", (int)frames.size());

    QJsonObject frame;
    for(unsigned int spriteIndex = 0; spriteIndex < frames.size(); spriteIndex++)
    {
        QJsonArray rows;
        // loops through rows of pixel
        for(int rowIndex = 0; rowIndex < frames[spriteIndex].getHeight(); rowIndex++)
        {
            QJsonArray pixels;
            // loops through each pixel in said row
            for(int pixelIndex = 0; pixelIndex < frames[spriteIndex].getWidth(); pixelIndex++)
            {
                QJsonArray color;
                QColor currentPixelColor = frames[spriteIndex].getColor(pixelIndex, rowIndex);
                color.push_back(currentPixelColor.red());
                color.push_back(currentPixelColor.green());
                color.push_back(currentPixelColor.blue());
                color.push_back(currentPixelColor.alpha());
                pixels.push_back(color);
            }
            // Each row will hold an array of each pixel in that row
            rows.push_back(pixels);
        }
        // Each frame holds an array of rows that describe a sprite
        frame.insert("frame" + QString::number(spriteIndex), rows);
    }
    spriteJSON.insert("frame", frame);
}

void pixelEditorModel::save(QString filename)
{
    createJSON();
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    QJsonDocument saveDoc(spriteJSON);
    saveFile.write(saveDoc.toJson());
}

void pixelEditorModel::load(QString filename)
{


    try
    {
        QFile loadFile(filename);
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qWarning("unable to load file");
        }

        if(!frames.empty())
        {
            // gets rid of all the previous frames if loading when there is already an existing project
            for(unsigned int i = 0; i < frames.size(); i++)
            {
                emit updateFrameBox(-1);
            }
        }
        QByteArray saveData = loadFile.readAll();
        QJsonDocument loadDocument(QJsonDocument::fromJson(saveData));
        QJsonObject savedData = loadDocument.object();

        QJsonValue height         = savedData.value("height");
        int spriteHeight          = height.toInt();
        QJsonValue width          = savedData.value("width");
        int spriteWidth           = width.toInt();
        QJsonValue numberOfFrames = savedData.value("numberOfFrames");
        int numberOfframes        = numberOfFrames.toInt();
        QJsonObject savedSprites = savedData.value("frame").toObject();

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
                    int red   = colors.at(0).toInt();
                    int green = colors.at(1).toInt();
                    int blue  = colors.at(2).toInt();
                    int alpha = colors.at(3).toInt();
                    QColor currentPixelColor(red, green, blue, alpha);
                    frames[imageIndex].setColor(pixelIndex, rowIndex, currentPixelColor);
                }
            }
        }
        emit setSpriteSizeAfterLoading(spriteWidth, spriteHeight);
        emit updateCanvasViewSignal();

        // shows the pic in the animation window
        QImage img = showFrame(0);
        emit showFrameSignal(img);
        emit updateCanvas();
        if (numberOfFrames.toInt() > 1)
            showFrame(0);

        //gets rid of the extra option from the load method
        emit updateFrameBox(-1);

        //add all the frames to the frame picker
        for (unsigned int i = 1; i <= frames.size(); i++)
        {
            emit updateFrameBox(i);
        }
    }
    catch (...)
    {
        emit createErrorMessagePopup(QString("Error reading save file!"), QString("Unable to read selected saved file"));
    }
}

void pixelEditorModel::addFrame()
{
    Sprite newFrame = frames[frames.size() - 1];
    frames.push_back(newFrame);
    emit updateFrameBox(frames.size());
}

void pixelEditorModel::deleteFrame()
{
    if(frames.size() == 1)
    {
        Sprite frame = frames[0];
        frames[0] = Sprite(frame.getWidth(), frame.getHeight());
        emit updateFrameBox(0);
        return;
    }
    frames.erase(next(begin(frames), currentFrameIndex));
    if(currentFrameIndex > 0)
    {
        currentFrameIndex--;
    }
    emit updateFrameBox(-1);
}

void pixelEditorModel::selectFrame(int data)
{
    currentFrameIndex = data;
}

void pixelEditorModel::setStopped(bool stopped)
{
    this->stopped = stopped;
    if(!stopped)
    {
        playAnimation();
    }
}

void pixelEditorModel::playAnimation()
{
    if (stopped)
    {
        return;
    }
    for(size_t i = 0; i < frames.size(); i++)
    {
        QImage frame = showFrame(i);
        QTimer::singleShot(i * (1000/fps), Qt::PreciseTimer, this, [this, frame](){emit showFrameSignal(frame); });
    }
    QTimer::singleShot(frames.size() * (1000/fps), Qt::PreciseTimer, this, [this](){playAnimation();});
}

QImage pixelEditorModel::showFrame(int i)
{
    Sprite frame = frames.at(i);
    QImage image = QImage(frame.getWidth(), frame.getHeight(), QImage::Format_ARGB32);

    for (size_t i = 0; i < frame.getWidth(); i++)
    {
        for (size_t j = 0; j < frame.getHeight(); j++)
        {
            image.setPixelColor(i, j, frame.getColor(i, j));
        }
    }
    return image;
}

void pixelEditorModel::changeFPS(int newFPS)
{
    fps = newFPS;
}

void pixelEditorModel::createInitialSprite(unsigned short int x, unsigned short int y)
{
    this->spriteWidth   = x;
    this->spriteHeight  = y;

    frames.pop_back(); // remove the 0,0 sprite
    Sprite initial(x,y);
    frames.push_back(initial);
}

void pixelEditorModel::updateSelectedTool(int tool)
{
    Tool selected = (Tool) tool;

    currentTool = selected;
}

unsigned short int pixelEditorModel::getWidth()
{
    return this->spriteWidth;
}

unsigned short int pixelEditorModel::getHeight()
{
    return this->spriteHeight;
}
