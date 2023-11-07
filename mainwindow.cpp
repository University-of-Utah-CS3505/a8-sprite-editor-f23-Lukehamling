#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <QPainter>
#include <QPen>
using namespace std;
void print(int num1, string toPrint, int num2) {
    qDebug() << num1 << toPrint << num2;
}

MainWindow::MainWindow(QWidget* parent)
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
    QRect box(100, 170, 80, 60);
    painter.fillRect(box, Qt::blue);
    QRect frame(ui->canvas->x(), ui->canvas->y(), ui->canvas->width(), ui->canvas->height());
//    pen.setColor(Qt::black);
    painter.drawRect(frame);
}
