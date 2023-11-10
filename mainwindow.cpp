#include "mainwindow.h"
#include "pixelEditorModel.h"
#include "ui_mainwindow.h"

#include <string>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QShortcut>
#include <QStringList>

using namespace std;
void print(int num1, string toPrint, int num2) {
    qDebug() << num1 << toPrint << num2;
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

    focusSpriteCenterx = 16;
    focusSpriteCentery = 16;



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

    connect(ui->colorButton,
            &QPushButton::pressed,
            &model,
            &pixelEditorModel::selectColor);

//    QAction *saveShortcut = new QAction(this);
//    saveShortcut->setShortcut(Qt::CTRL | Qt::Key_S);
//    connect(saveShortcut,
//            &QAction::triggered,
//            &model,
//            &pixelEditorModel::save);
//    this->addAction(saveShortcut);

    //Set connections for start up logic
    connect(ui->createButton,
            &QPushButton::clicked,
            this,
            &MainWindow::newSpriteScreen);
    connect(ui->startButton,
            &QPushButton::clicked,
            this,
            &MainWindow::startButtonClicked);
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadButtonClicked);

    //Create new sprite based on this size
    connect(this,
            &MainWindow::selectedSpriteSize,
            &model,
            &pixelEditorModel::createInitialSprite);

    setupStartScreen();
    populateSpriteSizeComboBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::valueChanged()
{
    update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    const int scale = 10;

    QPen pen(Qt::black);
    int penWidth =  1;
    pen.setWidth(penWidth);
    painter.setPen(pen);

    const int xOffset = canvasCenterx - (focusSpriteCenterx * scale);
    const int yOffset = canvasCentery - (focusSpriteCentery * scale);

    // draw the sprite
    for (size_t i = 0; i < loadedSprite.width; i++) {
        for (size_t j = 0; j < loadedSprite.height; j++) {
            //int x = ui->canvas->x() + (i * scale);
            //int y = ui->canvas->y() + (j * scale);
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

void MainWindow::setupStartScreen()
{
    ui->saveButton          ->setEnabled(false);
    ui->addFrameButton      ->setEnabled(false);
    ui->deleteFrameButton   ->setEnabled(false);
    ui->circleButton        ->setEnabled(false);
    ui->colorButton         ->setEnabled(false);
    ui->deleteFrameButton   ->setEnabled(false);
    ui->eraseButton         ->setEnabled(false);
    ui->fillButton          ->setEnabled(false);
    ui->panDownButton       ->setEnabled(false);
    ui->panLeftButton       ->setEnabled(false);
    ui->panRightButton      ->setEnabled(false);
    ui->panUpButton         ->setEnabled(false);
    ui->rectangleButton     ->setEnabled(false);
    ui->redoButton          ->setEnabled(false);
    ui->undoButton          ->setEnabled(false);
    ui->FPSLabel            ->setEnabled(false);
    ui->FPSslider           ->setEnabled(false);
    ui->animationPreview    ->setEnabled(false);
    ui->canvas              ->setEnabled(false);
    ui->statusbar           ->setEnabled(false);
    ui->penButton           ->setEnabled(false);
    ui->frameSelectorLabel  ->setEnabled(false);
    ui->frameSelector       ->setEnabled(false);
    ui->startButton         ->setEnabled(false);
    ui->spriteSizeComboBox  ->setEnabled(false);

    ui->saveButton              ->hide();
    ui->addFrameButton          ->hide();
    ui->deleteFrameButton       ->hide();
    ui->circleButton            ->hide();
    ui->colorButton             ->hide();
    ui->deleteFrameButton       ->hide();
    ui->eraseButton             ->hide();
    ui->fillButton              ->hide();
    ui->panDownButton           ->hide();
    ui->panLeftButton           ->hide();
    ui->panRightButton          ->hide();
    ui->panUpButton             ->hide();
    ui->rectangleButton         ->hide();
    ui->redoButton              ->hide();
    ui->undoButton              ->hide();
    ui->FPSLabel                ->hide();
    ui->FPSslider               ->hide();
    ui->animationPreview        ->hide();
    ui->canvas                  ->hide();
    ui->statusbar               ->hide();
    ui->penButton               ->hide();
    ui->frameSelectorLabel      ->hide();
    ui->frameSelector           ->hide();
    ui->spriteSizeSelectorLabel ->hide();
    ui->frameSelectorLabel      ->hide();
    ui->frameSelector           ->hide();
    ui->startButton             ->hide();
    ui->spriteSizeComboBox      ->hide();
}

void MainWindow::newSpriteScreen()
{
    ui->loadButton              ->setEnabled(false);
    ui->createButton            ->setEnabled(false);

    ui->loadButton              ->hide();
    ui->createButton            ->hide();
    ui->createNewOrLoadLabel    ->hide();


    ui->spriteSizeComboBox      ->setEnabled(true);
    ui->spriteSizeSelectorLabel ->setEnabled(true);
    ui->startButton             ->setEnabled(true);

    ui->spriteSizeComboBox      ->show();
    ui->spriteSizeSelectorLabel ->show();
    ui->startButton             ->show();
}

void MainWindow::populateSpriteSizeComboBox()
{
    QStringList sizeOptions{"32x32", "64x32", "32x64", "64x64", "128x64", "64x128", "128x128", "256x128", "128x256", "256x256", "512x256", "256x512", "512x512"};
    ui->spriteSizeComboBox->addItems(sizeOptions);
    ui->spriteSizeComboBox->setMaxVisibleItems(4); //TODO: Still displays 10 items?
}

void MainWindow::startButtonClicked()
{
    unsigned short int x = 0;
    unsigned short int y = 0;

    switch(ui->spriteSizeComboBox->currentIndex())
    {
    case 0:
        x = 32;
        y = 32;
        break;
    case 1:
        x = 64;
        y = 32;
        break;
    case 2:
        x = 32;
        y = 64;
        break;
    case 3:
        x = 64;
        y = 64;
        break;
    case 4:
        x = 128;
        y = 64;
        break;
    case 5:
        x = 64;
        y = 128;
        break;
    case 6:
        x = 128;
        y = 128;
        break;
    case 7:
        x = 256;
        y = 128;
        break;
    case 8:
        x = 128;
        y = 256;
        break;
    case 9:
        x = 256;
        y = 256;
        break;
    case 10:
        x = 512;
        y = 256;
        break;
    case 11:
        x = 256;
        y = 512;
        break;
    case 12:
        x = 512;
        y = 512;
        break;
    }
    emit selectedSpriteSize(x, y);

    mainScreen();
}

void MainWindow::mainScreen()
{
    ui->saveButton          ->setEnabled(true);
    ui->addFrameButton      ->setEnabled(true);
    ui->deleteFrameButton   ->setEnabled(true);
    ui->circleButton        ->setEnabled(true);
    ui->colorButton         ->setEnabled(true);
    ui->deleteFrameButton   ->setEnabled(true);
    ui->eraseButton         ->setEnabled(true);
    ui->fillButton          ->setEnabled(true);
    ui->panDownButton       ->setEnabled(true);
    ui->panLeftButton       ->setEnabled(true);
    ui->panRightButton      ->setEnabled(true);
    ui->panUpButton         ->setEnabled(true);
    ui->rectangleButton     ->setEnabled(true);
    ui->redoButton          ->setEnabled(true);
    ui->undoButton          ->setEnabled(true);
    ui->FPSLabel            ->setEnabled(true);
    ui->FPSslider           ->setEnabled(true);
    ui->animationPreview    ->setEnabled(true);
    ui->canvas              ->setEnabled(true);
    ui->statusbar           ->setEnabled(true);
    ui->penButton           ->setEnabled(true);
    ui->frameSelectorLabel  ->setEnabled(true);
    ui->frameSelector       ->setEnabled(true);
    ui->loadButton          ->setEnabled(true);
    ui->startButton         ->setEnabled(false);
    ui->spriteSizeComboBox  ->setEnabled(false);

    ui->saveButton              ->show();
    ui->addFrameButton          ->show();
    ui->deleteFrameButton       ->show();
    ui->circleButton            ->show();
    ui->colorButton             ->show();
    ui->deleteFrameButton       ->show();
    ui->eraseButton             ->show();
    ui->fillButton              ->show();
    ui->panDownButton           ->show();
    ui->panLeftButton           ->show();
    ui->panRightButton          ->show();
    ui->panUpButton             ->show();
    ui->rectangleButton         ->show();
    ui->redoButton              ->show();
    ui->undoButton              ->show();
    ui->FPSLabel                ->show();
    ui->FPSslider               ->show();
    ui->animationPreview        ->show();
    ui->canvas                  ->show();
    ui->statusbar               ->show();
    ui->penButton               ->show();
    ui->frameSelectorLabel      ->show();
    ui->frameSelector           ->show();
    ui->spriteSizeSelectorLabel ->show();
    ui->frameSelectorLabel      ->show();
    ui->frameSelector           ->show();
    ui->loadButton              ->show();
    ui->spriteSizeSelectorLabel ->hide();
    ui->startButton             ->hide();
    ui->spriteSizeComboBox      ->hide();

    ui->loadButton->setGeometry(260, 18, 75, 25);



    // TODO: remove
    // make a gradient grid for testing
    for (size_t i = 0; i < loadedSprite.width; i++) {
        for (size_t j = 0; j < loadedSprite.height; j++) {
            loadedSprite.setColor(i,j, QColor(i*7,j*7,i+j+80));
        }
    }
}

void MainWindow::loadButtonClicked()
{
    //TODO: Implement this method.
}
