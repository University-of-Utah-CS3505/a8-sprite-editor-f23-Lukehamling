#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
class pixelEditorModel : public QObject
{
public:
    explict pixelEditorModel();

public slots:
    void redo();
    void undo();
    void changePixel();
    void selectColor(QColor color);
    void save(QString filename);
    void load(QString filepath);
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

    QStack<std::vector<Sprite>> redo;
    QStack<std::vector<Sprite>> undo;
    Tool currentTool;
    std::vector<Sprite> frames;
    Sprite currentFrame;
    QColor currentColor;
    int fps;

}

#endif // PIXELEDITORMODEL_H

