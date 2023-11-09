#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pixelEditorModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(pixelEditorModel& model, QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);

public slots:
    void valueChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
