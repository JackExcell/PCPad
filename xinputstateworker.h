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
    //Controller detection
    void NO_CONTROLLER();
    void CONTROLLER_DETECTED();

    //Letter buttons
    void A_BUTTON_PRESSED();
    void A_BUTTON_RELEASED();
    void B_BUTTON_PRESSED();
    void B_BUTTON_RELEASED();
    void X_BUTTON_PRESSED();
    void X_BUTTON_RELEASED();
    void Y_BUTTON_PRESSED();
    void Y_BUTTON_RELEASED();

    //Shoulder/bumper buttons
    void LB_BUTTON_PRESSED();
    void LB_BUTTON_RELEASED();
    void RB_BUTTON_PRESSED();
    void RB_BUTTON_RELEASED();

    //Analog sticks (when pressed in like buttons)
    void LS_BUTTON_PRESSED();
    void LS_BUTTON_RELEASED();
    void RS_BUTTON_PRESSED();
    void RS_BUTTON_RELEASED();

    //Start and back buttons
    void START_BUTTON_PRESSED();
    void START_BUTTON_RELEASED();
    void BACK_BUTTON_PRESSED();
    void BACK_BUTTON_RELEASED();

    //Directional-pad buttons
    void UP_BUTTON_PRESSED();
    void UP_BUTTON_RELEASED();
    void DOWN_BUTTON_PRESSED();
    void DOWN_BUTTON_RELEASED();
    void LEFT_BUTTON_PRESSED();
    void LEFT_BUTTON_RELEASED();
    void RIGHT_BUTTON_PRESSED();
    void RIGHT_BUTTON_RELEASED();

    //Triggers
    void LT_BUTTON_PRESSED();
    void LT_BUTTON_RELEASED();
    void RT_BUTTON_PRESSED();
    void RT_BUTTON_RELEASED();
    void LT_VALUE(int LTvalue);
    void RT_VALUE(int RTvalue);

    //Analog stick movement
    void LS_VALUES(int LS_X, int LS_Y);
    void LS_DEADZONE();
    void RS_VALUES(int RS_X, int RS_Y);
    void RS_DEADZONE();

private:

    //Trigger values
    float LTfloat; //Value between 0 and 1 showing how much the left trigger is pressed in.
    float RTfloat; //Value between 0 and 1 showing how much the right trigger is pressed in.
    int LT; //0-100 Integer representation of LTfloat for QT widgets which require integer values.
    int RT; //0-100 Integer representation of RTfloat for QT widgets which require integer values.

    //Analog stick values
    float LSfloat_X; //Value between 0 and 1 showing the postion on the left stick on the X axis.
    float LSfloat_Y; //Value between 0 and 1 showing the postion on the left stick on the Y axis.
    float RSfloat_X; //Value between 0 and 1 showing the postion on the right stick on the X axis.
    float RSfloat_Y; //Value between 0 and 1 showing the postion on the right stick on the Y axis.
    int LS_X; //Integer Value between 0 and 100 showing the postion on the left stick on the X axis.
    int LS_Y; //Integer Value between 0 and 100 showing the postion on the left stick on the Y axis.
    int RS_X; //Integer Value between 0 and 100 showing the postion on the right stick on the X axis.
    int RS_Y; //Integer Value between 0 and 100 showing the postion on the right stick on the Y axis.


};

#endif // XINPUTSTATEWORKER_H
