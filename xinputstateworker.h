#ifndef XINPUTSTATEWORKER_H
#define XINPUTSTATEWORKER_H

#include <xinput.h>
#include <QString>
#include <QObject>

class XinputStateWorker : public QObject
{
    Q_OBJECT

public:
    XinputStateWorker();
    ~XinputStateWorker();
    XINPUT_STATE getState();
    XINPUT_STATE state;
    DWORD isConnected;

public slots:
    void processXinputState();

signals:
    void NO_CONTROLLER();
    void CONTROLLER_DETECTED();
    void A_BUTTON_PRESSED();
    void A_BUTTON_RELEASED();
    void B_BUTTON_PRESSED();
    void B_BUTTON_RELEASED();
    void X_BUTTON_PRESSED();
    void X_BUTTON_RELEASED();
    void Y_BUTTON_PRESSED();
    void Y_BUTTON_RELEASED();
    void LB_BUTTON_PRESSED();
    void LB_BUTTON_RELEASED();
    void RB_BUTTON_PRESSED();
    void RB_BUTTON_RELEASED();
    void LS_BUTTON_PRESSED();
    void LS_BUTTON_RELEASED();
    void RS_BUTTON_PRESSED();
    void RS_BUTTON_RELEASED();
    void START_BUTTON_PRESSED();
    void START_BUTTON_RELEASED();
    void BACK_BUTTON_PRESSED();
    void BACK_BUTTON_RELEASED();
    void UP_BUTTON_PRESSED();
    void UP_BUTTON_RELEASED();
    void DOWN_BUTTON_PRESSED();
    void DOWN_BUTTON_RELEASED();
    void LEFT_BUTTON_PRESSED();
    void LEFT_BUTTON_RELEASED();
    void RIGHT_BUTTON_PRESSED();
    void RIGHT_BUTTON_RELEASED();


private:

};

#endif // XINPUTSTATEWORKER_H
