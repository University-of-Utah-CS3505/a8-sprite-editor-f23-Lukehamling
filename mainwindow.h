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

    // these are inverse, so -5 to the center would move it 5 to the right.
    int scale = 0;
    float focusSpriteCenterx = 0;
    float focusSpriteCentery = 0;
    int xOffset = 0;
    int yOffset = 0;

public:
    ///@brief this is the constructor for our mainWindow
    ///@param model - this is the model for our sprite editor
    ///@param parent - the parent for our main window
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);
    ///@brief this is the destructor for our mainWindow
    ~MainWindow();
    ///@brief this is the override of the paint event for our model, it is used to draw things in the canvas
    void paintEvent(QPaintEvent*) override;
    ///@brief this method changes the scale of the canvas for zooming and panning
    void updateCanvasView();

public slots:
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
    void saveClicked();
    /// @brief The main screen for editing sprites. This contains the Sprite editor grid, along with all tools
    ///         and animation window.
    ///@brief When the load button is clicked, this will prompt the user to select a JSON file to load.
    void loadClicked();
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

signals:
    ///@brief Emitted after the user opts to create a new sprite, selects a size, and hits start. The model will
    ///        use this size to create a new Sprite.
    void selectedSpriteSize(int x, int y);
    void loadFileSelected(QString filename);
    void saveFileSelected(QString filename);



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
    void mainScreen();
    ///@brief Loads the combo box with appropriate Sprite sizes that the user can select.
    void populateSpriteSizeComboBox();
    ///@brief this method checks if the mouse is within the bounds of the canvas
    ///@param x - the x position of the mouse
    ///@param y - the y position of the mouse
    bool checkInCanvas(int& x, int& y);
    ///@brief Overrides the default mouse press event so we can use it for drawing
    virtual void mousePressEvent(QMouseEvent* event) override;
    ///@brief Overrides the default mouse move event so we can use it for drawing
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    ///@brief Overrides the default mouse release event so we can use it for drawing
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // MAINWINDOW_H
