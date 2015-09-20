#pragma once

#include <Xinput.h>

class Controller
{
public:
    Controller();
    bool isControllerConnected(int port);
    XINPUT_STATE getState(int port);

private:
    XINPUT_STATE state;
};

