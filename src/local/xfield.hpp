#ifndef XFIELD_HPP
#define XFIELD_HPP

#include <QObject>
#include <QVariant>
#include <QMetaObject>
#include <QMetaType>
#include <QMetaProperty>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSharedPointer>

#include "xstringtemplates.hpp"


enum XPropertyType
{
    PRIMARY_KEY,
    PRIMARY_KEY_AUTOINCREMENT,

    NOT_NULL,

    DEFAULT,

    CHECK,

    UNIQUE,
    UNIQUE_ASC,
    UNIQUE_DESC,

    INDEX_ASC,
    INDEX_DESC,
    INDEX_UNIQUE_ASC,
    INDEX_UNIQUE_DESC,

    FOREIGN_KEY,
    DATA_CONVERTER
};


class XFieldClauses
{
public:
    explicit XFieldClauses();
    XFieldClauses(const XPropertyType &type);
    XFieldClauses(const XPropertyType &type, const QVariant &value);
    XFieldClauses(const XFieldClauses &other);

    XFieldClauses& operator=(const XFieldClauses &other);
    XFieldClauses& operator|(const XFieldClauses &other);
    bool operator!=(const XFieldClauses &other);

    bool hasClause(const XPropertyType &type) const;

    QMap<XPropertyType, QVariant> clausesMap() const;

    QVariant clauseValue(const XPropertyType &type) const;

private:
    QMap<XPropertyType, QVariant> m_clauses;
};


class XDataConverter
{
public:
    explicit XDataConverter();
    virtual ~XDataConverter();

    virtual QVariant to(const QVariant &data) = 0;
    virtual QVariant from(const QVariant &data) = 0;
};
Q_DECLARE_METATYPE(XDataConverter*)


class XDataConverterPriv
{
public:
    explicit XDataConverterPriv();

    XDataConverterPriv(XDataConverter* dataConverter);

    XDataConverterPriv(const XDataConverterPriv &other);

    XDataConverterPriv& operator=(const XDataConverterPriv &other);

    QVariant to(const QVariant &data);
    QVariant from(const QVariant &data);

private:
    XDataConverter* m_dataConverter;
};
Q_DECLARE_METATYPE(XDataConverterPriv)


#define X_PRIMARY_KEY XFieldClauses(PRIMARY_KEY)
#define X_PRIMARY_KEY_AUTOINCREMENT XFieldClauses(PRIMARY_KEY_AUTOINCREMENT)

#define X_NOT_NULL XFieldClauses(NOT_NULL)

#define X_DEFAULT(VALUE) XFieldClauses(DEFAULT, #VALUE)

#define X_CHECK(VALUE) XFieldClauses(XFieldClauses::CHECK, "'" ## #VALUE ## "'")

#define X_UNIQUE XFieldClauses(UNIQUE)
#define X_UNIQUE_ASC XFieldClauses(UNIQUE_ASC)
#define X_UNIQUE_DESC XFieldClauses(UNIQUE_DESC)

#define X_INDEX_ASC XFieldClauses(INDEX_ASC)
#define X_INDEX_DESC XFieldClauses(INDEX_DESC)
#define X_INDEX_UNIQUE_ASC XFieldClauses(INDEX_UNIQUE_ASC)
#define X_INDEX_UNIQUE_DESC XFieldClauses(INDEX_UNIQUE_DESC)

#define X_FOREIGN_KEY_1(MODEL_CLASS) \
    XFieldClauses(FOREIGN_KEY, QVariant::fromValue(XForeignKey(new XForeignKeyPriv<MODEL_CLASS>())))

#define X_FOREIGN_KEY_2(MODEL_CLASS, REFERENCED_FIELD) \
    XFieldClauses(FOREIGN_KEY, QVariant::fromValue(XForeignKey(new XForeignKeyPriv<MODEL_CLASS>(REFERENCED_FIELD))))

#define X_FOREIGN_KEY_3(MODEL_CLASS, ON_UPDATE, ON_DELETE) \
    XFieldClauses(FOREIGN_KEY, QVariant::fromValue(XForeignKey(new XForeignKeyPriv<MODEL_CLASS>(ON_UPDATE, ON_DELETE))))

#define X_FOREIGN_KEY_4(MODEL_CLASS, REFERENCED_FIELD, ON_UPDATE, ON_DELETE) \
    XFieldClauses(FOREIGN_KEY, QVariant::fromValue(XForeignKey(new XForeignKeyPriv<MODEL_CLASS>(REFERENCED_FIELD, ON_UPDATE, ON_DELETE))))

#define X_FOREIGN_KEY_BUILDER(_1, _2, _3, _4, MACRO_NAME, ...) MACRO_NAME

#define X_FOREIGN_KEY(...) X_FOREIGN_KEY_BUILDER(__VA_ARGS__, X_FOREIGN_KEY_4, X_FOREIGN_KEY_3, X_FOREIGN_KEY_2, X_FOREIGN_KEY_1)(__VA_ARGS__)

#define X_DATA_CONVERTER(CONVERTER_CLASS) \
    XFieldClauses(DATA_CONVERTER, QVariant::fromValue(XDataConverterPriv(new CONVERTER_CLASS())))

#define X_FALSE 0
#define X_TRUE  1


class XField
{
public:
    explicit XField();
    XField(const QString &member);
    XField(const QString &member, const QMetaType::Type &type);
    XField(const QString &member, const QString &name);
    XField(const QString &member, const XFieldClauses &clauses);
    XField(const QString &member, const QMetaType::Type &type, const XFieldClauses &clauses);
    XField(const QString &member, const QString &name, const XFieldClauses &clauses);
    XField(const QString &member, const QString &name, const QMetaType::Type &type, const XFieldClauses &clauses);
    XField(const XField &other);

    XField& operator=(const XField &other);

    QString member() const;
    QString name() const;
    QMetaType::Type type() const;
    XFieldClauses clauses() const;

    bool isValid();

    void setMember(const QString &member);
    void setName(const QString &name);
    void setType(const QMetaType::Type &type);
    void setClauses(const XFieldClauses &clauses);

private:
    QString m_member;
    QString m_name;
    QMetaType::Type m_type;
    XFieldClauses m_clauses;
};

#define X_FIELD_1(MODEL_PROPERTY) \
    XField(#MODEL_PROPERTY)

#define X_FIELD_2(MODEL_PROPERTY, TYPE_OR_FIELD_NAME_OR_CLAUSES) \
    XField(#MODEL_PROPERTY, TYPE_OR_FIELD_NAME_OR_CLAUSES)

#define X_FIELD_3(MODEL_PROPERTY, TYPE_OR_FIELD_NAME, CLAUSES) \
    XField(#MODEL_PROPERTY, TYPE_OR_FIELD_NAME, CLAUSES)

#define X_FIELD_4(MODEL_PROPERTY, FIELD_NAME, TYPE, CLAUSES) \
    XField(#MODEL_PROPERTY, FIELD_NAME, TYPE, CLAUSES)

#define X_FIELD_BUILDER(_1, _2, _3, _4, MACRO_NAME, ...) MACRO_NAME

#define X_FIELD(...) X_FIELD_BUILDER(__VA_ARGS__, X_FIELD_4, X_FIELD_3, X_FIELD_2, X_FIELD_1)(__VA_ARGS__)


enum XForeignBehavior
{
    RESTRIC,
    NO_ACTION,
    CASCADE,
    SET_NULL,
    SET_DEFAULT
};

#define X_RESTRIC XForeignBehavior::RESTRICT
#define X_NO_ACTION XForeignBehavior::NO_ACTION
#define X_CASCADE XForeignBehavior::CASCADE
#define X_SET_NULL XForeignBehavior::SET_NULL
#define X_SET_DEFAULT XForeignBehavior::SET_DEFAULT


class XForeignKeyDecl
{
public:
    explicit XForeignKeyDecl();
    virtual ~XForeignKeyDecl();

    virtual QString referencedTable() const = 0;

    virtual QString referencedField() const = 0;

    virtual XForeignBehavior onUpdate() const = 0;

    virtual XForeignBehavior onDelete() const = 0;
};
Q_DECLARE_METATYPE(XForeignKeyDecl*);


template <class T>
class XForeignKeyPriv : public XForeignKeyDecl
{
public:
    explicit XForeignKeyPriv() : XForeignKeyDecl ()
    {
        m_referencedTable = T::tableName();
        XField autoGenKey = T::autogenPrimaryKey();
        if(autoGenKey.isValid())
        {
            m_referencedField = autoGenKey.name();
        }
        else
        {
            qWarning("No referenced field declared or no autogenerated primary key defined in referenced table");
        }

        m_onUpdate = SET_NULL;
        m_onDelete = SET_NULL;
    }

    XForeignKeyPriv(const QString &referencedField)
    {
        m_referencedTable = T::tableName();
        m_referencedField = referencedField;
        m_onUpdate = SET_NULL;
        m_onDelete = SET_NULL;
    }

    XForeignKeyPriv(const XForeignBehavior &onUpdate, XForeignBehavior onDelete)
    {
        m_referencedTable = T::tableName();
        XField autoGenKey = T::autogenPrimaryKey();
        if(autoGenKey.isValid())
        {
            m_referencedField = autoGenKey.name();
        }
        else
        {
            qWarning("No referenced field declared or no autogenerated primary key defined in referenced table");
        }

        m_onUpdate = onUpdate;
        m_onDelete = onDelete;
    }

    XForeignKeyPriv(const QString &referencedField, const XForeignBehavior &onUpdate, XForeignBehavior onDelete)
    {
        m_referencedTable = T::tableName();
        m_referencedField = referencedField;
        m_onUpdate = onUpdate;
        m_onDelete = onDelete;
    }

    XForeignKeyPriv(const XForeignKeyPriv &other)
    {
        m_referencedTable = other.m_referencedTable;
        m_referencedField = other.m_referencedField;
        m_onUpdate = other.m_onUpdate;
        m_onDelete = other.m_onDelete;
    }

    XForeignKeyPriv& operator=(const XForeignKeyPriv &other)
    {
        m_referencedTable = other.m_referencedTable;
        m_referencedField = other.m_referencedField;
        m_onUpdate = other.m_onUpdate;
        m_onDelete = other.m_onDelete;

        return *this;
    }

    QString referencedTable() const
    {
        return m_referencedTable;
    }

    QString referencedField() const
    {
        return m_referencedField;
    }

    XForeignBehavior onUpdate() const
    {
        return m_onUpdate;
    }

    XForeignBehavior onDelete() const
    {
        return m_onDelete;
    }

    void setReferencedTable(const QString &referencedTable)
    {
        if (m_referencedTable != referencedTable)
        {
            m_referencedTable = referencedTable;
        }
    }

    void setReferencedField(const QString &referencedField)
    {
        if (m_referencedField != referencedField)
        {
            m_referencedField = referencedField;
        }
    }

    void setOnUpdate(const XForeignBehavior &onUpdate)
    {
        if (m_onUpdate != onUpdate)
        {
            m_onUpdate = onUpdate;
        }
    }

    void setOnDelete(const XForeignBehavior &onDelete)
    {
        if (m_onDelete != onDelete)
        {
            m_onDelete = onDelete;
        }
    }

private:
    QString m_referencedTable;
    QString m_referencedField;
    XForeignBehavior m_onUpdate;
    XForeignBehavior m_onDelete;

};

// Q_DECLARE_METATYPE_TEMPLATE_1ARG did't worked for me
template <class T>
struct QMetaTypeId<XForeignKeyPriv<T>*>
{
    enum {
        Defined = QMetaTypeId2<T>::Defined
    };
    static int qt_metatype_id()
    {
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatype_id.loadRelaxed())
            return id;
        const char *tName = QMetaType::fromType<T>();
        Q_ASSERT(tName);
        const int tNameLen = int(qstrlen(tName));
        QByteArray typeName;
        typeName.reserve(int(sizeof("XForeignKeyPriv*")) + 1 + tNameLen + 1 + 1);
        typeName.append("XForeignKeyPriv*", int(sizeof("XForeignKeyPriv*")) - 1)
                .append('<').append(tName, tNameLen);
        if (typeName.endsWith('>'))
            typeName.append(' ');
        typeName.append('>');
        const int newId = qRegisterNormalizedMetaType<XForeignKeyPriv<T>*>(
                    typeName,
                    reinterpret_cast<XForeignKeyPriv<T>**>(quintptr(-1)));
        metatype_id.storeRelease(newId);
        return newId;
    }
};


class XForeignKey
{
public:
    explicit XForeignKey();
    XForeignKey(XForeignKeyDecl *foreignKeyDeclObj);
    XForeignKey(const XForeignKey &other);

    XForeignKey& operator=(const XForeignKey &other);

    QString referencedTable() const;

    QString referencedField() const;

    XForeignBehavior onUpdate() const;

    XForeignBehavior onDelete() const;

private:
    QSharedPointer<XForeignKeyDecl> m_foreignKeyDecl;
};
Q_DECLARE_METATYPE(XForeignKey);


class XForeignObjectDecl
{
public:
    explicit XForeignObjectDecl();
    virtual ~XForeignObjectDecl();

    virtual QString localProperty() = 0;

    virtual QString foreignProperty() = 0;

    virtual QSqlError resolve(QVariant &objectData, const QVariant &localPropertyValue, const QString &connectionName) = 0;

    virtual QString objectProperty() = 0;
};


template <class T>
class XForeignObjectPriv : public XForeignObjectDecl
{
public:
    explicit XForeignObjectPriv(const QString &foreignProperty, const QString &localProperty, const QString &objectProperty)
    {
        m_foreignProperty = foreignProperty;
        m_localProperty = localProperty;
        m_objectProperty = objectProperty;
    }

    XForeignObjectPriv(const XForeignObjectPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_objectProperty = other.m_objectProperty;
    }

    XForeignObjectPriv& operator=(const XForeignObjectPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_objectProperty = other.m_objectProperty;

        return *this;
    }

    QString foreignProperty()
    {
        return m_foreignProperty;
    }

    QString localProperty()
    {
        return m_localProperty;
    }

    QSqlError resolve(QVariant &objectData, const QVariant &localPropertyValue, const QString &connectionName)
    {
        QList<XField> fields = T::fields();
        QStringList members;
        QStringList fieldNames;
        QString findKey;

        foreach (XField field, fields)
        {
            members << field.member();
            fieldNames << field.name();

            if(field.member() == m_foreignProperty)
            {
                findKey = field.name();
            }
        }

        QString compareStatement = EQUAL_TEMPLATE.arg(findKey).arg(localPropertyValue.toString());
        QString tableName = T::tableName();
        QString sql = FIND_FIRST_TEMPLATE.arg(tableName).arg(compareStatement);
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        QSqlQuery query(sql, db);

        T* item = nullptr;

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;
            foreach (QString fieldName, fieldNames)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            if(query.next())
            {
                item = new T;

                for(qint16 i = 0; i < members.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        item->setProperty(members.at(i).toLocal8Bit(),
                                          query.value(columnsIndex.at(i)));
                    }
                }

                objectData.setValue(item);
            }
        }

        return query.lastError();
    }

    QString objectProperty()
    {
        return m_objectProperty;
    }

private:
    QString m_foreignProperty;
    QString m_localProperty;
    QString m_objectProperty;
};


class XForeignObject
{
public:
    explicit XForeignObject(XForeignObjectDecl *foreignObjectDeclObj);

    QString localProperty();

    QString foreignProperty();

    QSqlError resolve(QVariant &objectData, const QVariant &localPropertyValue, const QString &connectionName);

    QString objectProperty();

private:
    QSharedPointer<XForeignObjectDecl> m_foreignObject;
};

#define X_FOREIGN_OBJECT(MODEL_CLASS, FOREIGN_PROPERTY, LOCAL_PROPERTY, OBJECT_PROPERTY) \
    XForeignObject(new XForeignObjectPriv<MODEL_CLASS>(#FOREIGN_PROPERTY, #LOCAL_PROPERTY, #OBJECT_PROPERTY))

#define X_FOREIGN_OBJECTS(OBJECTS...) \
    public: \
    static QList<XForeignObject> foreignObjects() \
{ \
    QList<XForeignObject> objects = { OBJECTS }; \
    return objects; \
    }


class XForeignListDecl
{
public:
    explicit XForeignListDecl();
    virtual ~XForeignListDecl();

    virtual QString localProperty() = 0;

    virtual QSqlError resolve(QVariant &listData, const QVariant &localPropertyValue, const QString &connectionName) = 0;

    virtual QString listProperty() = 0;
};


template <class T>
class XForeignListPriv : public XForeignListDecl
{
public:
    explicit XForeignListPriv(const QString &foreignProperty, const QString &localProperty, const QString &listProperty)
    {
        m_foreignProperty = foreignProperty;
        m_localProperty = localProperty;
        m_listProperty = listProperty;
    }

    XForeignListPriv(const XForeignListPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_listProperty = other.m_listProperty;
    }

    XForeignListPriv& operator=(const XForeignListPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_listProperty = other.m_listProperty;

        return *this;
    }

    QString localProperty()
    {
        return m_localProperty;
    }

    QSqlError resolve(QVariant &listData, const QVariant &localPropertyValue, const QString &connectionName)
    {
        QList<XField> fields = T::fields();
        QStringList members;
        QStringList fieldNames;
        QString findKey;

        foreach (XField field, fields)
        {
            members << field.member();
            fieldNames << field.name();

            if(field.member() == m_foreignProperty)
            {
                findKey = field.name();
            }
        }

        QString compareStatement = EQUAL_TEMPLATE.arg(findKey).arg(localPropertyValue.toString());
        QString tableName = T::tableName();
        QString sql = FIND_TEMPLATE.arg(tableName).arg(compareStatement);

        QSqlDatabase db = QSqlDatabase::database(connectionName);
        QSqlQuery query(sql, db);

        QList<T*> items;

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;
            foreach (QString fieldName, fieldNames)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            while(query.next())
            {
                T* item = new T;

                for(qint16 i = 0; i < members.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        item->setProperty(members.at(i).toLocal8Bit(),
                                          query.value(columnsIndex.at(i)));
                    }
                }

                items.append(item);
            }

            listData.setValue(items);
        }

        return query.lastError();
    }

    QString listProperty()
    {
        return m_listProperty;
    }

private:
    QString m_foreignProperty;
    QString m_localProperty;
    QString m_listProperty;
};


template <class L, class I, class R>
class XIntermediatedForeignListPriv : public XForeignListDecl
{
public:
    explicit XIntermediatedForeignListPriv(const QString &foreignProperty, const QString &localProperty, const QString foreignLinkProperty,
                                           const QString &localLinkProperty, const QString &listProperty)
    {
        m_foreignProperty = foreignProperty;
        m_localProperty = localProperty;
        m_listProperty = listProperty;
        m_foreignLinkProperty = foreignLinkProperty;
        m_localLinkProperty = localLinkProperty;
    }

    XIntermediatedForeignListPriv(const XIntermediatedForeignListPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_listProperty = other.m_listProperty;
        m_foreignLinkProperty = other.m_foreignLinkProperty;
        m_localLinkProperty = other.m_localLinkProperty;
    }

    XIntermediatedForeignListPriv& operator=(const XIntermediatedForeignListPriv &other)
    {
        m_foreignProperty = other.m_foreignProperty;
        m_localProperty = other.m_localProperty;
        m_listProperty = other.m_listProperty;
        m_foreignLinkProperty = other.m_foreignLinkProperty;
        m_localLinkProperty = other.m_localLinkProperty;

        return *this;
    }

    QString localProperty()
    {
        return m_localProperty;
    }

    QSqlError resolve(QVariant &listData, const QVariant &localPropertyValue, const QString &connectionName)
    {
        QString tableNameOfLeft = L::tableName();
        QList<XField> fieldsOfLeft = L::fields();
        QString findKeyOfLeft;

        QList<XField>::iterator leftIterator = fieldsOfLeft.begin();
        while(findKeyOfLeft.isEmpty() && leftIterator != fieldsOfLeft.end())
        {
            XField field = (*leftIterator);
            if(field.member() == m_localProperty)
            {
                findKeyOfLeft = QString("%1.%2").arg(tableNameOfLeft).arg(field.name());
            }
            leftIterator++;
        }


        QString tableNameOfIntermediate = I::tableName();
        QList<XField> fieldsOfIntermediated = I::fields();
        QString findKeyIntermediatedForLeft;
        QString findKeyIntermediatedForRight;

        QList<XField>::iterator intermediatedIterator = fieldsOfIntermediated.begin();
        while((findKeyIntermediatedForLeft.isEmpty() || findKeyIntermediatedForRight.isEmpty()) && intermediatedIterator != fieldsOfIntermediated.end())
        {
            XField field = (*intermediatedIterator);
            if(findKeyIntermediatedForLeft.isEmpty() && field.member() == m_localLinkProperty)
            {
                findKeyIntermediatedForLeft = QString("%1.%2").arg(tableNameOfIntermediate).arg(field.name());
            }
            else if(findKeyIntermediatedForRight.isEmpty() && field.member() == m_foreignLinkProperty)
            {
                findKeyIntermediatedForRight = QString("%1.%2").arg(tableNameOfIntermediate).arg(field.name());
            }
            intermediatedIterator++;
        }

        QString tableNameOfRight= R::tableName();
        QList<XField> fieldsOfRight = R::fields();
        QString findKeyOfRight;
        QStringList membersOfRight;
        QStringList fieldNamesOfRight;
        QStringList fieldNamesWithTableNameOfRight;

        foreach(XField field, fieldsOfRight)
        {
            membersOfRight << field.member();
            fieldNamesOfRight << field.name();
            fieldNamesWithTableNameOfRight << QString("%1.%2").arg(tableNameOfRight).arg(field.name());

            if(field.member() == m_foreignProperty)
            {
                findKeyOfRight = QString("%1.%2").arg(tableNameOfRight).arg(field.name());
            }
        }

        QStringList tableNames;
        tableNames << tableNameOfLeft << tableNameOfRight << tableNameOfIntermediate;

        QString compareStatement = QString("%1%2%3%4%5")
                .arg(EQUAL_TEMPLATE.arg(findKeyOfLeft).arg(localPropertyValue.toString()))
                .arg(AND_SEPARATOR_TEMPLATE)
                .arg(EQUAL_FIELD_TEMPLATE.arg(findKeyOfLeft).arg(findKeyIntermediatedForLeft))
                .arg(AND_SEPARATOR_TEMPLATE)
                .arg(EQUAL_FIELD_TEMPLATE.arg(findKeyIntermediatedForRight).arg(findKeyOfRight));

        QString sql = FIND_WITH_FIELD_SELECTION_TEMPLATE
                .arg(fieldNamesWithTableNameOfRight.join(COMMA_TEMPLATE))
                .arg(tableNames.join(COMMA_TEMPLATE))
                .arg(compareStatement);

        QSqlDatabase db = QSqlDatabase::database(connectionName);
        QSqlQuery query(sql, db);

        QList<R*> items;

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;

            foreach (QString fieldName, fieldNamesOfRight)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            while(query.next())
            {
                R* item = new R;

                for(qint16 i = 0; i < membersOfRight.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        item->setProperty(membersOfRight.at(i).toLocal8Bit(),
                                          query.value(columnsIndex.at(i)));
                    }
                }

                items.append(item);
            }

            listData.setValue(items);
        }

        return query.lastError();
    }

    QString listProperty()
    {
        return m_listProperty;
    }

private:
    QString m_foreignProperty;
    QString m_localProperty;
    QString m_foreignLinkProperty;
    QString m_localLinkProperty;
    QString m_listProperty;
};


class XForeignList
{
public:
    explicit XForeignList();
    XForeignList(XForeignListDecl *foreignListDeclObj);
    XForeignList(const XForeignList &other);

    XForeignList& operator=(const XForeignList &other);

    QString localProperty();

    QSqlError resolve(QVariant &listData, const QVariant &localPropertyValue, const QString &connectionName);

    QString listProperty();

private:
    QSharedPointer<XForeignListDecl> m_foreignListDecl;
};

#define X_FOREIGN_LIST(FOREIGN_MODEL_CLASS, FOREIGN_PROPERTY, LOCAL_PROPERTY, LIST_PROPERTY) \
    XForeignList(new XForeignListPriv<FOREIGN_MODEL_CLASS>(#FOREIGN_PROPERTY, #LOCAL_PROPERTY, #LIST_PROPERTY))

#define X_INTERMEDIATED_FOREIGN_LIST(FOREIGN_MODEL_CLASS, FOREIGN_PROPERTY, LOCAL_MODEL_CLASS, LOCAL_PROPERTY, MODEL_CLASS_LINK, FOREIGN_PROPERTY_LINK, LOCAL_PROPERTY_LINK, LIST_PROPERTY) \
    XForeignList(new XIntermediatedForeignListPriv<LOCAL_MODEL_CLASS, MODEL_CLASS_LINK, FOREIGN_MODEL_CLASS>(#FOREIGN_PROPERTY, #LOCAL_PROPERTY, #FOREIGN_PROPERTY_LINK, #LOCAL_PROPERTY_LINK, #LIST_PROPERTY))

#define X_FOREIGN_LISTS(LISTS...) \
    public: \
    static QList<XForeignList> foreignLists() \
{ \
    QList<XForeignList> lists = { LISTS }; \
    return lists; \
    }

#endif // XFIELD_HPP
