#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
class pixelEditorModel : public QObject
{
public:
    explicit pixelEditorModel(QObject *parent = nullptr);

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

    /// @brief This method will recieve the signal with the x,y dimensions sent from the view then
    ///         create the first sprite
    void createInitialSprite(unsigned short int x, unsigned short int y);

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
    std::vector<Sprite> frames;
    int currentFrameIndex;
    QColor currentColor;
    int fps;

    unsigned short int spriteWidth;
    unsigned short int spriteHeight;
};

#endif // PIXELEDITORMODEL_H

