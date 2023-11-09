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
    QRect whiteOutBoxLeft;
    QRect whiteOutBoxRight;
    QRect whiteOutBoxTop;
    QRect whiteOutBoxBottom;
    int canvasCenterx;
    int canvasCentery;

    // these are inverse, so -5 to the center would move it 5 to the right.
    int scale = 0;
    float focusSpriteCenterx = 0;
    float focusSpriteCentery = 0;
    int xOffset = 0;
    int yOffset = 0;

    Sprite loadedSprite = Sprite(32,32);

public:
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*) override;
    void changeCanvasView(float focusOnCanvasX, float focusOnCanvasY, int newScale);

public slots:
    void valueChanged();

private:
    Ui::MainWindow *ui;
    bool checkInCanvas(int& canvasX, int& canvasY);
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};
#endif // MAINWINDOW_H
