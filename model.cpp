#include "model.h"
#include <QDebug>

#define MAX_THREAD_COUNT 1000
#define MAX_URL_COUNT 1000

Model::Model()
{}

Model::Model(uint threadCount, uint URLcount, QString &startURL, QString &searchText) :
    m_ThreadCount(threadCount),
    m_UrlCount(URLcount),
    m_StartUrl(startURL),
    m_SearchText(searchText)
{}

Model::~Model()
{}

void Model::setThreadCount(uint count)
{
    if(count <= MAX_THREAD_COUNT)
    {
        if(count !=0)
            m_ThreadCount = count;
        else
            m_ThreadCount = 1;
    }
    else
    {
        m_ThreadCount = MAX_THREAD_COUNT;
    }
}

int Model::getThreadCount() const
{
    return m_ThreadCount;
}

void Model::setURLCount(uint count)
{
    if(count <= MAX_URL_COUNT)
    {
        if(count !=0)
            m_UrlCount = count;
        else
            m_UrlCount = 1;
    }
    else
    {
        m_UrlCount = MAX_URL_COUNT;
    }
}

int Model::getURLCount() const
{
    return m_UrlCount;
}

void Model::setStartURL(const QString &url)
{
    m_StartUrl = url;
}

QString Model::getStartURL() const
{
    return m_StartUrl;
}

void Model::setSearchText(const QString &text)
{
    m_SearchText = text;
}

QString Model::getSearchText() const
{
    return m_SearchText;
}

SearchEngine* Model::getSearchEngine() const
{
    return m_SearchEngine;
}

/* Initializes the model with data from the GUI part,
 * creates the engine and executes an algorithm in a separate thread
 */
void Model::initModelData(uint threadCount, uint URLcount, QString &startURL, QString &searchText)
{
    m_ThreadCount = threadCount;
    m_UrlCount = URLcount;
    m_StartUrl = startURL;
    m_SearchText = searchText;

    m_Thread = new QThread(this);

    m_SearchEngine = new SearchEngine(m_ThreadCount, m_UrlCount, m_StartUrl, m_SearchText, m_QueuePageAfterParse);
    m_SearchEngine->moveToThread(m_Thread);

    connect(m_SearchEngine, SIGNAL(searchComplete()), this, SLOT(searchIsComplete()));
    connect(m_SearchEngine, SIGNAL(pageIsChanged(Node*)), this, SLOT(pageChaged(Node*)));

    m_Thread->start();
    m_SearchEngine->findTextOnWeb();
}

void Model::pageChaged(Node *page)
{
    emit pageIsChanged(page);
}

void Model::pause()
{
    m_SearchEngine->setSleepPauseFlag(!m_SearchEngine->getSleepPauseFlag());
}

void Model::cancelOperation()
{
    if(m_SearchEngine)
        m_SearchEngine->setSearchIsCompleteFlag(true);
}

void Model::searchIsComplete()
{
    emit searchComplete();
}
