/*!
    \brief This class implements a wrapper
    to run properly in the stream

    The goal - to provide an implementation start
    the search in a separate stream of data to
    update the node information obtained from page
    content data
*/

#ifndef THREADPARSER_H
#define THREADPARSER_H
#include<node.h>
#include<QThreadPool>

class ThreadParser : public QRunnable
{
public:
    ThreadParser(Node *page, QByteArray *pageContent, const QString &findText);
    void run();
private:
    Node *m_Page;
    QByteArray *m_PageContent;
    QString m_FindText;

};

#endif // THREADPARSER_H
