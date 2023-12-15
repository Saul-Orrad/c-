#include "EventHandler.h"
#include <QtWidgets/QApplication>
#include"FileIO.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{

   
 
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));
    EventHandler w;
    w.show();
    return a.exec();
   

}
