#include "mainwindow.h"
#include "pixelEditorModel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pixelEditorModel model;
    MainWindow w(model);
    w.show();
    return a.exec();
}
