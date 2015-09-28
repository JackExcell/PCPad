#include <QDebug>
#include <QThread>
#include <QSettings>
#include <winerror.h>
#include <windows.h>
#include <math.h>
#include "xinputstateworker.h"


XinputStateWorker::XinputStateWorker(QString settingsFilePath)
{
    settingsFile = settingsFilePath;
}

XinputStateWorker::~XinputStateWorker()
{

}

//QSettings settings;


//This process will be constantly repeated while the program runs
void XinputStateWorker::processXinputState() {

   // QSettings settings(settingsFile, QSettings::IniFormat);
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

        //Left trigger
        LTfloat = (float) state.Gamepad.bLeftTrigger / 255; //Translates value from 0-255 byte to 0-1 float
        LT = LTfloat * 100;
        emit LT_VALUE(LT);
        if(LT > 10)
        {
            emit LT_BUTTON_PRESSED();
        }
        else
        {
            emit LT_BUTTON_RELEASED();
        }

        //Right trigger
        RTfloat = (float) state.Gamepad.bRightTrigger / 255; //Translates value from 0-255 byte to 0-1 float
        RT = RTfloat * 100;
        emit RT_VALUE(RT);
        if(RT > 10)
        {
            emit RT_BUTTON_PRESSED();
        }
        else
        {
            emit RT_BUTTON_RELEASED();
        }

        //Get Deadzone information from settings for use with analog stick movements
        QSettings settings(settingsFile, QSettings::IniFormat);
        int dz_LSX = settings.value("deadzones/LSX").toInt();
        int dz_LSY = settings.value("deadzones/LSY").toInt();
        int dz_RSX = settings.value("deadzones/RSX").toInt();
        int dz_RSY = settings.value("deadzones/RSY").toInt();
        int dz_LSX_negative = dz_LSX*-1;
        int dz_LSY_negative = dz_LSY*-1;
        int dz_RSX_negative = dz_RSX*-1;
        int dz_RSY_negative = dz_RSY*-1;

        //Left stick
        LSfloat_X = (float) state.Gamepad.sThumbLX / 32767; //Translates to 0-1 float value
        LS_X = LSfloat_X * 100;
        LSfloat_Y = (float) state.Gamepad.sThumbLY / 32767; //Translates to 0-1 float value
        LS_Y = LSfloat_Y * 100;

        //Correct inverted Y-axis
        if(LS_Y > 0)
        {
            LS_Y *= -1; //Make negative if positive
        }
        else if(LS_Y < 0)
        {
            LS_Y -= LS_Y *2; //Make positive if negative
        }

        //Check if joystick is in dead zone
        if((LS_X > dz_LSX || LS_X < dz_LSX_negative) || (LS_Y > dz_LSY || LS_Y < dz_LSY_negative))
        {
            if(LS_X < dz_LSX && LS_X > dz_LSX_negative)
            {
                LS_X = 0;
            }

            if(LS_Y < dz_LSY && LS_Y > dz_LSY_negative)
            {
                LS_Y = 0;
            }

            emit LS_VALUES(LS_X, LS_Y);
        }
        else
        {
            emit LS_DEADZONE();
        }

        //Right stick
        RSfloat_X = (float) state.Gamepad.sThumbRX / 32767; //Translates to 0-1 float value
        RS_X = RSfloat_X * 100;
        RSfloat_Y = (float) state.Gamepad.sThumbRY / 32767; //Translates to 0-1 float value
        RS_Y = RSfloat_Y * 100;

        //Correct inverted Y-axis
        if(RS_Y > 0)
        {
            RS_Y *= -1; //Make negative if positive
        }
        else if(RS_Y < 0)
        {
            RS_Y -= RS_Y *2; //Make positive if negative
        }

        //Check if joystick is in dead zone
        if((RS_X > dz_RSX || RS_X < dz_RSX_negative) || (RS_Y > dz_RSY || RS_Y < dz_RSY_negative))
        {
            if(RS_X < dz_RSX && RS_X > dz_RSX_negative)
            {
                RS_X = 0;
            }

            if(RS_Y < dz_RSY && RS_Y > dz_RSY_negative)
            {
                RS_Y = 0;
            }

            emit RS_VALUES(RS_X, RS_Y);
        }
        else
        {
            emit RS_DEADZONE();
        }

        Sleep(18); // 18 millisecond delay means loop executes approximately 55 times per second.
        //A value of 16 which would effectively give near perfect 60fps cursor movement was causing issues
        //where the cursor appeared to skip a few pixels every half a second or so, need to investigate
        //this further.
    }
}

XINPUT_STATE XinputStateWorker::getState()
{
    return state;
}
