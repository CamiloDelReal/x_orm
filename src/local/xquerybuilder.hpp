#ifndef XQUERYBUILDER_HPP
#define XQUERYBUILDER_HPP

#include <QObject>
#include <QVariant>

#include "xfield.hpp"


class XComparisson
{
public:
    enum XComparissonType
    {
        EQUAL = 0,
        GREAT_THAN,
        GREAT_OR_EQUAL_THAN,
        LESS_THAN,
        LESS_OR_EQUAL_THAN,
        LIKE,
        IN,
        BETWEEN
    };

    explicit XComparisson();
    XComparisson(const QString &columnName, const XComparissonType &type, const QVariant &value);
    XComparisson(const XComparisson &other);

    XComparisson& operator=(const XComparisson &other);

    QString raw() const;

private:
    QString m_columnName;
    XComparissonType m_type;
    QVariant m_value;
};


class XWhere
{
public:
    enum XLogicalOperator
    {
        AND = 0,
        OR,
        AND_NOT,
        OR_NOT
    };

    explicit XWhere();
    XWhere(const XWhere &other);

    XWhere& operator=(const XWhere &other);

    XWhere& appendEqual(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendGreatOrEqualThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendGreatThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendLessOrEqualThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendLessThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendLike(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty = true);
    XWhere& appendIn(const QString &columnOrPropertyName, const QVariantList &values, const bool &isProperty = true);
    XWhere& appendBetween(const QString &columnOrPropertyName, const QVariant &lowValue, const QVariant &highValue, const bool &isProperty = true);

    XWhere& appendAnd();
    XWhere& appendAndNot();
    XWhere& appendOr();
    XWhere& appendOrNot();

    void setColumnByPropertyName(const QMap<QString, QString> &columnByPropertyName);
    bool hasClauses() const;

    QString raw() const;

private:
    QMap<QString, QString> m_columnByPropertyName;
    QList<XComparisson> m_comparissons;
    QList<XLogicalOperator> m_operators;
};


template <class T>
class XQueryBuilder
{
public:
    explicit XQueryBuilder()
    {
        m_limit = -1;
        m_offset = -1;
        prepareColumnByPropertyName();
    }

    XQueryBuilder(const XQueryBuilder &other)
    {
        m_columnByPropertyName = other.m_columnByPropertyName;
        m_columns = other.m_columns;
        m_groupBy = other.m_groupBy;
        m_limit = other.m_limit;
        m_offset = other.m_offset;
        m_where = other.m_where;
        prepareColumnByPropertyName();
    }

    XQueryBuilder& operator=(const XQueryBuilder &other)
    {
        m_columnByPropertyName = other.m_columnByPropertyName;
        m_columns = other.m_columns;
        m_groupBy = other.m_groupBy;
        m_limit = other.m_limit;
        m_offset = other.m_offset;
        m_where = other.m_where;

        return *this;
    }

    XQueryBuilder& appendColumn(const QString &columnOrPropertyName, const bool &isProperty = true)
    {
        if(isProperty)
        {
            m_columns.append(m_columnByPropertyName.value(columnOrPropertyName));
        }
        else
        {
            m_columns.append(columnOrPropertyName);
        }
        return *this;
    }

    XQueryBuilder& appendGroupBy(const QString &columnName)
    {
        m_groupBy.append(columnName);
        return *this;
    }

    XQueryBuilder& setLimit(const qint32 &limit)
    {
        m_limit = limit;
        return *this;
    }

    XQueryBuilder& setOffset(const qint32 &offset)
    {
        m_offset = offset;
        return *this;
    }

    XWhere& where()
    {
        return m_where;
    }

    QString raw() const
    {
        QString rawStr;
        if(!m_columns.isEmpty())
        {
            rawStr += SELECT_TEMPLATE.arg(m_columns.join(COMMA_TEMPLATE));
        }
        else
        {
            rawStr += SELECT_TEMPLATE.arg(ASTERISK_TEMPLATE);
        }
        rawStr += FROM_TEMPLATE.arg(T::tableName());
        if(m_where.hasClauses())
        {
            rawStr += WHERE_TEMPLATE.arg(m_where.raw());
        }
        if(!m_groupBy.isEmpty())
        {
            rawStr += GROUP_BY_TEMPLATE.arg(m_groupBy.join(COMMA_TEMPLATE));
        }
//        rawStr += LIMIT_TEMPLATE.arg(m_offset).arg(m_limit);
        return rawStr;
    }

private:
    QMap<QString, QString> m_columnByPropertyName;
    QStringList m_columns;
    QStringList m_groupBy;
    qint32 m_limit;
    qint32 m_offset;
    XWhere m_where;

    void prepareColumnByPropertyName()
    {
        QList<XField> m_fields = T::fields();
        foreach (XField field, m_fields)
        {
            m_columnByPropertyName.insert(field.member(), field.name());
        }
        m_where.setColumnByPropertyName(m_columnByPropertyName);
    }
};

#endif // XQUERYBUILDER_HPP
