#include "node.h"

Node::Node(const QString &str) :
    m_IsTextFoundFlag(NotFound),
    m_SelfUrl(str)
{
}

void Node::setUniqueID(int id)
{
    m_Id = id;
}

int Node::getUniqueID() const
{
    return m_Id;
}

void Node::setTextIsFound(textIsFound isTextFoundFlag)
{
    m_IsTextFoundFlag = isTextFoundFlag;
    emit nodeIsChanged(this);

}

textIsFound Node::getTextIsFound() const
{
    return m_IsTextFoundFlag;
}

void Node::setSelfURL(const QString str)
{
    m_SelfUrl = str;
}

QString Node::getSelfURL() const
{
    return m_SelfUrl;
}

void Node::setErrorInfo(const QString str)
{
    m_ErrorInfo = str;
}

QString Node::getErrorInfo() const
{
    return m_ErrorInfo;
}

void Node::setLinkURL(const QString &str)
{
    m_LinkUrl.push_back(str);
}

QString Node::dequeLinkURL()
{
    return m_LinkUrl.dequeue();
}

int Node::getLinkURLCount() const
{
    return m_LinkUrl.count();
}


