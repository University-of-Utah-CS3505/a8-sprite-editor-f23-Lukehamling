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
    Sprite* getSelectedSprite();

    unsigned short int spriteWidth;
    unsigned short int spriteHeight;

public slots:
    void redo();
    void undo();
    void clickPixel(int x, int y);
    void movePixel(int x, int y);
    void releasePixel(int x, int y);
    void selectColor();
    void createJSON();
    void save(QString filename);
    void load(QString filepath);
    void addFrame();
    void deleteFrame();
    void selectFrame(int data);
    void changeFPS();

    /// @brief This method will recieve the signal with the x,y dimensions sent from the view then
    ///         create the first sprite
    void createInitialSprite(unsigned short int x, unsigned short int y);

signals:
    void updateFrameBox(int data);
    void updateCanvas();

private:
    enum Tool {
        Pen,
        Erase,
        Circle,
        Rectangle,
        Fill
    };

    QStack<Sprite> redoStack;
    QStack<Sprite> undoStack;
    Tool currentTool;
    std::vector<Sprite> frames;
    int currentFrameIndex{0};
    QColor currentColor;
    int fps;
    QJsonObject spriteJSON;

    /// @brief Pops a Sprite off of the undo stack and adds it to the redo stack
    /// @return the Sprite object that was popped off of the undo stack
    Sprite addToRedo();

    /// @brief Saves a copy of the currently selected Sprite by pushing it to the undo stack.
    /// @return The Sprite that was pushed to the undo stack.
    Sprite addToUndo();
};

#endif // PIXELEDITORMODEL_H

