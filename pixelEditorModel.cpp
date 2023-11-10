#include "pixelEditorModel.h"
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
    // TODO
}

void pixelEditorModel::undo()
{
    qDebug() << "undo clicked";
    // TODO
}

void pixelEditorModel::changePixel(int x, int y)
{
    switch (currentTool) {
    case Pen:
        qDebug() << "coloring";
        frames[currentFrameIndex].setColor(x, y, currentColor);
        break;
    default:
        break;
    }
    //TODO
}

void pixelEditorModel::selectColor()
{
    QColor newColor = QColorDialog::getColor("Select Brush Color");

    if(newColor.isValid())
        currentColor = newColor;
}

void pixelEditorModel::save(QString filename)
{
    qDebug() << "save clicked";
    //TODO
}

void pixelEditorModel::load(QString filepath)
{
    //TODO
}

void pixelEditorModel::addFrame()
{
    Sprite newFrame(frames[currentFrameIndex].width, frames[currentFrameIndex].height);
    frames.push_back(newFrame);
    emit updateFrameBox(frames.size());

}

void pixelEditorModel::deleteFrame()
{
    frames.pop_back();
    emit updateFrameBox(-1);
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
