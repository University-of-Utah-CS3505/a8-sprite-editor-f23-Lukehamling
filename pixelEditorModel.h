#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
#include <QJsonObject>

class pixelEditorModel : public QObject
{
public:
    explicit pixelEditorModel(QObject *parent = nullptr);

public slots:
    void redo();
    void undo();
    void addToRedo();
    void addToUndo();
    void changePixel();
    void selectColor(QColor color);
    void createJSON();
    void save(QString filename);
    void load(QString filename);
    void panUp();
    void panDown();
    void panLeft();
    void panRight();
    void addFrame();
    void deleteFrame();
    void selectFrame();
    void changeFPS();

private:
    enum Tool {
        Circle,
        Square,
        Fill,
        Undo,
        Redo
    };

    QStack<std::vector<Sprite>> redoStack;
    QStack<std::vector<Sprite>> undoStack;
    Tool currentTool;
    std::vector<Sprite> sprites;
    int currentFrameIndex;
    QColor currentColor;
    int fps;
    QJsonObject spriteJSON;
};

#endif // PIXELEDITORMODEL_H

