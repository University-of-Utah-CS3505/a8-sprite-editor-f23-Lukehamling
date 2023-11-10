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
    void updateFPSLabel(int newFPS);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
