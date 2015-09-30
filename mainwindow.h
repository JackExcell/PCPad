#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QSystemTrayIcon>
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
    void InitSystemTray();
    void changeEvent(QEvent *e);

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
    void cursorSpeedChanged(); //UI update
    void scrollSpeedChanged(); //UI update

    //Triggered when enabled settings is toggled for instant enabling/disabling
    void enableToggled(bool toggleState);

    //Triggered when a setting is changed in the settings tab, allowing the apply button to be clicked.
    void settingsChanged();

    //Specific triggers to different settings UI elements.

    void deadzonesHelp();

    //Deadzone value label updates
    void updateDeadzoneDisplay_LSX();
    void updateDeadzoneDisplay_LSY();
    void updateDeadzoneDisplay_RSX();
    void updateDeadzoneDisplay_RSY();

    //Saves the current settings to the .ini file
    void saveSettings();

    //Tray icon functions
    void maximiseFromTray();
    void maximiseFromTray(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    XINPUT_STATE state;
    XinputStateWorker *controller;
    Simulator *sim;
    bool controlCheckShown;
    QString settingsFile;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
