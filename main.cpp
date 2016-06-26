#include "mainwindow.h"
#include <QApplication>
#include <searchengine.h>
#include <model.h>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QString startURL("http://encom.kiev.ua/");

    Model *model = new Model();     //Create Model
    MainWindow w;                   //CreateMainWindow
    w.show();

    QObject::connect(&w, SIGNAL(initModelData(uint,uint,QString&,QString&)), model, SLOT(initModelData(uint,uint,QString&,QString&)));
    QObject::connect(&w, SIGNAL(pauseBtnClicked()), model, SLOT(pause()));
    QObject::connect(&w, SIGNAL(cancelOperation()), model, SLOT(cancelOperation()));

    QObject::connect(model, SIGNAL(pageIsChanged(Node*)), &w, SLOT(pageInfoIsChanged(Node*)));
    QObject::connect(model, SIGNAL(searchComplete()), &w, SLOT(searchIsComplete()));

    return a.exec();
}
