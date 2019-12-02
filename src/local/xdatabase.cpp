#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>

#include "xdatabase.hpp"
#include "xstringtemplates.hpp"


XDatabase::XDatabase(QObject *parent)
    : QObject(parent)
{
    initialize(DEFAULT_CONNECTION_NAME, QStringLiteral("./x_orm_database.sqlite3"), EMPTY_TEMPLATE);
}

XDatabase::XDatabase(const QString &storagePath, QObject *parent)
    : QObject(parent)
{
    initialize(DEFAULT_CONNECTION_NAME, storagePath, EMPTY_TEMPLATE);
}

XDatabase::XDatabase(const QString &storagePath, const QString &connectionName, const QString &password, QObject *parent)
    : QObject(parent)
{
    initialize(connectionName, storagePath, password);
}

void XDatabase::initialize(const QString &connectionName, const QString &storagePath,
                           const QString &password)
{
    m_connectionName = connectionName;
    m_storagePath = storagePath;
    m_password = password;
    QString driver = m_password.isNull() || m_password.isEmpty()
            ? QSQLITE_DRIVER : QSQLITE_CIPHER_DRIVER;

    m_database = QSqlDatabase::addDatabase(driver, m_connectionName);
    m_database.setDatabaseName(m_storagePath);
    m_database.setPassword(m_password);
}

XDatabase::~XDatabase()
{
    if(m_database.isOpen())
        m_database.close();
}

bool XDatabase::open()
{
    bool opened = false;

    if(!m_password.isNull() && !m_password.isEmpty())
        opened = m_database.open(EMPTY_TEMPLATE, m_password);
    else
        opened = m_database.open();

    return opened;
}

bool XDatabase::isOpen()
{
    return m_database.isOpen();
}

void XDatabase::close()
{
    m_database.close();
}

QSqlError XDatabase::lastError()
{
    return m_database.lastError();
}

QSqlDatabase* XDatabase::database()
{
    return &m_database;
}

QString XDatabase::columnType(const QMetaType::Type &type)
{
    QString columnType;

    switch(type)
    {
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        columnType = INTEGER_TEMPLATE;
        break;
    case QMetaType::Float:
    case QMetaType::Double:
        columnType = DOUBLE_TEMPLATE;
        break;
    case QMetaType::Bool:
        columnType = BOOLEAN_TEMPLATE;
        break;
    case QMetaType::Char:
    case QMetaType::QString:
        columnType = TEXT_TEMPLATE;
        break;
    case QMetaType::QByteArray:
        columnType = BLOB_TEMPLATE;
        break;
    case QMetaType::QDate:
        columnType = DATE_TEMPLATE;
        break;
    case QMetaType::QTime:
        columnType = TIME_TEMPLATE;
        break;
    case QMetaType::QDateTime:
        columnType = DATETIME_TEMPLATE;
        break;
    default:
        columnType = TEXT_TEMPLATE;
    }

    return columnType;
}

QString XDatabase::foreignBehavior(const XForeignBehavior &behavior)
{
    QString behaviorStr;

    switch(behavior)
    {
    case NO_ACTION:
        behaviorStr = NO_ACTION_TEMPLATE;
        break;
    case RESTRIC:
        behaviorStr = RESTRIC_TEMPLATE;
        break;
    case CASCADE:
        behaviorStr = CASCADE_TEMPLATE;
        break;
    case SET_NULL:
        behaviorStr = SET_NULL_TEMPLATE;
        break;
    case SET_DEFAULT:
        behaviorStr = SET_DEFAULT_TEMPLATE;
        break;
    }

    return behaviorStr;
}

QString XDatabase::columnPropertiesClause(const XFieldClauses &clauses)
{
    QStringList propertiesClause;

    if(clauses.hasClause(NOT_NULL))
        propertiesClause << NOT_NULL_TEMPLATE;

    if(clauses.hasClause(DEFAULT))
        propertiesClause << DEFAULT_TEMPLATE.arg(clauses.clauseValue(DEFAULT).toString());

    return propertiesClause.join(SPACE_TEMPLATE);
}

bool XDatabase::columnIsPrimaryKey(const XFieldClauses &clauses)
{
    return (clauses.hasClause(PRIMARY_KEY) || clauses.hasClause(PRIMARY_KEY_AUTOINCREMENT));
}

QString XDatabase::columnForeignClause(const QString &tableName, const QString &fieldName,
                                       const XFieldClauses &clauses)
{
    QString foreignClause;

    if(clauses.hasClause(FOREIGN_KEY))
    {
        XForeignKey fKey = clauses.clauseValue(FOREIGN_KEY).value<XForeignKey>();
        if(!fKey.referencedTable().isNull() && !fKey.referencedTable().isEmpty()
                && !fKey.referencedField().isNull() && !fKey.referencedField().isNull())
        {
            QString fkName = FOREIGN_NAME_TEMPLATE.arg(tableName.toLower()).arg(fKey.referencedField());
            foreignClause = FOREIGN_KEY_TEMPLATE
                    .arg(FOREIGN_NAME_TEMPLATE.arg(fKey.referencedTable()).arg(fKey.referencedField()).arg(fieldName))
                    .arg(fieldName).arg(fKey.referencedTable()).arg(fKey.referencedField())
                    .arg(foreignBehavior(fKey.onUpdate()))
                    .arg(foreignBehavior(fKey.onDelete()));
        }
    }

    return foreignClause;
}

QString XDatabase::columnUniqueClause(const QString &tableName, const QString &fieldName,
                                      const XFieldClauses &clauses)
{
    QString uniqueClause;
    QString uniqueName = UNIQUE_NAME_TEMPLATE.arg(tableName).arg(fieldName);

    if(clauses.hasClause(UNIQUE))
        uniqueClause = UNIQUE_TEMPLATE.arg(uniqueName).arg(fieldName);
    else if(clauses.hasClause(UNIQUE_ASC))
        uniqueClause = UNIQUE_ASC_TEMPLATE.arg(uniqueName).arg(fieldName);
    else if(clauses.hasClause(UNIQUE_DESC))
        uniqueClause = UNIQUE_DESC_TEMPLATE.arg(uniqueName).arg(fieldName);

    return uniqueClause;
}

QString XDatabase::columnCheckClause(const QString &tableName, const QString &fieldName, const XFieldClauses &clauses)
{
    QString checkClause;
    QString checkName = CHECK_NAME_TEMPLATE.arg(tableName).arg(fieldName);

    if(clauses.hasClause(CHECK))
        checkClause = CHECK_TEMPLATE.arg(checkName).arg(clauses.clauseValue(CHECK).toString());

    return checkClause;
}

QString XDatabase::columnIndexClause(const QString &tableName, const QString &fieldName,
                                     const XFieldClauses &clauses)
{
    QString indexClause;
    QString indexName = INDEX_NAME_TEMPLATE.arg(tableName).arg(fieldName);

    if(clauses.hasClause(INDEX_ASC))
        indexClause = CREATE_INDEX_ASC_TEMPLATE.arg(indexName).arg(tableName).arg(fieldName);
    else if(clauses.hasClause(INDEX_DESC))
        indexClause = CREATE_INDEX_DESC_TEMPLATE.arg(indexName).arg(tableName).arg(fieldName);
    else if(clauses.hasClause(INDEX_UNIQUE_ASC))
        indexClause = CREATE_INDEX_UNIQUE_ASC_TEMPLATE.arg(indexName).arg(tableName).arg(fieldName);
    else if(clauses.hasClause(INDEX_UNIQUE_DESC))
        indexClause = CREATE_INDEX_UNIQUE_DESC_TEMPLATE.arg(indexName).arg(tableName).arg(fieldName);

    return indexClause;
}

QSqlError XDatabase::createTable(const QString &tableName, const QList<XField> &fields)
{
    QStringList columnStatements;
    QStringList primaryKeyFields;
    QStringList foreignKeyStatements;
    QStringList uniqueStatements;
    QStringList checkStatements;
    QStringList indexStatements;
    foreach (XField field, fields)
    {
        columnStatements << FIELD_TEMPLATE
                            .arg(field.name())
                            .arg(columnType(field.type()))
                            .arg(columnPropertiesClause(field.clauses()));

        if(columnIsPrimaryKey(field.clauses()))
            primaryKeyFields << field.name();

        QString foreignClause = columnForeignClause(tableName, field.name(), field.clauses());
        if(!foreignClause.isNull() && !foreignClause.isEmpty())
            foreignKeyStatements << foreignClause;

        QString uniqueClause = columnUniqueClause(tableName, field.name(), field.clauses());
        if(!uniqueClause.isNull() && !uniqueClause.isEmpty())
            uniqueStatements << uniqueClause;

        QString checkClause = columnCheckClause(tableName, field.name(), field.clauses());
        if(!checkClause.isNull() && !checkClause.isEmpty())
            checkStatements << checkClause;

        QString indexClause = columnIndexClause(tableName, field.name(), field.clauses());
        if(!indexClause.isNull() && !indexClause.isEmpty())
            indexStatements << indexClause;
    }

    if(!primaryKeyFields.isEmpty())
    {
        QString primaryKeyStatement = MULTI_PRIMARY_KEY_TEMPLATE
                .arg(primaryKeyFields.join(COMMA_LINEBREAK_TEMPLATE));
        columnStatements.append(primaryKeyStatement);
    }
    columnStatements.append(foreignKeyStatements);
    columnStatements.append(uniqueStatements);
    columnStatements.append(checkStatements);

    QString createTableSql = CREATE_TABLE_TEMPLATE
            .arg(tableName)
            .arg(columnStatements.join(COMMA_LINEBREAK_TEMPLATE));

    QSqlQuery query(m_database);
    query.exec(createTableSql);

    QListIterator<QString> indexStatementsIterator(indexStatements);

    while(!query.lastError().isValid() && indexStatementsIterator.hasNext())
    {
        query.exec(indexStatementsIterator.next());
    }

    return query.lastError();
}
