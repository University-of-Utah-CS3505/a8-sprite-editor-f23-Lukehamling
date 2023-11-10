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

    QRect whiteOutBoxLeft;
    QRect whiteOutBoxRight;
    QRect whiteOutBoxTop;
    QRect whiteOutBoxBottom;
    int canvasCenterx;
    int canvasCentery;

    // these are inverse, so -5 to the center would move it 5 to the right.
    float focusSpriteCenterx;
    float focusSpriteCentery;

    Sprite loadedSprite = Sprite(32,32);

public:
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);

public slots:
    void valueChanged();

private:
    Ui::MainWindow *ui;

    /// @brief Displays the start up screen while hiding all other elements of the UI. Forces a user to choose
    ///         whether to create a new sprite or load an old one.
    void setupStartScreen();

    /// @brief After the user selects to create a new sprite, this method will be called and will allow them to
    ///         choose from a few pre-selected sprite sizes.
    void newSpriteScreen();

    /// @brief The main screen for editing sprites. This contains the Sprite editor grid, along with all tools
    ///         and animation window.
    void mainScreen();
};
#endif // MAINWINDOW_H
