#include "pixelEditorModel.h"
#include <QColorDialog>>

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

void pixelEditorModel::changeFPS()
{
    //TODO
}
