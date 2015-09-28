#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xinput.h>
#include "xinputstateworker.h"
#include "simulator.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    void controlCheck();
    void connectButtonIndicators();
    void connectSettingsEvents();
    void loadSettings();

public slots:
    //Red dot positioning
    void setLSdot(int x, int y); //Moves the red dot which shows the position of the left stick
    void setLSdeadZone(); //Sets the left red dot back to its default position
    void setRSdot(int x, int y); //Moves the red dot which shows the position of the right stick
    void setRSdeadZone(); //Sets the right red dot back to its default position

    //Mouse events
    void mouseLeftDown();
    void mouseLeftUp();
    void mouseMovement(int xSpeed, int ySpeed);

    //Triggered when enabled settings is toggled for instant enabling/disabling
    void enableToggled(bool toggleState);

    //Triggered when a setting is changed in the settings tab, allowing the apply button to be clicked.
    void settingsChanged();

    //Specific triggers to different settings UI elements.
    void cursorSpeedChanged();
    void deadzonesHelp();

    //Deadzone value label updates
    void updateDeadzoneDisplay_LSX();
    void updateDeadzoneDisplay_LSY();
    void updateDeadzoneDisplay_RSX();
    void updateDeadzoneDisplay_RSY();

    //Saves the current settings to the .ini file
    void saveSettings();

private:
    Ui::MainWindow *ui;
    XINPUT_STATE state;
    XinputStateWorker *controller;
    Simulator *sim;

    bool controlCheckShown = false;
    QString settingsFile;
};

#endif // MAINWINDOW_H
