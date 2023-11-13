/*
    Team:       Coders for Christ
    Authors:    Ryan Dalrymple, Vincentio Dane, Luke Hamling, August O'Rourke
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pixelEditorModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    // constants
    QRect WhiteOutBoxLeft;
    QRect WhiteOutBoxRight;
    QRect WhiteOutBoxTop;
    QRect WhiteOutBoxBottom;
    int CanvasCenterx;
    int CanvasCentery;
    int OriginalScale{0};

    // these are inverse, so -5 to the center would move it 5 to the right.
    int scale{0};
    float focusSpriteCenterx{0};
    float focusSpriteCentery{0};
    int xOffset{0};
    int yOffset{0};

public:
    ///@brief this is the constructor for our mainWindow
    ///@param model - this is the model for our sprite editor
    ///@param parent - the parent for our main window
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);

    ///@brief this is the destructor for our mainWindow
    ~MainWindow();

    ///@brief this is the override of the paint event for our model, it is used to draw things in the canvas
    void paintEvent(QPaintEvent*) override;

public slots:
    ///@brief this method changes the scale of the canvas for zooming and panning
    void updateCanvasView();

    ///@brief this is a method that just calls update, for when we are changing pixels in the model
    void valueChanged();

    ///@brief this method changes what is drawn on the canvas when we pan up
    void panUp();

    ///@brief this method changes what is drawn on the canvas when we pan down
    void panDown();

    ///@brief this method changes what is drawn on the canvas when we pan left
    void panLeft();

    ///@brief this method changes what is drawn on the canvas when we pan right
    void panRight();

    ///@brief opens the file explorer and prompt the user a filename and file directory to save the file at (file will be saved with a .ssp format)
    void saveClicked();

    ///@brief When the load button is clicked, this will prompt the user to select a .ssp file to load.
    void loadClicked();

    ///@brief creates a message pop-up when a save file can't be read properly
    ///@param windowTitle - the title of the popUp
    ///@param errorMessage - error message to display
    void createErrorMessagePopup(QString windowTitle, QString errorMessage);

    ///@brief this method changes the elements in the frame selector combo box, mainly used when we are adding and deleting frames
    ///@param data - this is the index of the new element, it will be -1 when we are deleting a frame and 0 when the user hits the
    ///       delete button but there is only one frame
    void changeFrameBox(int data);

    ///@brief this method displays a QImage representing the next frame in the animation in the animation Preview wind
    void showFrame(QImage image);

    ///@brief this method changes the text of the label that displays the current FPS value
    ///@param newFPS - this is the FPS value the label will be set to
    void updateFPSLabel(int newFPS);

    /// @brief Mathematically determine the focused center of the Sprite so we can set the canvas size on screen.
    /// @param width is the width of the Sprite
    /// @param height is the height of the Sprite
    void calculateFocusCenter(unsigned short int width, unsigned short int height);

    /// @brief Sets the view to be the main editing window for creating Sprites.
    void mainScreen();

signals:
    ///@brief Emitted after the user opts to create a new sprite, selects a size, and hits start. The model will
    ///       use this size to create a new Sprite.
    void selectedSpriteSize(int x, int y);

    ///@brief sends the model a signal to execute the appropriate action when the load button is clicked
    void loadFileSelected(QString filename);

    ///@brief sends the model a signal to execute the appropriate action when the save button/shortcut is clicked
    void saveFileSelected(QString filename);

    ///@brief Sends the model the signal to change the selected tool.
    void toolSelected(int tool);

private:
    Ui::MainWindow *ui;
    pixelEditorModel* editorModel;

    ///@brief Displays the start up screen while hiding all other elements of the UI. Forces a user to choose
    ///       whether to create a new sprite or load an old one.
    void setupStartScreen();

    ///@brief After the user selects to create a new sprite, this method will be called and will allow them to
    ///       choose from a few pre-selected sprite sizes.
    void newSpriteScreen();

    ///@brief Called when the start button has been clicked. Determines the size of a new sprite based on user
    ///       selection from the ComboBox, emits the signal, then switches the view to the editor 'mainScreen'.
    void startButtonClicked();

    ///@brief When the fill button is clicked, this will emit the correct signal to the model.
    void penButtonClicked();

    ///@brief When the fill button is clicked, this will emit the correct signal to the model.
    void fillButtonClicked();

    ///@brief When the rectangle button is clicked, this will emit the correct signal to the model.
    void rectangleButtonClicked();

    ///@brief When the circle button is clicked, this will emit the correct signal to the model.
    void circleButtonClicked();

    ///@brief When the erase button is clicked, this will emit the correct signal to the model.
    void eraseButtonClicked();

    ///@brief Loads the combo box with appropriate Sprite sizes that the user can select.
    void populateSpriteSizeComboBox();

    ///@brief this method checks if the mouse is within the bounds of the canvas
    ///@param x - the x position of the mouse
    ///@param y - the y position of the mouse
    bool checkInCanvas(int& x, int& y);

    /// @brief Helper method to determine if an (x,y) location is outside of the canvas
    /// @returns true if outside canvas, false otherwise.
    bool isOutsideCanvas(const int& x, const int& y);

    /// @brief Helper method to determine if an (x,y) location is outside of the Sprite
    /// @returns true if outside Sprite, false otherwise.
    bool isOutsideSprite(const int& x, const int& y, Sprite* loadedSprite);

    ///@brief Overrides the default mouse press event so we can use it for drawing
    virtual void mousePressEvent(QMouseEvent* event) override;

    ///@brief Overrides the default mouse move event so we can use it for drawing
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    ///@brief Overrides the default mouse release event so we can use it for drawing
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

    ///@brief Determine when the scroll wheel is turned. Used to determine zooms.
    virtual void wheelEvent(QWheelEvent* event) override;
};
#endif // MAINWINDOW_H
