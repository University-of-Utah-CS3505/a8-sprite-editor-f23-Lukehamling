#include "mainwindow.h"
#include "pixelEditorModel.h"
#include "ui_mainwindow.h"

#include <string>
#include <QPainter>
#include <QPen>
#include <QShortcut>
#include <QMouseEvent>

using namespace std;
void print(int num1, string toPrint, int num2) {
    qDebug() << num1 << toPrint << num2;
}
void print(string toPrint) {
    qDebug() << toPrint;
}

MainWindow::MainWindow(pixelEditorModel& model, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const int canvasEdgeRight = ui->canvas->x() + ui->canvas->width();
    const int canvasEdgeBottom = ui->canvas->y() + ui->canvas->height();
    whiteOutBoxLeft = QRect(0,0,ui->canvas->x(),this->height());
    whiteOutBoxRight = QRect(canvasEdgeRight,0,this->width() - canvasEdgeRight,this->height());
    whiteOutBoxTop = QRect(ui->canvas->x(),0,ui->canvas->width(),ui->canvas->y());
    whiteOutBoxBottom = QRect(ui->canvas->x(),canvasEdgeBottom,ui->canvas->width(),this->height() - canvasEdgeBottom);

    canvasCenterx = ui->canvas->x() + (ui->canvas->width() / 2.0);
    canvasCentery = ui->canvas->y() + (ui->canvas->height() / 2.0);
    // set the focus center and zoom/scale of our view
    changeCanvasView(16,16,10);

    // todo remove
    // make a gradient grid for testing
    for (size_t i = 0; i < loadedSprite.width; i++) {
        for (size_t j = 0; j < loadedSprite.height; j++) {
            loadedSprite.setColor(i,j, QColor(i*7,j*7,i+j+80));
        }
    }


    print(ui->canvas->x(),",",ui->canvas->y());
    print(ui->canvas->height(),",",ui->canvas->width());

    // populate the combo box
//    ui->shapeComboBox->addItem("Rectangle");
//    ui->shapeComboBox->addItem("Ellipse");

//    // put limits on the pen width
//    ui->penWidthSpinBox->setRange(0, 20);

//    connect(ui->shapeComboBox,
//            &QComboBox::activated,
//            this,
//            &MainWindow::valueChanged);

//    connect(ui->penWidthSpinBox,
//            &QSpinBox::valueChanged,
//            this,
//            &MainWindow::valueChanged);

    // showing icons on the ui
    ui -> undoButton        -> setIcon(QIcon(":/buttons/undo.png"));
    ui -> redoButton        -> setIcon(QIcon(":/buttons/redo.png"));
    ui -> saveButton        -> setIcon(QIcon(":/buttons/save.png"));
    ui -> loadButton        -> setIcon(QIcon(":/buttons/load.png"));
    ui -> rectangleButton   -> setIcon(QIcon(":/buttons/rectangle.png"));
    ui -> circleButton      -> setIcon(QIcon(":/buttons/circle.png"));
    ui -> fillButton        -> setIcon(QIcon(":/buttons/fill.png"));
    ui -> colorButton       -> setIcon(QIcon(":/buttons/color.png"));
    ui -> addFrameButton    -> setIcon(QIcon(":/buttons/add_frame.png"));
    ui -> panUpButton       -> setIcon(QIcon(":/buttons/up_arrow.png"));
    ui -> panDownButton     -> setIcon(QIcon(":/buttons/down_arrow.png"));
    ui -> panLeftButton     -> setIcon(QIcon(":/buttons/left_arrow.png"));
    ui -> panRightButton    -> setIcon(QIcon(":/buttons/right_arrow.png"));
    ui -> deleteFrameButton -> setIcon(QIcon(":/buttons/remove_frame.png"));

    // keyboard shortcuts
    QAction *undoShortcut = new QAction(this);
    undoShortcut->setShortcut(Qt::CTRL | Qt::Key_Z);
    connect(undoShortcut,
            &QAction::triggered,
            &model,
            &pixelEditorModel::undo);
    this->addAction(undoShortcut);

    QAction *redoShortcut = new QAction(this);
    redoShortcut->setShortcut(Qt::CTRL | Qt::Key_Y);
    connect(redoShortcut,
            &QAction::triggered,
            &model,
            &pixelEditorModel::redo);
    this->addAction(redoShortcut);

//    QAction *saveShortcut = new QAction(this);
//    saveShortcut->setShortcut(Qt::CTRL | Qt::Key_S);
//    connect(saveShortcut,
//            &QAction::triggered,
//            &model,
//            &pixelEditorModel::save);
//    this->addAction(saveShortcut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::valueChanged()
{
    update();
}

void MainWindow::changeCanvasView(float focusOnSpriteX, float focusOnSpriteY, int newScale)
{
    scale = newScale;
    focusSpriteCenterx = focusOnSpriteX;
    focusSpriteCentery = focusOnSpriteY;
    xOffset = canvasCenterx - (focusSpriteCenterx * scale);
    yOffset = canvasCentery - (focusSpriteCentery * scale);
}

/// takes in x,y canvas mouse points, and turns them to x,y sprite points
bool MainWindow::checkInCanvas(int& x, int& y) {
    // move our window's 0,0 to the canvas 0,0
    if (x < ui->canvas->x() || y < ui->canvas->y() || x > ui->canvas->width() + ui->canvas->x() || y > ui->canvas->height() + ui->canvas->y()) {
        // we are clicking outside the canvas. Dont even calculate the sprite points
        return false;
    } else {
        x = (x - xOffset) / scale;
        y = (y - yOffset) / scale;
        if (x < 0 || y < 0 || x > loadedSprite.width || y > loadedSprite.height) {
            // we are checking if we are clicking outside the sprite
            return false;
        } else {
            return true;
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    print("mouse pressed");
    print(event->pos().x(), ",", event->pos().y());
    int x = event->pos().x();
    int y = event->pos().y();
    if (checkInCanvas(x, y)) {
        print(x, " onsprite ", y);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    print("mouse moved");
    print(event->pos().x(), ",", event->pos().y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    print("mouse released");
    print(event->pos().x(), ",", event->pos().y());
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPen pen(Qt::black);
    int penWidth =  1;
    pen.setWidth(penWidth);
    painter.setPen(pen);

    // draw the sprite
    for (size_t i = 0; i < loadedSprite.width; i++) {
        for (size_t j = 0; j < loadedSprite.height; j++) {
            int x = xOffset + (i * scale);
            int y = yOffset + (j * scale);
            QRect box(x, y, scale, scale);
            painter.fillRect(box, loadedSprite.getColor(i,j));//QColor(i*25,j*25,50));
        }
    }

    // crop out the sprite
    painter.fillRect(whiteOutBoxLeft, Qt::white);
    painter.fillRect(whiteOutBoxRight, Qt::white);
    painter.fillRect(whiteOutBoxTop, Qt::white);
    painter.fillRect(whiteOutBoxBottom, Qt::white);
    // add a black frame
    QRect frame(ui->canvas->x(), ui->canvas->y(), ui->canvas->width(), ui->canvas->height());
    painter.drawRect(frame);
}
