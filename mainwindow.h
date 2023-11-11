#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sprite.h"
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
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*) override;
    void updateCanvasView();

public slots:
    void valueChanged();
    void panUp();
    void panDown();
    void panLeft();
    void panRight();
    void changeFrameBox(int data);

signals:
    /// @brief Emitted after the user opts to create a new sprite, selects a size, and hits start. The model will
    ///         use this size to create a new Sprite.
    void selectedSpriteSize(int x, int y);
    void updateFPSLabel(int newFPS);
    void showFrame(QImage image);

private:
    Ui::MainWindow *ui;
    pixelEditorModel* editorModel;

    /// @brief Displays the start up screen while hiding all other elements of the UI. Forces a user to choose
    ///         whether to create a new sprite or load an old one.
    void setupStartScreen();

    /// @brief After the user selects to create a new sprite, this method will be called and will allow them to
    ///         choose from a few pre-selected sprite sizes.
    void newSpriteScreen();

    /// @brief Called when the start button has been clicked. Determines the size of a new sprite based on user
    ///         selection from the ComboBox, emits the signal, then switches the view to the editor 'mainScreen'.
    void startButtonClicked();

    /// @brief When the load button is clicked, this will prompt the user to select a JSON file to load.
    void loadButtonClicked();

    /// @brief The main screen for editing sprites. This contains the Sprite editor grid, along with all tools
    ///         and animation window.
    void mainScreen();

    /// @brief Loads the combo box with appropriate Sprite sizes that the user can select.
    void populateSpriteSizeComboBox();
    bool checkInCanvas(int& canvasX, int& canvasY);
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // MAINWINDOW_H
