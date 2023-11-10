#include "pixelEditorModel.h"
#include <QColorDialog>
#include <QDebug>
#include <QTimer>

pixelEditorModel::pixelEditorModel(QObject *parent)
{
    setParent(parent);
    currentColor = QColor(0, 0, 0);
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

void pixelEditorModel::changePixel()
{
    //TODO
}

void pixelEditorModel::selectColor()
{
    QColor newColor =QColorDialog::getColor("Select Brush Color");

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

void pixelEditorModel::playAnimation()
{
    for(size_t i =0; i < frames.size(); i++)
    {
        QTimer::singleShot(i * (1000/fps), Qt::PreciseTimer, this, [this, i](){showFrame(i); });
    }
}

void pixelEditorModel::showFrame(int i)
{
    Sprite frame = frames.at(i);
    QImage image = QImage(frame.width, frame.height, QImage::Format_ARGB32);

    for (size_t i = 0; i < frame.width; i++) {
        for (size_t j = 0; j < frame.height; j++) {
            image.setPixelColor(i, j, frame.getColor(i, j));
        }
    }
    emit showFrameSignal(image);
}

void pixelEditorModel::changeFPS(int newFPS)
{
    fps = newFPS;
}
