#include <QMessageBox>
#include <QShowEvent>
#include <QThread>
#include <QDebug>
#include <winerror.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //UI Preparation
    ui->setupUi(this);

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
    controller = new XinputStateWorker();
    controller->moveToThread(stateThread);
    connect(stateThread, SIGNAL(started()), controller, SLOT(processXinputState()));
    stateThread->start();

    //Connect button signals to controller test objects, this allows UI elements to
    //respond to the signals emitted by the xinputstateworker thread.

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
