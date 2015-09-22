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

public slots:
    void setLSdot(int x, int y); //Moves the red dot which shows the position of the left stick
    void setLSdeadZone(); //Sets the left red dot back to its default position
    void setRSdot(int x, int y); //Moves the red dot which shows the position of the right stick
    void setRSdeadZone(); //Sets the right red dot back to its default position

private:
    Ui::MainWindow *ui;
    XINPUT_STATE state;
    XinputStateWorker *controller;

    bool controlCheckShown = false;
};

#endif // MAINWINDOW_H
