#include <QMessageBox>
#include <QShowEvent>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QTimer>
#include <QIcon>
#include <QMenu>
#include <winerror.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simulator.h"

//Initialise simulator class which will simulate mouse and keyboard events
//Simulator sim;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //UI Preparation
    this->setWindowIcon(QIcon(":/icons/resources/pcpadicon.ico"));
    ui->setupUi(this);

    //Variable initialisation
    controlCheckShown = false;

    //Define location of setting .ini file
    settingsFile = QApplication::applicationDirPath() + "/PCPadSettings.ini";

    //Hide button highlights on the Controller test tab.
    //These are shown later when button input is detected.
    ui->PRESSED_A->hide();
    ui->PRESSED_B->hide();
    ui->PRESSED_X->hide();
    ui->PRESSED_Y->hide();
    ui->PRESSED_LB->hide();
    ui->PRESSED_RB->hide();
    ui->PRESSED_LS->hide();
    ui->PRESSED_RS->hide();
    ui->PRESSED_START->hide();
    ui->PRESSED_BACK->hide();
    ui->PRESSED_UP->hide();
    ui->PRESSED_DOWN->hide();
    ui->PRESSED_LEFT->hide();
    ui->PRESSED_RIGHT->hide();
    ui->PRESSED_LT->hide();
    ui->PRESSED_RT->hide();
    ui->NoControlLabel->hide();

    //end of UI preparation

    //Initialise separate thread which will continously retrieve the state from the controller
    //and emit the appropriate signals based on the current controller input.
    QThread* stateThread = new QThread;
    controller = new XinputStateWorker(settingsFile);
    controller->moveToThread(stateThread);
    connect(stateThread, SIGNAL(started()), controller, SLOT(processXinputState()));
    stateThread->start();

    //Initialise separate thread for input simulations. SendInput functions need to be on a separate
    //thread to GUI functions to prevent errors with the windows message loop when clicking on the
    //main window using an Xbox controller.
    QThread*  simThread = new QThread;
    sim = new Simulator(controller, settingsFile);
    sim->moveToThread(simThread);
    simThread->start();

    //Connect button signals to indication slots on controller test tab
    connectButtonIndicators();

    connectSettingsEvents();

    InitSystemTray();

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::controlCheck()
{
    //MessageBox shows warning if no controller is detected
    QMessageBox noControllerWarning;

    if(controller->isConnected != ERROR_SUCCESS)
    {
        noControllerWarning.setWindowTitle("PCPad: Warning");
        noControllerWarning.setText("No compatible controller was detected.\n\n"
                                    "Please ensure your Xbox controller is plugged into a USB port.");
        noControllerWarning.setIcon(QMessageBox::Warning);
        noControllerWarning.exec();
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    if(!controlCheckShown)
    {
        controlCheck();
        controlCheckShown = true;
    }
}

//Moves the red dot which shows the position of the left stick
void MainWindow::setLSdot(int x, int y)
{
    ui->LS_dot->setGeometry(62+(x/2), 378+(y/2), 7, 7);
}

//Sets the left red dot back to its default position
void MainWindow::setLSdeadZone()
{
    ui->LS_dot->setGeometry(62, 378, 7, 7);
}

//Moves the red dot which shows the position of the right stick
void MainWindow::setRSdot(int x, int y)
{
    ui->RS_dot->setGeometry(254+(x/2), 378+(y/2), 7, 7);
}

//Sets the right red dot back to its default position
void MainWindow::setRSdeadZone()
{
    ui->RS_dot->setGeometry(254, 378, 7, 7);
}

//Connect button signals to controller test objects, this allows UI elements to
//respond to the signals emitted by the xinputstateworker thread.
void MainWindow::connectButtonIndicators()
{
    //Controller detection
    connect(controller, SIGNAL(NO_CONTROLLER()), ui->NoControlLabel, SLOT(show()));
    connect(controller, SIGNAL(CONTROLLER_DETECTED()), ui->NoControlLabel, SLOT(hide()));

    //Letter buttons
    connect(controller, SIGNAL(A_BUTTON_PRESSED()), ui->PRESSED_A, SLOT(show()));
    connect(controller, SIGNAL(A_BUTTON_RELEASED()), ui->PRESSED_A, SLOT(hide()));
    connect(controller, SIGNAL(B_BUTTON_PRESSED()), ui->PRESSED_B, SLOT(show()));
    connect(controller, SIGNAL(B_BUTTON_RELEASED()), ui->PRESSED_B, SLOT(hide()));
    connect(controller, SIGNAL(X_BUTTON_PRESSED()), ui->PRESSED_X, SLOT(show()));
    connect(controller, SIGNAL(X_BUTTON_RELEASED()), ui->PRESSED_X, SLOT(hide()));
    connect(controller, SIGNAL(Y_BUTTON_PRESSED()), ui->PRESSED_Y, SLOT(show()));
    connect(controller, SIGNAL(Y_BUTTON_RELEASED()), ui->PRESSED_Y, SLOT(hide()));

    //Shoulder/bumper buttons
    connect(controller, SIGNAL(LB_BUTTON_PRESSED()), ui->PRESSED_LB, SLOT(show()));
    connect(controller, SIGNAL(LB_BUTTON_RELEASED()), ui->PRESSED_LB, SLOT(hide()));
    connect(controller, SIGNAL(RB_BUTTON_PRESSED()), ui->PRESSED_RB, SLOT(show()));
    connect(controller, SIGNAL(RB_BUTTON_RELEASED()), ui->PRESSED_RB, SLOT(hide()));

    //Analog sticks (when pressed in as buttons)
    connect(controller, SIGNAL(LS_BUTTON_PRESSED()), ui->PRESSED_LS, SLOT(show()));
    connect(controller, SIGNAL(LS_BUTTON_RELEASED()), ui->PRESSED_LS, SLOT(hide()));
    connect(controller, SIGNAL(RS_BUTTON_PRESSED()), ui->PRESSED_RS, SLOT(show()));
    connect(controller, SIGNAL(RS_BUTTON_RELEASED()), ui->PRESSED_RS, SLOT(hide()));

    //Start/back buttons
    connect(controller, SIGNAL(START_BUTTON_PRESSED()), ui->PRESSED_START, SLOT(show()));
    connect(controller, SIGNAL(START_BUTTON_RELEASED()), ui->PRESSED_START, SLOT(hide()));
    connect(controller, SIGNAL(BACK_BUTTON_PRESSED()), ui->PRESSED_BACK, SLOT(show()));
    connect(controller, SIGNAL(BACK_BUTTON_RELEASED()), ui->PRESSED_BACK, SLOT(hide()));

    //Directional pad
    connect(controller, SIGNAL(UP_BUTTON_PRESSED()), ui->PRESSED_UP, SLOT(show()));
    connect(controller, SIGNAL(UP_BUTTON_RELEASED()), ui->PRESSED_UP, SLOT(hide()));
    connect(controller, SIGNAL(DOWN_BUTTON_PRESSED()), ui->PRESSED_DOWN, SLOT(show()));
    connect(controller, SIGNAL(DOWN_BUTTON_RELEASED()), ui->PRESSED_DOWN, SLOT(hide()));
    connect(controller, SIGNAL(LEFT_BUTTON_PRESSED()), ui->PRESSED_LEFT, SLOT(show()));
    connect(controller, SIGNAL(LEFT_BUTTON_RELEASED()), ui->PRESSED_LEFT, SLOT(hide()));
    connect(controller, SIGNAL(RIGHT_BUTTON_PRESSED()), ui->PRESSED_RIGHT, SLOT(show()));
    connect(controller, SIGNAL(RIGHT_BUTTON_RELEASED()), ui->PRESSED_RIGHT, SLOT(hide()));

    //Triggers
    connect(controller, SIGNAL(LT_BUTTON_PRESSED()), ui->PRESSED_LT, SLOT(show()));
    connect(controller, SIGNAL(LT_BUTTON_RELEASED()), ui->PRESSED_LT, SLOT(hide()));
    connect(controller, SIGNAL(RT_BUTTON_PRESSED()), ui->PRESSED_RT, SLOT(show()));
    connect(controller, SIGNAL(RT_BUTTON_RELEASED()), ui->PRESSED_RT, SLOT(hide()));
    connect(controller, SIGNAL(LT_VALUE(int)), ui->LTBar, SLOT(setValue(int)));
    connect(controller, SIGNAL(RT_VALUE(int)), ui->RTBar, SLOT(setValue(int)));

    //Analog stick movement
    connect(controller, SIGNAL(LS_VALUES(int,int)), this, SLOT(setLSdot(int, int)));
    connect(controller, SIGNAL(LS_DEADZONE()), this, SLOT(setLSdeadZone()));
    connect(controller, SIGNAL(RS_VALUES(int,int)), this, SLOT(setRSdot(int, int)));
    connect(controller, SIGNAL(RS_DEADZONE()), this, SLOT(setRSdeadZone()));
}

void MainWindow::connectSettingsEvents()
{
    connect(ui->Enable_Checkbox, SIGNAL(toggled(bool)), this, SLOT(enableToggled(bool)));
    connect(ui->CursorSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->CursorSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(cursorSpeedChanged()));
    connect(ui->ScrollSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->ScrollSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(scrollSpeedChanged()));
    connect(ui->ApplyButton, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(ui->DeadzoneHelpButton, SIGNAL(clicked(bool)), this, SLOT(deadzonesHelp()));
    connect(ui->LSX_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->LSY_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->RSX_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->RSY_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->LSX_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDeadzoneDisplay_LSX()));
    connect(ui->LSY_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDeadzoneDisplay_LSY()));
    connect(ui->RSX_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDeadzoneDisplay_RSX()));
    connect(ui->RSY_DeadzoneSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDeadzoneDisplay_RSY()));
}

void MainWindow::mouseLeftDown()
{
    sim->LeftClickDown();
}

void MainWindow::mouseLeftUp()
{
    sim->LeftClickUp();
}

void MainWindow::mouseMovement(int xSpeed, int ySpeed)
{
    sim->MouseMove(xSpeed, ySpeed);
}

void MainWindow::enableToggled(bool toggleState)
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.setValue("general/enabled", toggleState);
    settings.sync();
}

void MainWindow::loadSettings()
{
    QSettings settings(settingsFile, QSettings::IniFormat);

    //Enabled
    bool enabled = settings.value("general/enabled").toBool();
    ui->Enable_Checkbox->setChecked(enabled);

    //Cursor speed
    int cursorSpeed = settings.value("cursor/speed").toInt();
    ui->CursorSpeedSlider->setValue(cursorSpeed);

    //Scroll speed
    int scrollSpeed = settings.value("cursor/scrollspeed").toInt();
    ui->ScrollSpeedSlider->setValue(scrollSpeed);

    //Deadzones
    int LSX = settings.value("deadzones/LSX").toInt();
    int LSY = settings.value("deadzones/LSY").toInt();
    int RSX = settings.value("deadzones/RSX").toInt();
    int RSY = settings.value("deadzones/RSY").toInt();
    ui->LSX_DeadzoneSlider->setValue(LSX);
    ui->LSY_DeadzoneSlider->setValue(LSY);
    ui->RSX_DeadzoneSlider->setValue(RSX);
    ui->RSY_DeadzoneSlider->setValue(RSY);

    ui->ApplyButton->setEnabled(false);
}

void MainWindow::saveSettings()
{
    QSettings settings(settingsFile, QSettings::IniFormat);

    //Cursor speed
    int cursorSpeed = ui->CursorSpeedSlider->value();
    settings.setValue("cursor/speed", cursorSpeed);

    //Scroll speed
    int scrollSpeed = ui->ScrollSpeedSlider->value();
    settings.setValue("cursor/scrollspeed", scrollSpeed);

    //Deadzones
    int deadzone_LSX = ui->LSX_DeadzoneSlider->value();
    int deadzone_LSY = ui->LSY_DeadzoneSlider->value();
    int deadzone_RSX = ui->RSX_DeadzoneSlider->value();
    int deadzone_RSY = ui->RSY_DeadzoneSlider->value();
    settings.setValue("deadzones/LSX", deadzone_LSX);
    settings.setValue("deadzones/LSY", deadzone_LSY);
    settings.setValue("deadzones/RSX", deadzone_RSX);    
    settings.setValue("deadzones/RSY", deadzone_RSY);

    ui->ApplyButton->setEnabled(false);
}

void MainWindow::settingsChanged()
{
    ui->ApplyButton->setEnabled(true);
}

void MainWindow::cursorSpeedChanged()
{
    int speed = ui->CursorSpeedSlider->value();

    switch(speed)
    {
    case 1:
        //Slow
        ui->CursorSpeedLabel->setText("Cursor Speed: Slow");
        break;
    case 2:
        //Medium
        ui->CursorSpeedLabel->setText("Cursor Speed: Medium");
        break;
    case 3:
        //Fast
        ui->CursorSpeedLabel->setText("Cursor Speed: Fast");
        break;
    case 4:
        //Super Fast
        ui->CursorSpeedLabel->setText("Cursor Speed: Super Fast");
        break;
    }
}

void MainWindow::scrollSpeedChanged()
{
    int speed = ui->ScrollSpeedSlider->value();

    switch(speed)
    {
    case 1:
        //Slow
        ui->ScrollSpeedLabel->setText("Scroll Speed: Slow");
        break;
    case 2:
        //Medium
        ui->ScrollSpeedLabel->setText("Scroll Speed: Medium");
        break;
    case 3:
        //Fast
        ui->ScrollSpeedLabel->setText("Scroll Speed: Fast");
        break;
    case 4:
        //Super Fast
        ui->ScrollSpeedLabel->setText("Scroll Speed: Super Fast");
        break;
    }
}

void MainWindow::deadzonesHelp()
{
    QMessageBox deadzoneHelp;
    deadzoneHelp.setWindowTitle("Deadzones");
    deadzoneHelp.setText("A Deadzone is the area in the centre position of the analog stick where"
                         " the stick is considered not in use. "
                         "Sometimes you may need to change these values depending on the application "
                         "you are using, or if your controller is old and the analogue sticks are worn out.\n\n"
                         "Increase these settings if the mouse cursor moves when the control is not "
                         "in use.");
    deadzoneHelp.setIcon(QMessageBox::Information);
    deadzoneHelp.exec();
}

void MainWindow::updateDeadzoneDisplay_LSX()
{
    QString value = QString::number(ui->LSX_DeadzoneSlider->value());
    QString valueString = QString("LS X-Axis: ").append(value).append("%");
    ui->LSX_Label->setText(valueString);
}

void MainWindow::updateDeadzoneDisplay_LSY()
{
    QString value = QString::number(ui->LSY_DeadzoneSlider->value());
    QString valueString = QString("LS Y-Axis: ").append(value).append("%");
    ui->LSY_Label->setText(valueString);
}

void MainWindow::updateDeadzoneDisplay_RSX()
{
    QString value = QString::number(ui->RSX_DeadzoneSlider->value());
    QString valueString = QString("RS X-Axis: ").append(value).append("%");
    ui->RSX_Label->setText(valueString);
}

void MainWindow::updateDeadzoneDisplay_RSY()
{
    QString value = QString::number(ui->RSY_DeadzoneSlider->value());
    QString valueString = QString("RSY X-Axis: ").append(value).append("%");
    ui->RSY_Label->setText(valueString);
}

void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type())
        {
            case QEvent::WindowStateChange:
                {
                    if (this->windowState() & Qt::WindowMinimized)
                    {
                        QTimer::singleShot(0, this, SLOT(hide()));
                    }
                    break;
                }

            default:
                break;
        }

        QMainWindow::changeEvent(e);
}

void MainWindow::InitSystemTray()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/icons/resources/pcpadicon.ico"), this);

    QAction *trayShow = new QAction( "Show PCPad Interface",trayIcon );
    QAction *trayQuit = new QAction( "Exit", trayIcon );

    connect(trayQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(trayShow, SIGNAL(triggered()), this, SLOT(maximiseFromTray()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(maximiseFromTray(QSystemTrayIcon::ActivationReason)));

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(trayQuit);
    trayMenu->addAction(trayShow);

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
}

void MainWindow::maximiseFromTray()
{
    this->showNormal();
    this->setFocus();
}

void MainWindow::maximiseFromTray(QSystemTrayIcon::ActivationReason reason)
{
    if(reason != QSystemTrayIcon::DoubleClick)
    {
        return;
    }
    else
    {
        this->showNormal();
        //this->setFocus();
        this->raise();
        this->activateWindow();
    }
}
