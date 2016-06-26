
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

    /*!
     * When StartButton is clicked, the form fields are locked.
     * It sent a signal to start the model algorithm
     */
    void on_StartButton_clicked();

    /*!
     * When PauseButton is clicked, the form fields are locked.
     * Button text is changed and send pause signal to model
     */
    void on_PauseButton_clicked();

    /*!
     * When CancelButton is clicked,
     * the form fields are unlocked,
     * and send signal to model
     */
    void on_CancelButton_clicked();

    /*!
     * Slot receives a signal from the model,
     * and resets the current page status in the view
     */
    void pageInfoIsChanged(Node* page);


    /*!
     * The slot receives a signal from the model algorithm is completed,
     *  and sets the information in a view
     */
    void searchIsComplete();

private:
    Ui::MainWindow *m_Ui;


};

#endif // MAINWINDOW_H
