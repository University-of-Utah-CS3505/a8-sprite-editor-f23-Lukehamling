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
    void changePixel(int x, int y);
    void selectColor(QColor color);
    void createJSON();
    void save(QString filename);
    void load(QString filepath);
    void addFrame();
    void deleteFrame();
    void selectFrame();
    void changeFPS();

    /// @brief This method will recieve the signal with the x,y dimensions sent from the view then
    ///         create the first sprite
    void createInitialSprite(unsigned short int x, unsigned short int y);

/// @brief Recieves a signal from the view of (x,y) location and a fillColor. Then this will add
    ///         the current state of the Sprite to the undo stack then call the sprite's fill method.
    /// @param x is the x location of the selected pixel
    /// @param y is the y location of the selected pixel
    /// @param fillColor is the desired color to fill the Sprite with
    void fill(unsigned short int x, unsigned short int y, QColor fillColor);

    /// @brief Recieves a signal from the view to draw a circle with required locations and then
    ///         calls the drawCircle method on the currently selected Sprite object.
    /// @param startX is the starting x location that the user clicked
    /// @param startY is the starting y location that the user clicked
    /// @param endX is the ending x location that the user clicked
    /// @param endY is the ending y location that the user clicked
    void drawCircleOnSprite(unsigned short int startX, unsigned short int startY,
                            unsigned short int endX, unsigned short int endY);

    /// @brief Recieves a signal from the view to draw a rectangle with required locations and then
    ///         calls the drawRectangle method on the currently selected Sprite object.
    /// @param startX is the starting x location that the user clicked
    /// @param startY is the starting y location that the user clicked
    /// @param endX is the ending x location that the user clicked
    /// @param endY is the ending y location that the user clicked
    void drawRectangleOnSprite(unsigned short int startX, unsigned short int startY,
                               unsigned short int endX, unsigned short int endY);

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
    std::vector<Sprite> sprites;
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

