#ifndef XJSONMODEL_HPP
#define XJSONMODEL_HPP

#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>

#include "xkey.hpp"


template <class T>
class XJsonModel
{
public:
    static QList<XKey> keys()
    {
        const QMetaObject metaObject = T::staticMetaObject;
        quint32 propertyCount = metaObject.propertyCount();
        quint32 propertyOffset = metaObject.propertyOffset();
        QList<XKey> keys;

        for(quint32 i = propertyOffset; i < propertyCount; i++)
        {
            QMetaProperty property = metaObject.property(i);
            XKey key(property.name());
            keys << key;
        }

        return keys;
    }
};

#define X_JSON_KEYS(KEYS...) \
    public: \
    static QList<XKey> keys() \
    { \
        QList<XKey> fields = { KEYS }; \
        return fields; \
    }

#endif // XJSONMODEL_HPP
