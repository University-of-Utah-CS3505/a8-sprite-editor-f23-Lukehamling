#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

using namespace std;

void print(int num1, string toPrint, int num2) {
    qDebug() << num1 << toPrint << num2;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    print(ui->canvas->x(),",",ui->canvas->y());
    print(ui->canvas->height(),",",ui->canvas->width());

    //QPaintEvent *paintEvent = new QPaintEvent();

    QPainter* painter = new QPainter(ui->canvas);
    painter->begin(this);
    QRect r(20,20,60,60);
    painter->pen().color().green();
    painter->drawRect(r);
    painter->end();
    update();
}

void CustomWidget::paintEvent(QPaintEvent* e)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

