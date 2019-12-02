#ifndef XDATABASE_HPP
#define XDATABASE_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

#include "xfield.hpp"
#include "xtablemodel.hpp"
#include "xdao.hpp"


class XDatabase : public QObject
{
    Q_OBJECT

public:    
    explicit XDatabase(QObject *parent = nullptr);
    XDatabase(const QString &storagePath, QObject *parent = nullptr);
    XDatabase(const QString &storagePath, const QString &connectionName, const QString &password = "", QObject *parent = nullptr);

    ~XDatabase();

    template <class T>
    QSqlError registerModel()
    {
        QString tableName = T::tableName();
        QList<XField> fields = T::fields();
        return createTable(tableName, fields);
    }

    bool open();
    bool isOpen();
    void close();

    QSqlError lastError();

    QSqlDatabase* database();

    template <class T>
    XDao<T> dao() {
        return XDao<T>(m_connectionName);
    }

private:
    QSqlDatabase m_database;
    QString m_connectionName;
    QString m_storagePath;
    QString m_password;

    void initialize(const QString &connectionName, const QString &storagePath,
                    const QString &password);

    QString columnType(const QMetaType::Type &type);
    QString foreignBehavior(const XForeignBehavior &behavior);
    QString columnPropertiesClause(const XFieldClauses &clauses);
    bool columnIsPrimaryKey(const XFieldClauses &clauses);
    QString columnForeignClause(const QString &tableName, const QString &fieldName,
                                const XFieldClauses &clauses);
    QString columnUniqueClause(const QString &tableName, const QString &fieldName,
                               const XFieldClauses &clauses);
    QString columnCheckClause(const QString &tableName, const QString &fieldName,
                              const XFieldClauses &clauses);
    QString columnIndexClause(const QString &tableName, const QString &fieldName,
                              const XFieldClauses &clauses);
    QSqlError createTable(const QString &tableName, const QList<XField> &fields);
};

#endif // XDATABASE_HPP
