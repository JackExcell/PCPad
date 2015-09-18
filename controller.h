#pragma once

#include <Xinput.h>

class Controller
{
public:
    Controller();
    bool isControllerConnected(int port);

private:
    XINPUT_STATE state;
};

