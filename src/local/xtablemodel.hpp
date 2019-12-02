#ifndef XTABLEMODEL_HPP
#define XTABLEMODEL_HPP

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QMetaObject>
#include <QMetaProperty>

#include "xfield.hpp"


template <class T>
class XTableModel
{
public:
    static QMetaObject qMetaObject()
    {
        return T::staticMetaObject;
    }

    static QString tableName()
    {
        return T::staticMetaObject.className();
    }

    static QList<XField> fields()
    {
        const QMetaObject metaObject = T::staticMetaObject;
        quint32 propertyCount = metaObject.propertyCount();
        quint32 propertyOffset = metaObject.propertyOffset();
        QList<XField> fields;

        for(quint32 i = propertyOffset; i < propertyCount; i++)
        {
            QMetaProperty property = metaObject.property(i);
            XField field(property.name(), static_cast<QMetaType::Type>(property.type()));
            fields << field;
        }

        return fields;
    }

    static QList<XField> primariesKeys(const QList<XField> &allFields = QList<XField>())
    {
        QList<XField> fields;

        if(allFields.isEmpty())
            fields = T::fields();
        else
            fields = allFields;

        QList<XField> pks;

        foreach (XField field, fields)
        {
            XFieldClauses clauses = field.clauses();
            if(clauses.hasClause(PRIMARY_KEY)
                    || clauses.hasClause(PRIMARY_KEY_AUTOINCREMENT))
            {
                pks << field;
            }
        }

        return pks;
    }

    static XField autogenPrimaryKey(const QList<XField> &allPrimariesKeys = QList<XField>())
    {
        QList<XField> pks;

        if(allPrimariesKeys.isEmpty())
            pks = T::primariesKeys();
        else
            pks = allPrimariesKeys;

        QListIterator<XField> allPksIterator(pks);
        XField autogen;
        allPksIterator.toBack();
        bool found = false;

        while (!found && allPksIterator.hasPrevious())
        {
            XField field = allPksIterator.previous();
            if(field.type() == QMetaType::Int || field.type() == QMetaType::UInt
                    || field.type() == QMetaType::LongLong || field.type() == QMetaType::ULongLong)
            {
                autogen = field;
                found = true;
            }
        }

        return autogen;
    }

    static QList<XForeignObject> foreignObjects()
    {
        return QList<XForeignObject>();
    }

    static QList<XForeignList> foreignLists()
    {
        return QList<XForeignList>();
    }
};

#define X_TABLE_NAME(NAME) \
    public: \
    static QString tableName() \
    { \
        return NAME; \
    }

#define X_TABLE_FIELDS(FIELDS...) \
    public: \
    static QList<XField> fields() \
    { \
        QList<XField> fields = { FIELDS }; \
        const QMetaObject meta = qMetaObject(); \
        for(qint32 i = 0; i < fields.size(); i++) \
        { \
            XField field = fields.at(i); \
            field.setType(static_cast<QMetaType::Type>(meta.property(meta.indexOfProperty(field.member().toLocal8Bit().data())).type())); \
            fields.replace(i, field); \
        } \
        return fields; \
    }


#endif // XTABLEMODEL_HPP
