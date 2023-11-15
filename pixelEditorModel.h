/*
    Team:       Coders for Christ
    Authors:    Ryan Dalrymple, Vincentio Dane, Luke Hamling, August O'Rourke
    Class:      CS3505
    Assignment: 8 - Sprite Editor
    Reviewed by: Vincentio Dane
*/

#ifndef PIXELEDITORMODEL_H
#define PIXELEDITORMODEL_H

#include "sprite.h"
#include <QStack>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QVector>
#include <QFileDialog>
#include <QColorDialog>
#include <QColorDialog>
#include <QTimer>

#include <QImage>
class PixelEditorModel : public QObject
{
    Q_OBJECT
public:
    ///@brief the default constructer for our model
    explicit PixelEditorModel(QObject *parent = nullptr);
    ///@brief this method returns the sprite that is currently selected by the model
    Sprite* getSelectedSprite();

    /// @brief Getter for the Sprite width.
    /// @return Width of Sprite
    unsigned short int getWidth();

    /// @brief Getter for the Sprite height.
    /// @return Height of Sprite
    unsigned short int getHeight();

    ///@brief An enumerator to help select the tool that the user has selected
    enum Tool
    {
        Pen,
        Erase,
        Circle,
        Rectangle,
        Fill
    };

public slots:
    ///@brief this method reverts the last undo action,
    void redo();

    ///@brief this method reverts the last change to the sprite
    void undo();

    ///@brief this method calls the appropriate method to edit the sprite based off of which tool is selected, and where the sprite was clicked
    ///@param x - the x position of the sprite where it was clicked
    ///@param y - the y position of the sprite where it was clicked
    void clickPixel(int x, int y);

    ///@brief this method changes the pixel that the mouse was dragged to, only interacts with the pen and eraser
    ///@param x - the x position of the sprite where the mouse was moved to
    ///@param y - the y position of the sprite where the mouse was moved to
    void movePixel(int x, int y);

    ///@brief this method captures the position of the mouse when it was released, only interacts with the drawing shapes tool
    ///@param x - the x position of the sprite where the mouse was released
    ///@param y - the y position of the sprite where the mouse was released
    void releasePixel(int x, int y);

    ///@brief this method changes the color that the brush has selected
    void selectColor();

    ///@brief this method creates a JSON object to save the current state of the project with.
    void createJSON();

    ///@brief this method saves the state of the project to a .ssp file, which is really a json relpesntation of its variables.
    ///@param filename - the name of the file that the project will be saved to.
    void save(QString filename);

    ///@brief this method loads the state of the project from a .ssp file
    ///@param filepath - the filepath of the .ssp file we are loading the project from
    void load(QString filepath);

    ///@brief this method adds a frame to the project.
    void addFrame();

    ///@brief this method deletes a frame from the project
    void deleteFrame();

    ///@brief this method changes the frame to the one at the index passed into the method
    ///@param data - the index of the new frame in the frames vector
    void selectFrame(int data);

    ///@brief this method changes the FPS of the animation
    ///@param newFPS - the FPS value we are changing to
    void changeFPS(int newFPS);

    ///@brief this method controls the logic for playing the animation in the view
    void playAnimation();

    ///@brief this method changes the stopped bool which plays or stops the animation
    ///@param stopped - the bool that decides if the animation should play or not
    void setStopped(bool stopped);

    ///@brief This method will recieve the signal with the x,y dimensions sent from the view then
    ///       create the first sprit
    ///@param x - the width of the sprite
    ///@param y - the height of the sprite
    void createInitialSprite(unsigned short int x, unsigned short int y);

    /// @brief Changes the selected tool based on what the user has selected.
    /// @param selectedTool - the tool the user has selected.
    void updateSelectedTool(Tool tool);

signals:
    ///@brief this tells the view to add a new element in the frameSelector comboBox
    ///@param data - the number of the frame to add
    void updateFrameBox(int data);

    ///@brief this signal tells the view to redraw the canvas
    void updateCanvas();

    ///@brief a signal that just tells the view to show the next frame of the animation
    ///@param image - A QImage representation of the frame to show.
    void showFrameSignal(QImage image);

    ///@brief creates a message pop-up when a save file can't be read properly
    ///@param windowTitle - the title of the popUp
    ///@param errorMessage - error message to display
    void createErrorMessagePopup(QString windowTitle, QString errorMessage);

    ///@brief sets the size of the sprite after loading
    ///@param width - the width of the sprite
    ///@param height - the height of the sprite
    void setSpriteSizeAfterLoading(unsigned short int width, unsigned short int height);

    ///@brief tells the canvas to redraw
    void updateCanvasViewSignal();

    ///@brief Calculates a new scale after loading a Sprite to make sure that the
    ///         scale is correct when zooming.
    ///@param width is the width of the Sprite
    ///@param height is the height of the Sprite
    void recalculateScale(unsigned short int width, unsigned short int height);

private:

    bool stopped;
    QStack<Sprite> redoStack;
    QStack<Sprite> undoStack;
    vector<Sprite> frames;
    Tool currentTool;
    QColor currentColor;
    QJsonObject spriteJSON;
    int storedX;
    int storedY;
    int fps;
    int currentFrameIndex{0};
    unsigned short int spriteWidth;
    unsigned short int spriteHeight;

    ///@brief this is a small helper method to convert the sprite into a QImage
    ///@param i - the index of the frame we are converting into a QImage
    ///@return the frame as a QImage Object
    QImage showFrame(int i);

    /// @brief Pops a Sprite off of the undo stack and adds it to the redo stack
    /// @return the Sprite object that was popped off of the undo stack
    Sprite addToRedo();

    /// @brief Saves a copy of the currently selected Sprite by pushing it to the undo stack.
    /// @return The Sprite that was pushed to the undo stack.
    Sprite addToUndo();
};

#endif // PIXELEDITORMODEL_H

