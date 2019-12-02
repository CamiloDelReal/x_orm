#include "xfield.hpp"
#include "xstringtemplates.hpp"


XFieldClauses::XFieldClauses()
{
}

XFieldClauses::XFieldClauses(const XPropertyType &type)
{
    m_clauses.insert(type, QVariant::Invalid);
}

XFieldClauses::XFieldClauses(const XPropertyType &type, const QVariant &value)
{
    m_clauses.insert(type, value);
}

XFieldClauses::XFieldClauses(const XFieldClauses &other)
{
    m_clauses = other.m_clauses;
}

XFieldClauses& XFieldClauses::operator=(const XFieldClauses &other)
{
    m_clauses = other.m_clauses;

    return *this;
}

XFieldClauses& XFieldClauses::operator|(const XFieldClauses &other)
{
    QMapIterator<XPropertyType, QVariant> iterator(other.m_clauses);
    while(iterator.hasNext())
    {
        iterator.next();
        m_clauses.insert(iterator.key(), iterator.value());
    }

    return *this;
}

bool XFieldClauses::operator!=(const XFieldClauses &other)
{
    return (m_clauses != other.m_clauses);
}

bool XFieldClauses::hasClause(const XPropertyType &type) const
{
    return m_clauses.contains(type);
}

QMap<XPropertyType, QVariant> XFieldClauses::clausesMap() const
{
    return m_clauses;
}

QVariant XFieldClauses::clauseValue(const XPropertyType &type) const
{
    return m_clauses.value(type, EMPTY_TEMPLATE);
}


XDataConverter::XDataConverter() {}

XDataConverter::~XDataConverter() {}


XDataConverterPriv::XDataConverterPriv()
{
    m_dataConverter = nullptr;
}

XDataConverterPriv::XDataConverterPriv(XDataConverter *dataConverter)
{
    m_dataConverter = dataConverter;
}

XDataConverterPriv::XDataConverterPriv(const XDataConverterPriv &other)
{
    m_dataConverter = other.m_dataConverter;
}

XDataConverterPriv& XDataConverterPriv::operator=(const XDataConverterPriv &other)
{
    m_dataConverter = other.m_dataConverter;

    return *this;
}

QVariant XDataConverterPriv::to(const QVariant &data)
{
    return m_dataConverter->to(data);
}

QVariant XDataConverterPriv::from(const QVariant &data)
{
    return m_dataConverter->from(data);
}


XField::XField()
{
    m_type = QMetaType::UnknownType;
}

XField::XField(const QString &member)
{
    m_member = member;
    m_name = member;
    m_type = QMetaType::UnknownType;
}

XField::XField(const QString &member, const QMetaType::Type &type)
{
    m_member = member;
    m_name = member;
    m_type = type;
}

XField::XField(const QString &member, const QString &name)
{
    m_member = member;
    m_name = name;
    m_type = QMetaType::UnknownType;
}

XField::XField(const QString &member, const XFieldClauses &clauses)
{
    m_member = member;
    m_name = member;
    m_type = QMetaType::UnknownType;
    m_clauses = clauses;
}

XField::XField(const QString &member, const QMetaType::Type &type, const XFieldClauses &clauses)
{
    m_member = member;
    m_name = member;
    m_type = type;
    m_clauses = clauses;
}

XField::XField(const QString &member, const QString &name, const XFieldClauses &clauses)
{
    m_member = member;
    m_name = name;
    m_type = QMetaType::UnknownType;
    m_clauses = clauses;
}

XField::XField(const QString &member, const QString &name, const QMetaType::Type &type, const XFieldClauses &clauses)
{
    m_member = member;
    m_name = name;
    m_type = type;
    m_clauses = clauses;
}

XField::XField(const XField &other)
{
    m_member = other.m_member;
    m_name = other.m_name;
    m_type = other.m_type;
    m_clauses = other.m_clauses;
}

XField& XField::operator=(const XField &other)
{
    m_member = other.m_member;
    m_name = other.m_name;
    m_type = other.m_type;
    m_clauses = other.m_clauses;

    return *this;
}

QString XField::member() const
{
    return m_member;
}

QString XField::name() const
{
    return m_name;
}

QMetaType::Type XField::type() const
{
    return m_type;
}

XFieldClauses XField::clauses() const
{
    return m_clauses;
}

bool XField::isValid()
{
    return (!m_name.isNull() && !m_name.isEmpty() && m_type != QMetaType::UnknownType);
}

void XField::setMember(const QString &member)
{
    if (m_member != member)
    {
        m_member = member;
    }
}

void XField::setName(const QString &name)
{
    if (m_name != name)
    {
        m_name = name;
    }
}

void XField::setType(const QMetaType::Type &type)
{
    if (m_type != type)
    {
        m_type = type;
    }
}

void XField::setClauses(const XFieldClauses &clauses)
{
    if (m_clauses != clauses)
    {
        m_clauses = clauses;
    }
}


XForeignKeyDecl::XForeignKeyDecl() {}

XForeignKeyDecl::~XForeignKeyDecl() {}


XForeignKey::XForeignKey()
{
    m_foreignKeyDecl = nullptr;
}

XForeignKey::XForeignKey(XForeignKeyDecl *foreignKeyDeclObj)
{
    m_foreignKeyDecl.reset(foreignKeyDeclObj);
}

XForeignKey::XForeignKey(const XForeignKey &other)
{
    m_foreignKeyDecl = other.m_foreignKeyDecl;
}

XForeignKey& XForeignKey::operator=(const XForeignKey &other)
{
    m_foreignKeyDecl = other.m_foreignKeyDecl;

    return *this;
}

QString XForeignKey::referencedTable() const
{
    return m_foreignKeyDecl != nullptr
            ? m_foreignKeyDecl->referencedTable()
            : QString();
}

QString XForeignKey::referencedField() const
{
    return m_foreignKeyDecl != nullptr
            ? m_foreignKeyDecl->referencedField()
            : QString();
}

XForeignBehavior XForeignKey::onUpdate() const
{
    return m_foreignKeyDecl != nullptr
            ? m_foreignKeyDecl->onUpdate()
            : SET_NULL;
}

XForeignBehavior XForeignKey::onDelete() const
{
    return m_foreignKeyDecl != nullptr
            ? m_foreignKeyDecl->onDelete()
            : SET_NULL;
}


XForeignObjectDecl::XForeignObjectDecl() {}

XForeignObjectDecl::~XForeignObjectDecl() {}


XForeignObject::XForeignObject(XForeignObjectDecl *foreignObjectDeclObj)
{
    m_foreignObject.reset(foreignObjectDeclObj);
}

QString XForeignObject::localProperty()
{
    return m_foreignObject->localProperty();
}

QString XForeignObject::foreignProperty()
{
    return m_foreignObject->foreignProperty();
}

QSqlError XForeignObject::resolve(QVariant &objectData, const QVariant &localPropertyValue, const QString &connectionName)
{
    return m_foreignObject->resolve(objectData, localPropertyValue, connectionName);
}

QString XForeignObject::objectProperty()
{
    return m_foreignObject->objectProperty();
}


XForeignListDecl::XForeignListDecl() {}

XForeignListDecl::~XForeignListDecl() {}


XForeignList::XForeignList()
{
}

XForeignList::XForeignList(XForeignListDecl *foreignListDeclObj)
{
    m_foreignListDecl.reset(foreignListDeclObj);
}

XForeignList::XForeignList(const XForeignList &other)
{
    m_foreignListDecl = other.m_foreignListDecl;
}

XForeignList& XForeignList::operator=(const XForeignList &other)
{
    m_foreignListDecl = other.m_foreignListDecl;

    return *this;
}

QString XForeignList::localProperty()
{
    return m_foreignListDecl->localProperty();
}

QSqlError XForeignList::resolve(QVariant &listData, const QVariant &localPropertyValue, const QString &connectionName)
{
    return m_foreignListDecl->resolve(listData, localPropertyValue, connectionName);
}

QString XForeignList::listProperty()
{
    return m_foreignListDecl->listProperty();
}
