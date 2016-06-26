#include "threadparser.h"

#define REG_EXP_HTTP                "(http[s]?:\\/\\/)([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?"

ThreadParser::ThreadParser(Node *page, QByteArray *pageContent, const QString &findText) :
    m_Page(page),
    m_PageContent(pageContent),
    m_FindText(findText)
{
}

void ThreadParser::run()
{
    m_PageContent->toLower();

    //If the SearchText is found
    if(m_PageContent->contains(m_FindText.toLower().toUtf8()))
        m_Page->setTextIsFound(Found);
    else
        m_Page->setTextIsFound(NotFound);

    QRegExp exp(REG_EXP_HTTP);
    QString tempStr;
    int tempPos = 0;

    //Search URL on the page
    while ((tempPos = exp.indexIn(*m_PageContent, tempPos)) != -1) {
        tempStr = exp.cap(0);
        tempPos += exp.matchedLength();
        m_Page->setLinkURL(tempStr);
    }
}
