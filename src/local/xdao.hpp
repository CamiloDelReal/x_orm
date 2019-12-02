#ifndef XDAO_HPP
#define XDAO_HPP

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "xstringtemplates.hpp"
#include "xfield.hpp"
#include "xquerybuilder.hpp"


typedef QMap<QString, QVariant> XMatcher;


template <class T>
class XDao
{
public:
    explicit XDao()
    {
        prepare();
    }

    XDao(const QSqlDatabase &database)
    {        
        prepare();
        setDatabase(database);
    }

    ~XDao()
    {
        if(m_database.isValid() && m_database.isOpen())
        {
            m_database.close();
        }
    }

    QString connectionName()
    {
        return m_connectionName;
    }

    QSqlDatabase database()
    {
        return m_database;
    }

    void setDatabase(const QSqlDatabase &database)
    {
        if(m_database.isValid() && m_database.isOpen())
        {
            m_database.close();
        }
        m_connectionName = QString("0x%1").arg(reinterpret_cast<quintptr>(this), QT_POINTER_SIZE * 2, 16, QChar('0'))
                + QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss.zzz");
        m_database = QSqlDatabase::cloneDatabase(database, m_connectionName);
        m_database.open();  // Need to store possible errors
    }

    QSqlError createOne(T* item)
    {
        QStringList values;
        foreach (QString member, m_membersWithoutAutoGenKey)
        {
            if(m_converters.contains(member))
            {
                XDataConverterPriv converter = m_converters.value(member);
                values << converter.to(item->property(member.toLocal8Bit().data()))
                          .toString()
                          .prepend(QUOTE_TEMPLATE)
                          .append(QUOTE_TEMPLATE);
            }
            else
            {
                values << item->property(member.toLocal8Bit().data()).toString()
                          .prepend(QUOTE_TEMPLATE)
                          .append(QUOTE_TEMPLATE);
            }
        }
        QString sql = INSERT_ROW_TEMPLATE
                .arg(m_tableName)
                .arg(m_fieldNamesWithoutAutoGenKey.join(COMMA_TEMPLATE))
                .arg(values.join(COMMA_TEMPLATE));

        QSqlQuery query(sql, m_database);

        if(!query.lastError().isValid() && m_autogenPrimaryKey.isValid())
        {
            item->setProperty(m_autogenPrimaryKey.member().toLocal8Bit().data(),
                              query.lastInsertId());
        }

        return query.lastError();
    }

    QSqlError createAll(QList<T*> items)
    {
        QSqlQuery query(m_database);

        for (qint32 i = 0; !query.lastError().isValid() && i < items.size(); i++)
        {
            QStringList values;
            foreach (QString member, m_membersWithoutAutoGenKey)
            {
                if(m_converters.contains(member))
                {
                    XDataConverterPriv converter = m_converters.value(member);
                    values << converter.to(items.at(i)->property(member.toLocal8Bit().data()))
                              .toString()
                              .prepend(QUOTE_TEMPLATE)
                              .append(QUOTE_TEMPLATE);
                }
                else
                {
                    values << items.at(i)->property(member.toLocal8Bit().data())
                              .toString()
                              .prepend(QUOTE_TEMPLATE)
                              .append(QUOTE_TEMPLATE);
                }
            }
            QString sql = INSERT_ROW_TEMPLATE
                    .arg(m_tableName)
                    .arg(m_fieldNamesWithoutAutoGenKey.join(COMMA_TEMPLATE))
                    .arg(values.join(COMMA_TEMPLATE));

            query.exec(sql);

            if(!query.lastError().isValid() && m_autogenPrimaryKey.isValid())
            {
                items.at(i)->setProperty(m_autogenPrimaryKey.member().toLocal8Bit().data(),
                                         query.lastInsertId());
            }
        }

        return query.lastError();
    }

    QSqlError readAll(QList<T*> &items)
    {
        return readBlock(items, -1, -1);
    }

    QSqlError readBlock(QList<T*> &items, const qint32 &offset, const qint32 &blockSize)
    {
        QString sql = SELECT_FROM_WITH_LIMIT_TEMPLATE.arg(m_tableName).arg(offset).arg(blockSize);

        QSqlQuery query(sql, m_database);

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;
            foreach (QString fieldName, m_fieldNames)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            while(query.next())
            {
                T* item = new T;

                for(qint16 i = 0; i < m_members.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        if(m_converters.contains(m_members.at(i)))
                        {
                            XDataConverterPriv converter = m_converters.value(m_members.at(i));
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              converter.from(query.value(columnsIndex.at(i))));
                        }
                        else
                        {
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              query.value(columnsIndex.at(i)));
                        }
                    }
                }

                items.append(item);
            }
        }

        return query.lastError();
    }

    QSqlError updateOne(T* item)
    {
        QStringList setValueStatements;
        QStringList keys;

        if(m_autogenPrimaryKey.isValid())
        {
            for(quint16 i = 0; i < m_membersWithoutAutoGenKey.size(); i++)
            {
                setValueStatements <<  SET_FIELD_TEMPLATE
                                       .arg(m_fieldNamesWithoutAutoGenKey.at(i))
                                       .arg(item->property(m_membersWithoutAutoGenKey.at(i).toLocal8Bit().data()).toString());
            }

            keys << EQUAL_TEMPLATE
                    .arg(m_autogenPrimaryKey.name())
                    .arg(item->property(m_autogenPrimaryKey.member().toLocal8Bit().data()).toString());
        }
        else
        {
            for(quint16 i = 0; i < m_membersWithoutKeys.size(); i++)
            {
                setValueStatements <<  SET_FIELD_TEMPLATE
                                       .arg(m_fieldNamesWithoutKeys.at(i))
                                       .arg(item->property(m_membersWithoutKeys.at(i).toLocal8Bit().data()).toString());
            }

            for(quint16 i = 0; i < m_primariesKeys.size(); i++)
            {
                keys << EQUAL_TEMPLATE
                        .arg(m_primariesKeys.at(i).name())
                        .arg(item->property(m_primariesKeys.at(i).member().toLocal8Bit().data()).toString());
            }
        }

        QString sql = UPDATE_ROW_TEMPLATE
                .arg(m_tableName)
                .arg(setValueStatements.join(COMMA_TEMPLATE))
                .arg(keys.join(AND_SEPARATOR_TEMPLATE));

        QSqlQuery query(sql, m_database);

        return query.lastError();
    }

    QSqlError updateAll(const QList<T*> &items)
    {
        QSqlQuery query(m_database);

        for (qint32 i = 0; !query.lastError().isValid() && i < items.size(); i++)
        {
            QStringList setValueStatements;
            QStringList keysStatements;

            if(m_autogenPrimaryKey.isValid())
            {
                QStringList innerSetValueStatements;


                for(qint16 j = 0; j < m_membersWithoutAutoGenKey.size(); j++)
                {
                    setValueStatements <<  SET_FIELD_TEMPLATE
                                           .arg(m_fieldNamesWithoutAutoGenKey.at(j))
                                           .arg(items.at(i)->property(m_membersWithoutAutoGenKey.at(j).toLocal8Bit().data()).toString());
                }

                keysStatements << EQUAL_TEMPLATE
                                  .arg(m_autogenPrimaryKey.name())
                                  .arg(items.at(i)->property(m_autogenPrimaryKey.member().toLocal8Bit().data()).toString());
            }
            else
            {
                for(qint16 j = 0; j < m_membersWithoutKeys.size(); j++)
                {
                    setValueStatements <<  SET_FIELD_TEMPLATE
                                           .arg(m_fieldNamesWithoutKeys.at(j))
                                           .arg(items.at(i)->property(m_membersWithoutKeys.at(j).toLocal8Bit().data()).toString());
                }

                for(quint16 j = 0; j < m_primariesKeys.size(); j++)
                {
                    keysStatements << EQUAL_TEMPLATE
                                      .arg(m_primariesKeys.at(j).name())
                                      .arg(items.at(i)->property(m_primariesKeys.at(j).member().toLocal8Bit().data()).toString());
                }
            }

            QString sql = UPDATE_ROW_TEMPLATE
                    .arg(m_tableName)
                    .arg(setValueStatements.join(COMMA_TEMPLATE))
                    .arg(keysStatements.join(AND_SEPARATOR_TEMPLATE));

            query.exec(sql);
        }

        return query.lastError();
    }

    QSqlError deleteOne(T* item)
    {
        QStringList keysStatements;

        if(m_autogenPrimaryKey.isValid())
        {
            keysStatements << EQUAL_TEMPLATE
                              .arg(m_autogenPrimaryKey.name())
                              .arg(item->property(m_autogenPrimaryKey.member().toLocal8Bit().data()).toString());
        }
        else
        {
            for(quint16 i = 0; i < m_primariesKeys.size(); i++)
            {
                keysStatements << EQUAL_TEMPLATE
                                  .arg(m_primariesKeys.at(i).name())
                                  .arg(item->property(m_primariesKeys.at(i).member().toLocal8Bit().data()).toString());
            }
        }

        QString sql = DELETE_ROW_TEMPLATE
                .arg(m_tableName)
                .arg(keysStatements.join(AND_SEPARATOR_TEMPLATE));

        QSqlQuery query(m_database);
        query.exec(ENABLE_FOREIGN_KEYS_TEMPLATES);
        query.exec(sql);

        if(!query.lastError().isValid() && m_autogenPrimaryKey.isValid())
        {
            item->setProperty(m_autogenPrimaryKey.member().toLocal8Bit().data(), 0);
        }

        return query.lastError();

    }

    QSqlError deleteAll(const QList<T*> &items)
    {
        QStringList keysStatements;

        if(m_autogenPrimaryKey.isValid())
        {
            foreach (T* item, items)
            {
                keysStatements << EQUAL_TEMPLATE
                                  .arg(m_autogenPrimaryKey.name())
                                  .arg(item->property(m_autogenPrimaryKey.member().toLocal8Bit().data()).toString());
            }
        }
        else
        {
            foreach (T* item, items)
            {
                QStringList innerKeysStatements;

                for(quint16 i = 0; i < m_primariesKeys.size(); i++)
                {
                    innerKeysStatements << EQUAL_TEMPLATE
                                           .arg(m_primariesKeys.at(i).name())
                                           .arg(item->property(m_primariesKeys.at(i).member().toLocal8Bit().data()).toString());
                }

                keysStatements << innerKeysStatements.join(AND_SEPARATOR_TEMPLATE);
            }
        }

        QSqlQuery query(m_database);
        query.exec(ENABLE_FOREIGN_KEYS_TEMPLATES);

        for(quint32 i = 0; !query.lastError().isValid() && i < items.size(); i++)
        {
            QString sql = DELETE_ROW_TEMPLATE
                    .arg(m_tableName)
                    .arg(keysStatements.at(i));

            query.exec(sql);

            if(!query.lastError().isValid() && m_autogenPrimaryKey.isValid())
            {
                items.at(i)->setProperty(m_autogenPrimaryKey.member().toLocal8Bit().data(), 0);
            }
        }

        return query.lastError();
    }

    QSqlError deleteBy(const XMatcher &keyValuesFinder, const bool &keysAreProperties = true)
    {
        QStringList findStatements;
        QStringList keys = keyValuesFinder.keys();

        if(keysAreProperties)
        {
            foreach (QString key, keys)
            {
                qint32 index = m_members.indexOf(key);

                findStatements << EQUAL_TEMPLATE.arg(m_fieldNames.at(index))
                                  .arg(keyValuesFinder.value(key)
                                       .toString());
            }
        }
        else
        {
            foreach (QString key, keys)
            {
                findStatements << EQUAL_TEMPLATE.arg(key)
                                  .arg(keyValuesFinder.value(key)
                                       .toString());
            }
        }

        QString compareStatement = findStatements.join(AND_SEPARATOR_TEMPLATE);
        QString sql = DELETE_ROW_TEMPLATE.arg(m_tableName).arg(compareStatement);

        QSqlQuery query(m_database);
        query.exec(ENABLE_FOREIGN_KEYS_TEMPLATES);
        query.exec(sql);

        return query.lastError();
    }

    QSqlError resolveForeigns(T* item)
    {
        QSqlError error;

        QList<XForeignObject> foreignObjects = T::foreignObjects();

        for(qint32 i = 0; !error.isValid() && i < foreignObjects.size(); i++)
        {
            XForeignObject fObj = foreignObjects.at(i);
            QVariant data;
            error = fObj.resolve(data, item->property(fObj.localProperty().toLocal8Bit().data()), m_connectionName);
            if(!error.isValid())
            {
                item->setProperty(fObj.objectProperty().toLocal8Bit().data(), data);
            }
        }

        if(!error.isValid())
        {
            QList<XForeignList> foreignLists = T::foreignLists();

            for(qint32 i = 0; !error.isValid() && i < foreignLists.size(); i++)
            {
                XForeignList fList = foreignLists.at(i);
                QVariant data;
                error = fList.resolve(data, item->property(fList.localProperty().toLocal8Bit().data()), m_connectionName);
                if(!error.isValid())
                {
                    item->setProperty(fList.listProperty().toLocal8Bit().data(), data);
                }
            }
        }

        return error;
    }

    QSqlError query(const XQueryBuilder<T> &queryBuilder, QList<T*> &items)
    {
        return query(queryBuilder.raw(), items);
    }

    QSqlError query(const QString &rawQuery, QList<T*> &items)
    {
        QSqlQuery query(rawQuery, m_database);

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;
            foreach (QString fieldName, m_fieldNames)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            while(query.next())
            {
                T* item = new T;

                for(qint16 i = 0; i < m_members.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        if(m_converters.contains(m_members.at(i)))
                        {
                            XDataConverterPriv converter = m_converters.value(m_members.at(i));
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              converter.from(query.value(columnsIndex.at(i))));
                        }
                        else
                        {
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              query.value(columnsIndex.at(i)));
                        }
                    }
                }

                items.append(item);
            }
        }

        return query.lastError();
    }

    QSqlError findOneBy(T* &item, const XMatcher &keyValuesFinder, const bool &keysAreProperties = true)
    {
        QStringList findStatements;
        QStringList keys = keyValuesFinder.keys();

        if(keysAreProperties)
        {
            foreach (QString key, keys)
            {
                qint32 index = m_members.indexOf(key);

                findStatements << EQUAL_TEMPLATE.arg(m_fieldNames.at(index))
                                  .arg(keyValuesFinder.value(key)
                                       .toString());
            }
        }
        else
        {
            foreach (QString key, keys)
            {
                findStatements << EQUAL_TEMPLATE.arg(key)
                                  .arg(keyValuesFinder.value(key)
                                       .toString());
            }
        }

        QString compareStatement = findStatements.join(AND_SEPARATOR_TEMPLATE);
        QString sql = FIND_FIRST_TEMPLATE.arg(m_tableName).arg(compareStatement);

        QSqlQuery query(sql, m_database);

        if(!query.lastError().isValid())
        {
            QList<qint32> columnsIndex;
            foreach (QString fieldName, m_fieldNames)
            {
                columnsIndex << query.record().indexOf(fieldName);
            }

            if(query.next())
            {
                item = new T;

                for(qint16 i = 0; i < m_members.size(); i++)
                {
                    if(columnsIndex.at(i) > -1)
                    {
                        if(m_converters.contains(m_members.at(i)))
                        {
                            XDataConverterPriv converter = m_converters.value(m_members.at(i));
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              converter.from(query.value(columnsIndex.at(i))));
                        }
                        else
                        {
                            item->setProperty(m_members.at(i).toLocal8Bit(),
                                              query.value(columnsIndex.at(i)));
                        }
                    }
                }
            }
        }

        return query.lastError();
    }

//    QSqlError findBlock(QList<T*> &items, const QList<XFieldMatcher> matchers, const qint32 &offset, const qint32 &blockSize)
//    {
//        QStringList findStatements;

//        foreach(XFieldMatcher matcher, matchers)
//        {
//            QString fieldName;

//            if(matcher.isProperty())
//            {
//                qint32 index = m_members.indexOf(matcher.name());
//                if(index != -1)
//                {
//                    fieldName = m_fieldNames.at(index);
//                }
//            }
//            else
//            {
//                fieldName = matcher.name();
//            }

//            if(matcher.isLike())
//            {
//                findStatements << LIKE_TEMPLATE.arg(fieldName)
//                                  .arg(matcher.value()
//                                       .toString());
//            }
//            else
//            {
//                findStatements << EQUAL_TEMPLATE.arg(fieldName)
//                                  .arg(matcher.value()
//                                       .toString());
//            }
//        }

//        QString compareStatement = findStatements.join(AND_SEPARATOR_TEMPLATE);
//        QString sql = FIND_BLOCK_TEMPLATE.arg(m_tableName).arg(compareStatement).arg(offset).arg(blockSize);

//        QSqlQuery query(sql, m_database);

//        if(!query.lastError().isValid())
//        {
//            QList<qint32> columnsIndex;
//            foreach (QString fieldName, m_fieldNames)
//            {
//                columnsIndex << query.record().indexOf(fieldName);
//            }

//            while(query.next())
//            {
//                T* item = new T;

//                for(qint16 i = 0; i < m_members.size(); i++)
//                {
//                    if(columnsIndex.at(i) > -1)
//                    {
//                        if(m_converters.contains(m_members.at(i)))
//                        {
//                            XDataConverterPriv converter = m_converters.value(m_members.at(i));
//                            item->setProperty(m_members.at(i).toLocal8Bit(),
//                                              converter.from(query.value(columnsIndex.at(i))));
//                        }
//                        else
//                        {
//                            item->setProperty(m_members.at(i).toLocal8Bit(),
//                                              query.value(columnsIndex.at(i)));
//                        }
//                    }
//                }

//                items.append(item);
//            }
//        }

//        return query.lastError();
//    }

private:
    // Database access
    QSqlDatabase m_database;
    QString m_connectionName;
    // Instance data cache
    QString m_tableName;
    QList<XField> m_fields;
    QList<XField> m_primariesKeys;
    XField m_autogenPrimaryKey;
    QMap<QString, XDataConverterPriv> m_converters;
    // Parallel data
    QStringList m_members;
    QStringList m_fieldNames;
    // Parallel data
    QStringList m_membersWithoutAutoGenKey;
    QStringList m_fieldNamesWithoutAutoGenKey;
    // Parallel data
    QStringList m_membersWithoutKeys;
    QStringList m_fieldNamesWithoutKeys;

    void prepare()
    {
        m_tableName = T::tableName();
        m_fields = T::fields();
        m_primariesKeys = T::primariesKeys(m_fields);
        m_autogenPrimaryKey = T::autogenPrimaryKey(m_primariesKeys);

        foreach (XField field, m_fields)
        {
            m_members << field.member();
            m_fieldNames << field.name();

            if(!m_autogenPrimaryKey.isValid() || (field.name() != m_autogenPrimaryKey.name()))
            {
                m_membersWithoutAutoGenKey << field.member();
                m_fieldNamesWithoutAutoGenKey << field.name();
            }

            if(!field.clauses().hasClause(PRIMARY_KEY)
                    && !field.clauses().hasClause(PRIMARY_KEY))
            {
                m_membersWithoutKeys << field.member();
                m_fieldNamesWithoutKeys << field.name();
            }

            if(field.clauses().hasClause(DATA_CONVERTER))
            {
                m_converters.insert(field.member(), field.clauses().clauseValue(DATA_CONVERTER).value<XDataConverterPriv>());
            }
        }
    }
};

#endif // XDAO_HPP
