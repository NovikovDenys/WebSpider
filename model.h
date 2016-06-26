#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QVector>
#include <node.h>
#include <QObject>
#include <searchengine.h>

class Model : public QObject
{
    Q_OBJECT
public:
    Model();
    Model(uint threadCount, uint URLcount, QString &startURL, QString &searchText);
    Model(const Model &otherModel) = delete;
    Model operator=(Model &otherModel) = delete;

    void setThreadCount(uint count);
    int getThreadCount() const;

    void setURLCount(uint count);
    int getURLCount() const;

    void setStartURL(const QString &url);
    QString getStartURL() const;

    void setSearchText(const QString &text);
    QString getSearchText() const;

    SearchEngine *getSearchEngine() const;
    ~Model();
signals:
    void pageIsChanged(Node*);
    void searchComplete();

private slots:
    void initModelData(uint threadCount, uint URLcount, QString &startURL, QString &searchText);
    void pageChaged(Node* page);
    void pause();
    void cancelOperation();
    void searchIsComplete();

private:
    uint m_ThreadCount;
    uint m_UrlCount;

    SearchEngine *m_SearchEngine;
    QThread *m_Thread;
    QString m_StartUrl;
    QString m_SearchText;

    QQueue<Node*> m_QueuePageAfterParse;
};

#endif // MODEL_H
