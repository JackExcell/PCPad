#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QString>
#include "xinputstateworker.h"

class Simulator : public QObject
{
    Q_OBJECT

public:
    Simulator(XinputStateWorker *controller, QString settingsFilePath);
    ~Simulator();
    //Events

    //Button state checks
    bool leftClickDownCheck();

public slots:
    void LeftClickDown();
    void LeftClickUp();
    void MouseMove(int xSpeed, int ySpeed);
    bool PCPadEnabled();
    int getCursorSpeed();

private:
    bool leftClickDown = false;
    QString settingsFile;
    XinputStateWorker *control_ptr;
};

#endif // SIMULATOR_H
