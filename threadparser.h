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
