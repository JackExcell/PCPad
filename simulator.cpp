#include <windows.h>
#include <QDebug>
#include <QSettings>
#include "simulator.h"


Simulator::Simulator(XinputStateWorker *controller, QString settingsFilePath)
{
    settingsFile = settingsFilePath;

    control_ptr = controller;
    connect(control_ptr, SIGNAL(A_BUTTON_PRESSED()), this, SLOT(LeftClickDown()));
    connect(control_ptr, SIGNAL(A_BUTTON_RELEASED()), this, SLOT(LeftClickUp()));
    connect(control_ptr, SIGNAL(LS_VALUES(int,int)), this, SLOT(MouseMove(int,int)));
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
