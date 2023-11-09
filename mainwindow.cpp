#include "mainwindow.h"
#include "pixelEditorModel.h"
#include "ui_mainwindow.h"

#include <string>
#include <QPainter>
#include <QPen>
#include <QShortcut>

using namespace std;
void print(int num1, string toPrint, int num2) {
    qDebug() << num1 << toPrint << num2;
}

MainWindow::MainWindow(pixelEditorModel& model, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui -> undoButton -> setIcon(QIcon(":/buttons/undo.png"));
    ui -> redoButton -> setIcon(QIcon(":/buttons/redo.png"));
    ui -> saveButton -> setIcon(QIcon(":/buttons/save.png"));
    ui -> loadButton -> setIcon(QIcon(""));

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

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    QPen pen(Qt::black);
    int penWidth =  1;
    pen.setWidth(penWidth);
    painter.setPen(pen);

    const int scale = 20;
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            int x = ui->canvas->x() + (i * scale);
            int y = ui->canvas->y() + (j * scale);
            QRect box(x, y, scale, scale);
            painter.fillRect(box, QColor(i*25,j*25,50));
        }
    }

    QRect frame(ui->canvas->x(), ui->canvas->y(), ui->canvas->width(), ui->canvas->height());
//    pen.setColor(Qt::black);
    painter.drawRect(frame);
}
