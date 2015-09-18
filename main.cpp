#include "mainwindow.h"
#include "controller.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //MessageBox shows warning if no controller is detected
    Controller c;
    QMessageBox noControllerWarning;

    if(!c.isControllerConnected(0))
    {
        noControllerWarning.setWindowTitle("Warning");
        noControllerWarning.setText("No compatible controller was detected.\n\n"
                                    "Please ensure your Xbox controller is plugged into a USB port.");
        noControllerWarning.setIcon(QMessageBox::Warning);
        noControllerWarning.exec();
    }

    return a.exec();
}
