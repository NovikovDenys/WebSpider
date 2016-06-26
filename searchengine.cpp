#include "searchengine.h"
#include <QApplication>

SearchEngine::SearchEngine(uint threadCount, uint URLcount, QString &startURL, QString &searchText,QQueue<Node*> &queueNodeAfterParseChanged) :
    m_CounterId(0),
    m_IterationCount(0),
    m_ThreadCount(threadCount),
    m_UrlCount(URLcount),
    m_StartUrl(startURL),
    m_SearchText(searchText),
    m_QueuePageAfterParse(&queueNodeAfterParseChanged),
    m_SleepPause(false),
    m_SearchIsCompleteFlag(false)
{
    m_ThreadPool = new QThreadPool(this);
    m_ThreadPool->setMaxThreadCount(threadCount);

    Node *startPage = new Node(startURL);       //  It creates a node starting page,
    startPage->setUniqueID(m_CounterId);        //
    m_QueuePageToParse.push_back(startPage);    //  and sends it to the processing queue

}

SearchEngine::~SearchEngine()
{
    qDebug() << "~SearchEngine()";
}

void SearchEngine::findTextOnWeb()
{
    m_Manager = new QNetworkAccessManager();

    //cycle which is interrupted after the search is complete
    while(!m_SearchIsCompleteFlag)
    {
        //pause cycle
        while(m_SleepPause)
        {
            QApplication::processEvents();
        }

        //duplication test cycle
        while(!m_QueuePageToParse.isEmpty() && m_UrlCount >= m_CounterId)
        {
            bool isDuplicate = false;
            for(int i = 0; i < m_QueuePageAfterParse->count(); i++)
            {
                if(m_QueuePageToParse[0]->getSelfURL() == (*m_QueuePageAfterParse)[i]->getSelfURL())
                {
                    isDuplicate = true;
                    m_QueuePageToParse.pop_front();
                    break;
                }
            }


            //request data block and the start of processing in a separate thread
            if(!isDuplicate && !m_QueuePageToParse.isEmpty())
            {
                Node* page = m_QueuePageToParse.dequeue();
                connect(page, SIGNAL(nodeIsChanged(Node*)), this, SLOT(pageChanged(Node*)));
                page->setUniqueID(m_CounterId++);
                page->setTextIsFound(Processing);
                m_QueuePageAfterParse->enqueue(page);

                QEventLoop loop;
                QNetworkReply *reply = m_Manager->get(QNetworkRequest(QUrl(page->getSelfURL())));
                connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();

                if(reply->error() != QNetworkReply::NoError)
                {
                    (*m_QueuePageAfterParse)[m_CounterId -1]->setTextIsFound(Error);
                    (*m_QueuePageAfterParse)[m_CounterId -1]->setErrorInfo(reply->errorString());

                }
                else
                {
                    QByteArray *pageContent = new QByteArray(reply->readAll());

                    ThreadParser *parser = new ThreadParser(page, pageContent, m_SearchText);
                    while(!m_ThreadPool->tryStart(parser));
                }
            }
        }
        //checking algorithm end
        if(!addToQueue())
        {
            m_SearchIsCompleteFlag = !m_SearchIsCompleteFlag;

            if(m_UrlCount == m_CounterId)
            {
                 for(int i = 0; i < m_QueuePageAfterParse->count(); i++)
                    qDebug() << (*m_QueuePageAfterParse)[i]->getSelfURL();
            }
        }
    }

    QApplication::processEvents(0);
    emit searchComplete();
}

Node* SearchEngine::getQueueNodeAfterParse(int index) const
{
    return (*m_QueuePageAfterParse)[index];
}

int SearchEngine::getQueueNodeAfterParseCount() const
{
    return m_QueuePageAfterParse->count();
}

bool SearchEngine::addToQueue()
{
    if(m_QueuePageToParse.isEmpty() && m_QueuePageAfterParse->count() < m_UrlCount)
    {

        while (!m_ThreadPool->waitForDone(-1)){}

        for(int i = m_IterationCount;i < m_QueuePageAfterParse->count();i++, m_IterationCount++)
        {
            while((*m_QueuePageAfterParse)[i]->getLinkURLCount())
            {
                if((m_QueuePageAfterParse->count() + m_QueuePageToParse.count()) < m_UrlCount)
                {
                    Node *page = new Node((*m_QueuePageAfterParse)[i]->dequeLinkURL());
                    m_QueuePageToParse.push_back(page);
                }
                else if(!m_QueuePageToParse.isEmpty())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        if(m_QueuePageToParse.isEmpty())
            return false;
    }
    else
    {
         return false;
    }

    return true;
}

void SearchEngine::setSleepPauseFlag(bool flag)
{
    m_SleepPause = flag;
}

bool SearchEngine::getSleepPauseFlag() const
{
    return m_SleepPause;
}

void SearchEngine::setSearchIsCompleteFlag(bool flag)
{
    m_SearchIsCompleteFlag = flag;
}

bool SearchEngine::getSearchIsCompleteFlag() const
{
    return m_SearchIsCompleteFlag;
}

void SearchEngine::restartNetworkManager()
{
    findTextOnWeb();
}

void SearchEngine::pause()
{
    m_SleepPause = !m_SleepPause;
}

void SearchEngine::pageChanged(Node *node)
{
    emit pageIsChanged(node);
}


