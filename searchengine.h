#ifndef ENGINE_H
#define ENGINE_H
#include <QRegExp>
#include <QQueue>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <node.h>
#include<QThreadPool>
#include <threadparser.h>


class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(uint threadCount, uint URLcount, QString &startURL, QString &searchText,QQueue<Node*> &queueNodeAfterParseChanged);
    SearchEngine(const SearchEngine &otherSearchEngine) = delete;
    SearchEngine operator=(SearchEngine &otherSearchEngine) = delete;

    void findTextOnWeb();

    Node *getQueueNodeAfterParse(int index) const;
    int getQueueNodeAfterParseCount() const;

    bool addToQueue();

    void setSleepPauseFlag(bool flag);
    bool getSleepPauseFlag() const;

    void setSearchIsCompleteFlag(bool flag);
    bool getSearchIsCompleteFlag() const;
    ~SearchEngine();
signals:
    void pageIsChanged(Node*);
    void searchComplete();

public slots:
    void restartNetworkManager();
    void pause();
    void pageChanged(Node* page);

private:
    int m_CounterId;
    int m_IterationCount;

    int m_ThreadCount;
    int m_UrlCount;

    QString m_StartUrl;
    QString m_SearchText;

    bool m_SleepPause;
    bool m_SearchIsCompleteFlag;
    QThreadPool *m_ThreadPool;
    QQueue<Node*> m_QueuePageToParse;
    QQueue<Node*> *m_QueuePageAfterParse;

    QNetworkAccessManager *m_Manager;

};

#endif // ENGINE_H
