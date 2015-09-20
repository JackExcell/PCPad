#include <QMessageBox>
#include <QShowEvent>
#include <QThread>
#include <winerror.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    ui->NoControlLabel->hide();


    QThread* stateThread = new QThread;
    controller = new XinputStateWorker();
    controller->moveToThread(stateThread);
    connect(stateThread, SIGNAL(started()), controller, SLOT(processXinputState()));
    stateThread->start();

    //Connect button signals to controller test objects
    connect(controller, SIGNAL(NO_CONTROLLER()), ui->NoControlLabel, SLOT(show()));
    connect(controller, SIGNAL(CONTROLLER_DETECTED()), ui->NoControlLabel, SLOT(hide()));
    connect(controller, SIGNAL(A_BUTTON_PRESSED()), ui->PRESSED_A, SLOT(show()));
    connect(controller, SIGNAL(A_BUTTON_RELEASED()), ui->PRESSED_A, SLOT(hide()));
    connect(controller, SIGNAL(B_BUTTON_PRESSED()), ui->PRESSED_B, SLOT(show()));
    connect(controller, SIGNAL(B_BUTTON_RELEASED()), ui->PRESSED_B, SLOT(hide()));
    connect(controller, SIGNAL(X_BUTTON_PRESSED()), ui->PRESSED_X, SLOT(show()));
    connect(controller, SIGNAL(X_BUTTON_RELEASED()), ui->PRESSED_X, SLOT(hide()));
    connect(controller, SIGNAL(Y_BUTTON_PRESSED()), ui->PRESSED_Y, SLOT(show()));
    connect(controller, SIGNAL(Y_BUTTON_RELEASED()), ui->PRESSED_Y, SLOT(hide()));
    connect(controller, SIGNAL(LB_BUTTON_PRESSED()), ui->PRESSED_LB, SLOT(show()));
    connect(controller, SIGNAL(LB_BUTTON_RELEASED()), ui->PRESSED_LB, SLOT(hide()));
    connect(controller, SIGNAL(RB_BUTTON_PRESSED()), ui->PRESSED_RB, SLOT(show()));
    connect(controller, SIGNAL(RB_BUTTON_RELEASED()), ui->PRESSED_RB, SLOT(hide()));
    connect(controller, SIGNAL(LS_BUTTON_PRESSED()), ui->PRESSED_LS, SLOT(show()));
    connect(controller, SIGNAL(LS_BUTTON_RELEASED()), ui->PRESSED_LS, SLOT(hide()));
    connect(controller, SIGNAL(RS_BUTTON_PRESSED()), ui->PRESSED_RS, SLOT(show()));
    connect(controller, SIGNAL(RS_BUTTON_RELEASED()), ui->PRESSED_RS, SLOT(hide()));
    connect(controller, SIGNAL(START_BUTTON_PRESSED()), ui->PRESSED_START, SLOT(show()));
    connect(controller, SIGNAL(START_BUTTON_RELEASED()), ui->PRESSED_START, SLOT(hide()));
    connect(controller, SIGNAL(BACK_BUTTON_PRESSED()), ui->PRESSED_BACK, SLOT(show()));
    connect(controller, SIGNAL(BACK_BUTTON_RELEASED()), ui->PRESSED_BACK, SLOT(hide()));
    connect(controller, SIGNAL(UP_BUTTON_PRESSED()), ui->PRESSED_UP, SLOT(show()));
    connect(controller, SIGNAL(UP_BUTTON_RELEASED()), ui->PRESSED_UP, SLOT(hide()));
    connect(controller, SIGNAL(DOWN_BUTTON_PRESSED()), ui->PRESSED_DOWN, SLOT(show()));
    connect(controller, SIGNAL(DOWN_BUTTON_RELEASED()), ui->PRESSED_DOWN, SLOT(hide()));
    connect(controller, SIGNAL(LEFT_BUTTON_PRESSED()), ui->PRESSED_LEFT, SLOT(show()));
    connect(controller, SIGNAL(LEFT_BUTTON_RELEASED()), ui->PRESSED_LEFT, SLOT(hide()));
    connect(controller, SIGNAL(RIGHT_BUTTON_PRESSED()), ui->PRESSED_RIGHT, SLOT(show()));
    connect(controller, SIGNAL(RIGHT_BUTTON_RELEASED()), ui->PRESSED_RIGHT, SLOT(hide()));
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

