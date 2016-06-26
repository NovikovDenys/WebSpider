#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <model.h>
#include <searchengine.h>
#include <ui_mainwindow.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void initModelData(uint, uint, QString&, QString&);
    void pauseBtnClicked();
    void cancelOperation();
private slots:

    void on_StartButton_clicked();
    void on_PauseButton_clicked();
    void on_CancelButton_clicked();

    void pageInfoIsChanged(Node* page);
    void searchIsComplete();

private:
    Ui::MainWindow *m_Ui;


};

#endif // MAINWINDOW_H
