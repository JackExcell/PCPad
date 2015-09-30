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
    void RightClickUp();
    void RightClickDown();
    void MouseMove(int xSpeed, int ySpeed);
    void MouseScroll(int xAxis, int yAxis);
    bool PCPadEnabled();
    int getCursorSpeed();
    int getScrollSpeed();

private:
    bool leftClickDown;
    bool rightClickDown;
    QString settingsFile;
    XinputStateWorker *control_ptr;
};

#endif // SIMULATOR_H
