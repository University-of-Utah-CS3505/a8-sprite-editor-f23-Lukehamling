#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
#include <QJsonObject>

class pixelEditorModel : public QObject
{
    Q_OBJECT

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

    /// @brief Recieves a signal from the view of (x,y) location and a fillColor. Then this will add
    ///         the current state of the Sprite to the undo stack then call the sprite's fill method.
    void fill();

private:
    enum Tool {
        Circle,
        Square,
        Fill,
        Undo,
        Redo
    };

    QStack<Sprite> redoStack;
    QStack<Sprite> undoStack;
    Tool currentTool;
    std::vector<Sprite> sprites;
    int currentFrameIndex{0};
    QColor currentColor;
    int fps;
    QJsonObject spriteJSON;
};

#endif // PIXELEDITORMODEL_H

