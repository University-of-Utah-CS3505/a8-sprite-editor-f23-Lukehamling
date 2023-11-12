#include "mainwindow.h"
#include "pixelEditorModel.h"
#include "ui_mainwindow.h"

#include <string>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QShortcut>
#include <QStringList>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>

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
    editorModel = &model;

    
    ui->FPSslider->setMaximum(60);
    ui->FPSslider->setMinimum(1);
    ui->FPSslider->setValue(24);
    ui->FPSLabel->setText("FPS 24");

    // Math out some x,y locations for later calculation
    const int canvasEdgeRight = ui->canvas->x() + ui->canvas->width();
    const int canvasEdgeBottom = ui->canvas->y() + ui->canvas->height();
    WhiteOutBoxLeft = QRect(0,0,ui->canvas->x(),this->height());
    WhiteOutBoxRight = QRect(canvasEdgeRight,0,this->width() - canvasEdgeRight,this->height());
    WhiteOutBoxTop = QRect(ui->canvas->x(),0,ui->canvas->width(),ui->canvas->y());
    WhiteOutBoxBottom = QRect(ui->canvas->x(),canvasEdgeBottom,ui->canvas->width(),this->height() - canvasEdgeBottom);
    CanvasCenterx = ui->canvas->x() + (ui->canvas->width() / 2);
    CanvasCentery = ui->canvas->y() + (ui->canvas->height() / 2);
    focusSpriteCenterx = 900; // draw a empty box out of the way
    focusSpriteCentery = 900; // these views are initialzied in startButtonClicked()
    scale = 1;
    updateCanvasView();// set the focus center and zoom/scale of our view

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
      
   QAction *saveShortcut = new QAction(this);
   saveShortcut->setShortcut(Qt::CTRL | Qt::Key_S);
   connect(saveShortcut,
           &QAction::triggered,
           this,
           &MainWindow::saveClicked);
   this->addAction(saveShortcut);

    QAction *panLeftShortcut = new QAction(this);
    panLeftShortcut->setShortcut(Qt::Key_Left);
    connect(panLeftShortcut,
            &QAction::triggered,
            this,
            &MainWindow::panLeft);
    this->addAction(panLeftShortcut);

    QAction *panUpShortcut = new QAction(this);
    panUpShortcut->setShortcut(Qt::Key_Up);
    connect(panUpShortcut,
            &QAction::triggered,
            this,
            &MainWindow::panUp);
    this->addAction(panUpShortcut);

    QAction *panRightShortcut = new QAction(this);
    panRightShortcut->setShortcut(Qt::Key_Right);
    connect(panRightShortcut,
            &QAction::triggered,
            this,
            &MainWindow::panRight);
    this->addAction(panRightShortcut);

    QAction *panDownShortcut = new QAction(this);
    panDownShortcut->setShortcut(Qt::Key_Down);
    connect(panDownShortcut,
            &QAction::triggered,
            this,
            &MainWindow::panDown);
    this->addAction(panDownShortcut);

    // save-load
    connect(&model,
            &pixelEditorModel::updateCanvasViewSignal,
            this,
            &MainWindow::updateCanvasView);
    connect(ui->saveButton,
            &QPushButton::clicked,
            this,
            &MainWindow::saveClicked);
    connect(this,
            &MainWindow::saveFileSelected,
            &model,
            &pixelEditorModel::save);
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadClicked);
    connect(this,
            &MainWindow::loadFileSelected,
            &model,
            &pixelEditorModel::load);
    connect(&model,
            &pixelEditorModel::createErrorMessagePopup,
            this,
            &MainWindow::createErrorMessagePopup);
    
    //fps slider
    connect(ui->FPSslider,
            &QSlider::valueChanged,
            &model,
            &pixelEditorModel::changeFPS);
    connect(ui->FPSslider,
            &QSlider::valueChanged,
            this,
            &MainWindow::updateFPSLabel);
    connect(ui->playButton,
            &QPushButton::pressed,
            &model,
            [&model](){model.setStopped(false);});
    connect(&model,
            &pixelEditorModel::showFrameSignal,
            this,
            &MainWindow::showFrame);
    connect(ui->stopButton,
            &QPushButton::pressed,
            &model,
            [&model](){model.setStopped(true);});

    // undo-redo & color select
    connect(ui->undoButton,
            &QPushButton::clicked,
            &model,
            &pixelEditorModel::undo);
    connect(ui->redoButton,
            &QPushButton::clicked,
            &model,
            &pixelEditorModel::redo);
    connect(ui->colorButton,
            &QPushButton::pressed,
            &model,
            &pixelEditorModel::selectColor);
    connect(ui->penButton,
            &QPushButton::clicked,
            this,
            &MainWindow::penButtonClicked);
    connect(ui->fillButton,
            &QPushButton::clicked,
            this,
            &MainWindow::fillButtonClicked);
    connect(ui->eraseButton,
            &QPushButton::clicked,
            this,
            &MainWindow::eraseButtonClicked);
    connect(ui->circleButton,
            &QPushButton::clicked,
            this,
            &MainWindow::circleButtonClicked);
    connect(ui->rectangleButton,
            &QPushButton::clicked,
            this,
            &MainWindow::rectangleButtonClicked);
    connect(this,
            &MainWindow::toolSelected,
            &model,
            &pixelEditorModel::updateSelectedTool);

      
    //Set connections for start up logic
    connect(ui->createButton,
            &QPushButton::clicked,
            this,
            &MainWindow::newSpriteScreen);
    connect(ui->startButton,
            &QPushButton::clicked,
            this,
            &MainWindow::startButtonClicked);

    //Create new sprite based on this size
    connect(this,
            &MainWindow::selectedSpriteSize,
            &model,
            &pixelEditorModel::createInitialSprite);
    connect(&model,
            &pixelEditorModel::setSpriteSizeAfterLoading,
            this,
            &MainWindow::calculateFocusCenter);
    connect(&model,
            &pixelEditorModel::setSpriteSizeAfterLoading,
            this,
            &MainWindow::mainScreen);

    //Connect view control buttons
    connect(ui->panUpButton,
            &QPushButton::clicked,
            this,
            &MainWindow::panUp);
    connect(ui->panLeftButton,
            &QPushButton::clicked,
            this,
            &MainWindow::panLeft);
    connect(ui->panRightButton,
            &QPushButton::clicked,
            this,
            &MainWindow::panRight);
    connect(ui->panDownButton,
            &QPushButton::clicked,
            this,
            &MainWindow::panDown);
    connect(&model,
            &pixelEditorModel::updateCanvas,
            this,
            &MainWindow::valueChanged);
    connect(ui->addFrameButton,
            &QPushButton::clicked,
            &model,
            &pixelEditorModel::addFrame);
    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            &model,
            &pixelEditorModel::deleteFrame);
    connect(&model,
            &pixelEditorModel::updateFrameBox,
            this,
            &MainWindow::changeFrameBox);
    connect(ui->frameSelector,
            &QComboBox::activated,
            &model,
            &pixelEditorModel::selectFrame);
    connect(ui->frameSelector,
            &QComboBox::activated,
            this,
            &MainWindow::valueChanged);

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

void MainWindow::updateCanvasView()
{
    xOffset = CanvasCenterx - (focusSpriteCenterx * scale);
    yOffset = CanvasCentery - (focusSpriteCentery * scale);
    update();
    qDebug() << "Scale:" << scale << ",X:" << focusSpriteCenterx << ",Y:" << focusSpriteCentery
             << ",XOff:" << xOffset << ",YOff" << yOffset;
}

bool MainWindow::checkInCanvas(int& x, int& y) {
    print("checkInCanvas");
    // move our window's 0,0 to the canvas 0,0
    if (x < ui->canvas->x() || y < ui->canvas->y() || x > ui->canvas->width() + ui->canvas->x() || y > ui->canvas->height() + ui->canvas->y()) {
        // we are clicking outside the canvas. Dont even calculate the sprite points
        return false;
    } else {
        Sprite* loadedSprite = editorModel->getSelectedSprite();
        x = (x - xOffset) / scale;
        y = (y - yOffset) / scale;
        if (x < 0 || y < 0 || x > loadedSprite->width || y > loadedSprite->height) { // todo crashes if start to draw off go on sprite then release off
            // we are checking if we are clicking outside the sprite
            return false;
        } else {
            return true;
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();
    if (checkInCanvas(x, y)) {
        editorModel->clickPixel(x,y);
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();
    if (checkInCanvas(x, y)) {
        editorModel->movePixel(x,y);
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();
    if (checkInCanvas(x, y)) {
        editorModel->releasePixel(x,y);
    }
    update();
}

void MainWindow::updateFPSLabel(int newFPS)
{
    ui->FPSLabel->setText("FPS: " + QString::number(newFPS));
}

void MainWindow::showFrame(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = pixmap.scaled(ui->animationPreview->height(),ui->animationPreview->width(), Qt::IgnoreAspectRatio);
    ui->animationPreview->setPixmap(pixmap);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    Sprite* loadedSprite = editorModel->getSelectedSprite();
//    print(loadedSprite->width, "painting", loadedSprite->height);
    QPainter painter(this);

    QPen pen(Qt::black);
    int penWidth =  1;
    pen.setWidth(penWidth);
    painter.setPen(pen);

    QRect spriteFrame(xOffset, yOffset, loadedSprite->width * scale, loadedSprite->height * scale);
    painter.drawRect(spriteFrame);

    // draw the sprite
    for (size_t i = 0; i < loadedSprite->width; i++) {
        for (size_t j = 0; j < loadedSprite->height; j++) {
            int x = xOffset + (i * scale);
            int y = yOffset + (j * scale);
            QRect box(x, y, scale, scale);
            painter.fillRect(box, loadedSprite->getColor(i,j));//QColor(i*25,j*25,50));
        }
    }

    // crop out the sprite
    painter.fillRect(WhiteOutBoxLeft, Qt::white);
    painter.fillRect(WhiteOutBoxRight, Qt::white);
    painter.fillRect(WhiteOutBoxTop, Qt::white);
    painter.fillRect(WhiteOutBoxBottom, Qt::white);
    // add a black frame
    QRect canvasFrame(ui->canvas->x(), ui->canvas->y(), ui->canvas->width(), ui->canvas->height());
    painter.drawRect(canvasFrame);
}

void MainWindow::panUp()
{
    focusSpriteCentery -= editorModel->spriteHeight / 4;
    updateCanvasView();
}

void MainWindow::panDown()
{
    focusSpriteCentery += editorModel->spriteHeight / 4;
    updateCanvasView();
}

void MainWindow::panLeft()
{
    focusSpriteCenterx -= editorModel->spriteWidth / 4;
    updateCanvasView();
}

void MainWindow::panRight()
{
    focusSpriteCenterx += editorModel->spriteWidth / 4;
    updateCanvasView();
}

void MainWindow::saveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    "/home/",
                                                    tr("Sprites (*.ssp)"));
    emit saveFileSelected(fileName);
}

void MainWindow::loadClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "/home/",
                                                    tr("Sprites (*.ssp)"));

    if(!fileName.isEmpty())
        emit loadFileSelected(fileName);
}

void MainWindow::createErrorMessagePopup(QString windowTitle, QString errorMessage)
{
    QWidget popUp;
    QMessageBox::warning(&popUp, windowTitle, errorMessage);
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
    ui->playButton          ->setEnabled(false);
    ui->stopButton          ->setEnabled(false);

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
    ui->playButton              ->hide();
    ui->stopButton              ->hide();
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
    unsigned short int x = 32;
    unsigned short int y = 32;

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
    calculateFocusCenter(x, y);
    updateCanvasView();
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
    ui->playButton          ->setEnabled(true);
    ui->stopButton          ->setEnabled(true);
    ui->startButton         ->setEnabled(false);
    ui->spriteSizeComboBox  ->setEnabled(false);
    ui->createButton        ->setEnabled(false);
    ui->createNewOrLoadLabel->setEnabled(false);

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
    ui->playButton              ->show();
    ui->stopButton              ->show();
    ui->spriteSizeSelectorLabel ->hide();
    ui->startButton             ->hide();
    ui->spriteSizeComboBox      ->hide();
    ui->createButton            ->hide();
    ui->createNewOrLoadLabel    ->hide();
    ui->frameSelector           ->addItem("Frame 1");

    ui->loadButton              ->setGeometry(260, 18, 75, 25);
}

void MainWindow::changeFrameBox(int data)
{
    if (data > 0)
    {
        qDebug() << "called correct signal";
        ui->frameSelector->addItem("Frame " + QString::number(data));
    }
    //this is the case where someone deletes a frame, but there is only one frame
    else if (data == 0)
    {
        update();
    }
    else
    {
        int index = ui->frameSelector->currentIndex();
        ui->frameSelector->setCurrentIndex(index - 1);
        update();
        ui->frameSelector->removeItem(ui->frameSelector->count() - 1);
    }
}

void MainWindow::calculateFocusCenter(unsigned short int width, unsigned short int height)
{
    int largeSide = width;
    if (height > width)
        largeSide = height;
    scale = ui->canvas->height() / largeSide;
    if (scale < 1) {
        scale = 1;
    }
    focusSpriteCenterx = width / 2;
    focusSpriteCentery = height / 2;
    qDebug() << "Scale:" << scale << ",X:" << focusSpriteCenterx << ",Y:" << focusSpriteCentery;
}


void MainWindow::fillButtonClicked()
{
    emit toolSelected(4);
}

void MainWindow::eraseButtonClicked()
{
    emit toolSelected(1);
}

void MainWindow::penButtonClicked()
{
    emit toolSelected(0);
}

void MainWindow::circleButtonClicked()
{
    emit toolSelected(2);
}

void MainWindow::rectangleButtonClicked()
{
    emit toolSelected(3);
}
