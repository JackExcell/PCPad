#include <windows.h>
#include <QDebug>
#include <QSettings>
#include "simulator.h"

#define MOUSEEVENTF_HWHEEL 0x01000

//This class simulates windows mouse and keyboard input by sending an
//INPUT struct to the operating system.
Simulator::Simulator(XinputStateWorker *controller, QString settingsFilePath)
{
    settingsFile = settingsFilePath;
    control_ptr = controller;

    connect(control_ptr, SIGNAL(A_BUTTON_PRESSED()), this, SLOT(LeftClickDown()));
    connect(control_ptr, SIGNAL(A_BUTTON_RELEASED()), this, SLOT(LeftClickUp()));
    connect(control_ptr, SIGNAL(B_BUTTON_PRESSED()), this, SLOT(RightClickDown()));
    connect(control_ptr, SIGNAL(B_BUTTON_RELEASED()), this, SLOT(RightClickUp()));
    connect(control_ptr, SIGNAL(LS_VALUES(int,int)), this, SLOT(MouseMove(int,int)));
    connect(control_ptr, SIGNAL(RS_VALUES(int,int)), this, SLOT(MouseScroll(int,int)));

    //Variable initialisation
    leftClickDown = false;
    rightClickDown = false;

}

Simulator::~Simulator()
{

}

//Simulates left mouse button down at the cursors current position
void Simulator::LeftClickDown()
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    if(!leftClickDown)
    {
        INPUT Input={0}; // Input struct

        //Specify input type
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;

        //Send input
        SendInput(1,&Input,sizeof(INPUT));

        leftClickDown = true;
    }
}

//Simulates releasing the left mouse button if the left mouse button is currently is the down state
void Simulator::LeftClickUp()
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    if(leftClickDown)
    {
        INPUT    Input={0}; // Input struct

        //Specify input type
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;

        //Send input
        SendInput(1,&Input,sizeof(INPUT));

        leftClickDown = false;
    }
}

//Adjusts the current mouse cursor position based on input from the left analog stick.
void Simulator::MouseMove(int xSpeed, int ySpeed)
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    //Convert values from analog stick movement to something more suitable for our function,
    //otherwise the cursor will move much too quickly
    int xSpeedNormalised;
    int ySpeedNormalised;

    int speed = getCursorSpeed();

    switch(speed)
    {
        case 1:
        //Slow
        xSpeedNormalised = xSpeed/25;
        ySpeedNormalised = ySpeed/25;
        break;

        case 2:
        //Medium
        xSpeedNormalised = xSpeed/14;
        ySpeedNormalised = ySpeed/14;
        break;

        case 3:
        //Fast
        xSpeedNormalised = xSpeed/8;
        ySpeedNormalised = ySpeed/8;
        break;

        case 4:
        //Super Fast
        xSpeedNormalised = xSpeed/4;
        ySpeedNormalised = ySpeed/4;
        break;
    }

    INPUT Input={0}; // Input struct

    Input.mi.mouseData = 0;
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;

    POINT currentMousePos;
    GetCursorPos(&currentMousePos);

    double screenWidth = GetSystemMetrics(SM_CXSCREEN) -1;
    double screenHeight = GetSystemMetrics(SM_CYSCREEN) -1;

    LONG newX = (currentMousePos.x + xSpeedNormalised ) * (65535.0f/screenWidth);
    LONG newY = (currentMousePos.y + ySpeedNormalised ) * (65535.0f/screenHeight);

    Input.mi.dx = newX;
    Input.mi.dy = newY;
    SendInput(1,&Input,sizeof(INPUT));
}

void Simulator::MouseScroll(int xAxis, int yAxis)
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    int speedSetting = getScrollSpeed();

    int xSpeed = (xAxis/4)*speedSetting;
    int ySpeed = (yAxis/4)*speedSetting;

    //X-axis
    //Sending both X-axis and Y-axis scrolling at the same time causes neither axis to work properly.
    //Both work fine on their own, this is possibly due to MOUSEEVENTF_HWHEEL and MOUSEEVENTF_WHEEL
    //causing some kind of conflict. Needs further investigation. For now, scrolling only works on the
    //Y-axis.


    //INPUT Input_X={0}; // Input struct

    //Input_X.type = INPUT_MOUSE;
    //Input_X.mi.dwFlags = MOUSEEVENTF_HWHEEL;
    //Input_X.mi.mouseData = xSpeed;

    //Y-axis
    INPUT Input_Y={0}; // Input struct

    Input_Y.type = INPUT_MOUSE;
    Input_Y.mi.dwFlags = MOUSEEVENTF_WHEEL;
    Input_Y.mi.mouseData = ySpeed;


    //SendInput(1,&Input_X,sizeof(INPUT));
    SendInput(1,&Input_Y, sizeof(INPUT));
}

bool Simulator::leftClickDownCheck()
{
    return leftClickDown;
}

bool Simulator::PCPadEnabled()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    bool enabled = settings.value("general/enabled").toBool();
    return enabled;
}

int Simulator::getCursorSpeed()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    int speed = settings.value("cursor/speed").toInt();
    return speed;
}

int Simulator::getScrollSpeed()
{
    QSettings settings(settingsFile, QSettings::IniFormat);
    int speed = settings.value("cursor/scrollspeed").toInt();
    return speed;
}

void Simulator::RightClickDown()
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    if(!rightClickDown)
    {
        INPUT Input={0}; // Input struct

        //Specify input type
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN;

        //Send input
        SendInput(1,&Input,sizeof(INPUT));

        rightClickDown = true;
    }
}

void Simulator::RightClickUp()
{
    if(control_ptr->isConnected != ERROR_SUCCESS || !PCPadEnabled())
    {
        return;
    }

    if(rightClickDown)
    {
        INPUT    Input={0}; // Input struct

        //Specify input type
        Input.type      = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;

        //Send input
        SendInput(1,&Input,sizeof(INPUT));

        rightClickDown = false;
    }
}
