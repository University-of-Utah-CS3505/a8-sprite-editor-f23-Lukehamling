#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
class pixelEditorModel : public QObject
{
    Q_OBJECT
public:

    explicit pixelEditorModel(QObject *parent = nullptr);
    Sprite* getSelectedSprite();

    unsigned short int spriteWidth;
    unsigned short int spriteHeight;

signals:
    void updateFrameBox(int data);
public slots:
    void redo();
    void undo();
    void changePixel(int x, int y);
    void selectColor();
    void save(QString filename);
    void load(QString filepath);
    void addFrame();
    void deleteFrame();
    void selectFrame();
    void changeFPS();

    /// @brief This method will recieve the signal with the x,y dimensions sent from the view then
    ///         create the first sprite
    void createInitialSprite(unsigned short int x, unsigned short int y);

private:
    enum Tool {
        Pen,
        Erase,
        Square,
        Circle,
        Fill

    };

    QStack<std::vector<Sprite>> redoStack;
    QStack<std::vector<Sprite>> undoStack;
    Tool currentTool;
    std::vector<Sprite> frames;
    int currentFrameIndex;
    QColor currentColor;
    int fps;
};

#endif // PIXELEDITORMODEL_H

