#include "mainwindow.h"

#define COLUMN_COUNT                2
#define URL_COLUMN_WIDTH            500

#define REG_EXP_HTTP                "(http[s]?:\\/\\/)([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?"

#define STRING_CANCEL               "Cancel"
#define STRING_COMPLETED            "Completed"
#define STRING_ERROR                "Error"
#define STRING_EXIT                 "Exit"
#define STRING_FOUND                "Found"
#define STRING_NOT_FOUND            "NotFound"
#define STRING_PAUSE                "Pause"
#define STRING_PROCESSING           "Processing"
#define STRING_RESUME               "Resume"
#define STRING_URL_CHECKED_DEADLOCK "Url checked Deadlock:"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);
    QRegExp exp(REG_EXP_HTTP);
    m_Ui->StartURL->setValidator(new QRegExpValidator(exp, this));
    m_Ui->tableWidget->setColumnCount(COLUMN_COUNT);
    m_Ui->tableWidget->setColumnWidth(0, URL_COLUMN_WIDTH);
    m_Ui->tableWidget->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

/* When StartButton is clicked, the form fields are locked.
 * It sent a signal to start the model algorithm
 */
void MainWindow::on_StartButton_clicked()
{
    if (m_Ui->StartURL->hasAcceptableInput() && !m_Ui->textEdit->toPlainText().isEmpty()) {
        m_Ui->StartURL->setDisabled(true);
        m_Ui->textEdit->setDisabled(true);

        m_Ui->UrlSpinBox->setDisabled(true);
        m_Ui->ThreadSpinBox->setDisabled(true);

        m_Ui->CancelButton->setText(STRING_CANCEL);
        m_Ui->tableWidget->setRowCount(m_Ui->UrlSpinBox->value());

        emit initModelData(m_Ui->ThreadSpinBox->value(), m_Ui->UrlSpinBox->value(), m_Ui->StartURL->text(), m_Ui->textEdit->toPlainText());
    }
}

/* When PauseButton is clicked, the form fields are locked.
 * Button text is changed and send pause signal to model
 */
void MainWindow::on_PauseButton_clicked()
{
    m_Ui->StartURL->setDisabled(true);
    m_Ui->textEdit->setDisabled(true);

    m_Ui->UrlSpinBox->setDisabled(true);
    m_Ui->ThreadSpinBox->setDisabled(true);

    if (m_Ui->PauseButton->text() == STRING_PAUSE)
        m_Ui->PauseButton->setText(STRING_RESUME);
    else
        m_Ui->PauseButton->setText(STRING_PAUSE);

    emit pauseBtnClicked();
}

/* When CancelButton is clicked,
 * the form fields are unlocked,
 * and send signal to model
 */
void MainWindow::on_CancelButton_clicked()
{
    if(m_Ui->CancelButton->text() == STRING_EXIT)
        qApp->exit();

    m_Ui->StartURL->setDisabled(false);
    m_Ui->textEdit->setDisabled(false);

    m_Ui->UrlSpinBox->setDisabled(false);
    m_Ui->ThreadSpinBox->setDisabled(false);

    m_Ui->CancelButton->setText(STRING_EXIT);

    emit cancelOperation();
}

/* Slot receives a signal from the model, and resets the current page status in
 * the view
 */
void MainWindow::pageInfoIsChanged(Node *page)
{
    static int textFound = 1;

    QTableWidgetItem *itemURL = new QTableWidgetItem(page->getSelfURL(),0);
    QTableWidgetItem *itemStatus;

    switch (page->getTextIsFound()) {
    case NotFound:
        itemStatus = new QTableWidgetItem(STRING_NOT_FOUND,0);
        break;

    case Found:
        itemStatus = new QTableWidgetItem(STRING_FOUND,0);
        m_Ui->resultFoundCountValue->setText((QString::number(textFound++)));
        break;

    case Error:
        itemStatus = new QTableWidgetItem(STRING_ERROR,0);
        itemStatus->setToolTip(page->getErrorInfo());
        break;

    case Processing:
        itemStatus = new QTableWidgetItem(STRING_PROCESSING,0);
        break;
    }

    m_Ui->tableWidget->setItem(page->getUniqueID(), 0,itemURL);
    m_Ui->tableWidget->setItem(page->getUniqueID(), 1,itemStatus);

    m_Ui->resultURLCountValue->setText(QString::number(page->getUniqueID()+1));

    int progress = (100.0 / m_Ui->UrlSpinBox->value()) * (page->getUniqueID() +1);
    m_Ui->progressBar->setValue(progress);
}

/* The slot receives a signal from the model algorithm is completed,
 *  and sets the information in a view
 */
void MainWindow::searchIsComplete()
{
    if(m_Ui->resultURLCountValue->text() != QString::number(m_Ui->UrlSpinBox->value()))
        m_Ui->resultURLCount->setText(STRING_URL_CHECKED_DEADLOCK);

    m_Ui->progressBar->setValue(100);
    m_Ui->progressBar->setTextVisible(true);
    m_Ui->progressBar->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    m_Ui->progressBar->setFormat(STRING_COMPLETED);
}
