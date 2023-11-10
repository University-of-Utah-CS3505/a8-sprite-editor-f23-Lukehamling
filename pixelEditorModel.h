#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
#include <QImage>
class pixelEditorModel : public QObject
{
    Q_OBJECT
public:
    explicit pixelEditorModel(QObject *parent = nullptr);

signals:
    void showFrameSignal(QImage image);

public slots:
    void redo();
    void undo();
    void changePixel();
    void selectColor();
    void save(QString filename);
    void load(QString filepath);
    void panUp();
    void panDown();
    void panLeft();
    void panRight();
    void addFrame();
    void deleteFrame();
    void selectFrame();
    void changeFPS(int newFPS);
    void playAnimation();

private:
    enum Tool {
        Circle,
        Square,
        Fill,
        Undo,
        Redo
    };

    void showFrame(int i);

    QStack<std::vector<Sprite>> redoStack;
    QStack<std::vector<Sprite>> undoStack;
    Tool currentTool;
    std::vector<Sprite> frames;
    int currentFrameIndex;
    QColor currentColor;
    int fps;

};

#endif // PIXELEDITORMODEL_H

