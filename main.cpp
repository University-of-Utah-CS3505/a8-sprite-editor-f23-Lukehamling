/*
    Team:       Coders for Christ
    Authors:    Ryan Dalrymple, Vincentio Dane, Luke Hamling, August O'Rourke
    Class:      CS3505
    Assignment: 8 - Sprite Editor
*/

#include "mainwindow.h"
#include "pixelEditorModel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PixelEditorModel model;
    MainWindow w(model);
    w.show();
    return a.exec();
}
