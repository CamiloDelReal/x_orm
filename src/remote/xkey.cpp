#include "xkey.hpp"


XKey::XKey()
{

}

XKey::XKey(const QString &member)
{
    m_member = member;
    m_name = member;
}

XKey::XKey(const QString &member, const QString &name)
{
    m_member = member;
    m_name = name;
}

XKey::XKey(const XKey &other)
{
    m_member = other.m_member;
    m_name = other.m_name;
}

XKey &XKey::operator=(const XKey &other)
{
    m_member = other.m_member;
    m_name = other.m_name;

    return *this;
}

QString XKey::member() const
{
    return m_member;
}

QString XKey::name() const
{
    return m_name;
}

void XKey::setMember(const QString &member)
{
    if (m_member != member)
    {
        m_member = member;
    }
}

void XKey::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
    }
}
