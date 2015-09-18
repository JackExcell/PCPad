#include <windows.h>
#include "controller.h"

Controller::Controller()
{

}

//Returns true if an Xinput controller (such as an Xbox controller) is detected on specified port, false otherwise.
bool Controller::isControllerConnected(int port)
{
    if (XInputGetState(port, &state) == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
