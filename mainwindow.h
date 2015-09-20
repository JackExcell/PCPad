#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xinput.h>
#include "xinputstateworker.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void controlCheck();
    void showEvent(QShowEvent *);

private:
    Ui::MainWindow *ui;
    XINPUT_STATE state;
    XinputStateWorker *controller;

    bool controlCheckShown = false;
};

#endif // MAINWINDOW_H
