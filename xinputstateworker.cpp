#include <QDebug>
#include <QThread>
#include <winerror.h>
#include "xinputstateworker.h"

XinputStateWorker::XinputStateWorker()
{

}

XinputStateWorker::~XinputStateWorker()
{

}

//Sleeper class to allow for millisecond delays, subclass of QThread
class Sleeper : public QThread
{
public:
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
};

//This process will be constantly repeated while the program runs
void XinputStateWorker::processXinputState() {

    while(true)
    {
        isConnected = (XInputGetState(0, &state));

        //Emit NO_CONTROLLER if no Xinput compatible controller is plugged in
        if(isConnected != ERROR_SUCCESS)
        {
            emit NO_CONTROLLER();
        }
        else
        {
            emit CONTROLLER_DETECTED();
        }

        //A button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0)
        {
            emit A_BUTTON_PRESSED();
        }
        else
        {
             emit A_BUTTON_RELEASED();
        }

        //B button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0)
        {
            emit B_BUTTON_PRESSED();
        }
        else
        {
             emit B_BUTTON_RELEASED();
        }

        //X button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0)
        {
            emit X_BUTTON_PRESSED();
        }
        else
        {
             emit X_BUTTON_RELEASED();
        }

        //Y button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0)
        {
            emit Y_BUTTON_PRESSED();
        }
        else
        {
             emit Y_BUTTON_RELEASED();
        }

        //LB button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0)
        {
            emit LB_BUTTON_PRESSED();
        }
        else
        {
             emit LB_BUTTON_RELEASED();
        }

        //RB button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0)
        {
            emit RB_BUTTON_PRESSED();
        }
        else
        {
             emit RB_BUTTON_RELEASED();
        }

        Sleeper::msleep(16); // 16 millisecond delay means loop executes 60 times per second

        //LS button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0)
        {
            emit LS_BUTTON_PRESSED();
        }
        else
        {
             emit LS_BUTTON_RELEASED();
        }

        //RS button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0)
        {
            emit RS_BUTTON_PRESSED();
        }
        else
        {
             emit RS_BUTTON_RELEASED();
        }

        //Start button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0)
        {
            emit START_BUTTON_PRESSED();
        }
        else
        {
             emit START_BUTTON_RELEASED();
        }

        //Back button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0)
        {
            emit BACK_BUTTON_PRESSED();
        }
        else
        {
             emit BACK_BUTTON_RELEASED();
        }

        //Up d-pad button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
        {
            emit UP_BUTTON_PRESSED();
        }
        else
        {
             emit UP_BUTTON_RELEASED();
        }

        //Down d-pad button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
        {
            emit DOWN_BUTTON_PRESSED();
        }
        else
        {
             emit DOWN_BUTTON_RELEASED();
        }

        //Left d-pad button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
        {
            emit LEFT_BUTTON_PRESSED();
        }
        else
        {
             emit LEFT_BUTTON_RELEASED();
        }

        //Right d-pad button
        if((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
        {
            emit RIGHT_BUTTON_PRESSED();
        }
        else
        {
             emit RIGHT_BUTTON_RELEASED();
        }
    }
}

XINPUT_STATE XinputStateWorker::getState()
{
    return state;
}
