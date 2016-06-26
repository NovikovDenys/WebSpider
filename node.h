#ifndef NODE_H
#define NODE_H

#include <QQueue>
#include <QObject>

enum textIsFound{ NotFound = 0, Found, Error, Processing };

class Node : public QObject
{
    Q_OBJECT

public:
    Node(const QString &str);
    Node(const Node &otherNode) = delete;
    Node operator=(Node &otherNode) = delete;
    void setUniqueID(int id);
    int getUniqueID() const;

    void setTextIsFound(textIsFound isTextFoundFlag);
    textIsFound getTextIsFound() const;

    void setSelfURL(const QString str);
    QString getSelfURL() const;

    void setErrorInfo(const QString str);
    QString getErrorInfo() const;

    void setLinkURL(const QString &str);
    QString dequeLinkURL();
    int getLinkURLCount() const;

signals:
    void nodeIsChanged(Node*);

private:
    int m_Id;

    textIsFound m_IsTextFoundFlag;
    QString m_SelfUrl;
    QString m_ErrorInfo;
    QQueue<QString> m_LinkUrl;
};

#endif // NODE_H
