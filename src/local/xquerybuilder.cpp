#include "xquerybuilder.hpp"
#include "xstringtemplates.hpp"


XComparisson::XComparisson()
{
}

XComparisson::XComparisson(const QString &columnName, const XComparisson::XComparissonType &type, const QVariant &value)
{
    m_columnName = columnName;
    m_type = type;
    m_value = value;
}

XComparisson::XComparisson(const XComparisson &other)
{
    m_columnName = other.m_columnName;
    m_type = other.m_type;
    m_value = other.m_value;
}

XComparisson& XComparisson::operator=(const XComparisson &other)
{
    m_columnName = other.m_columnName;
    m_type = other.m_type;
    m_value = other.m_value;

    return *this;
}

QString XComparisson::raw() const
{
    QString rawStr;

    switch(m_type)
    {
        case EQUAL:
        {
            rawStr = EQUAL_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case GREAT_THAN:
        {
            rawStr = GREAT_THAN_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case GREAT_OR_EQUAL_THAN:
        {
            rawStr = GREAT_AND_EQUAL_THAN_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case LESS_THAN:
        {
            rawStr = LESS_THAN_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case LESS_OR_EQUAL_THAN:
        {
            rawStr = LESS_AND_EQUAL_THAN_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case LIKE:
        {
            rawStr = LIKE_TEMPLATE
                    .arg(m_columnName)
                    .arg(m_value.toString());
            break;
        }
        case IN:
        {
            QVariantList values = m_value.value<QVariantList>();
            if(values.size() > 0)
            {
                QString preparedValue = EMPTY_TEMPLATE;
                for(qint32 i = 0; i < values.size(); i++)
                {
                    preparedValue.append(values.at(i)
                                         .toString()
                                         .prepend(QUOTE_TEMPLATE)
                                         .append(QUOTE_TEMPLATE));
                    if(i < (values.size() - 1))
                    {
                        preparedValue.append(COMMA_TEMPLATE);
                    }
                }
                rawStr = IN_TEMPLATE
                        .arg(m_columnName)
                        .arg(preparedValue);
            }
            break;
        }
        case BETWEEN:
        {
            QVariantList values = m_value.value<QVariantList>();
            if(values.size() == 2)
            {
                rawStr = BETWEEN_TEMPLATE
                        .arg(m_columnName)
                        .arg(values.at(0).toString())
                        .arg(values.at(1).toString());
            }
            break;
        }
    }

    return rawStr;
}


XWhere::XWhere()
{
}

XWhere::XWhere(const XWhere &other)
{
    m_comparissons = other.m_comparissons;
    m_operators = other.m_operators;
    m_columnByPropertyName = other.m_columnByPropertyName;
}

XWhere& XWhere::operator=(const XWhere &other)
{
    m_comparissons = other.m_comparissons;
    m_operators = other.m_operators;
    m_columnByPropertyName = other.m_columnByPropertyName;

    return *this;
}

XWhere& XWhere::appendEqual(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::EQUAL, value));
    return *this;
}

XWhere& XWhere::appendGreatOrEqualThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::GREAT_OR_EQUAL_THAN, value));
    return *this;
}

XWhere& XWhere::appendGreatThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::GREAT_THAN, value));
    return *this;
}

XWhere& XWhere::appendLessOrEqualThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::LESS_OR_EQUAL_THAN, value));
    return *this;
}

XWhere& XWhere::appendLessThan(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::LESS_THAN, value));
    return *this;
}

XWhere& XWhere::appendLike(const QString &columnOrPropertyName, const QVariant &value, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::LIKE, value));
    return *this;
}

XWhere& XWhere::appendIn(const QString &columnOrPropertyName, const QVariantList &values, const bool &isProperty)
{
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::IN, QVariant::fromValue(values)));
    return *this;
}

XWhere& XWhere::appendBetween(const QString &columnOrPropertyName, const QVariant &lowValue, const QVariant &highValue, const bool &isProperty)
{
    QVariantList values;
    values.append(lowValue);
    values.append(highValue);
    m_comparissons.append(XComparisson(isProperty
                                       ? m_columnByPropertyName.value(columnOrPropertyName)
                                       : columnOrPropertyName,
                                       XComparisson::BETWEEN, values));
    return *this;
}

XWhere& XWhere::appendAnd()
{
    m_operators.append(AND);
    return *this;
}

XWhere& XWhere::appendAndNot()
{
    m_operators.append(AND_NOT);
    return *this;
}

XWhere& XWhere::appendOr()
{
    m_operators.append(OR);
    return *this;
}

XWhere& XWhere::appendOrNot()
{
    m_operators.append(OR_NOT);
    return *this;
}

void XWhere::setColumnByPropertyName(const QMap<QString, QString> &columnByPropertyName)
{
    m_columnByPropertyName = columnByPropertyName;
}

bool XWhere::hasClauses() const
{
    return (!m_comparissons.isEmpty());
}

QString XWhere::raw() const
{
    QString rawStr;

    if(m_comparissons.size() > 0 && m_operators.size() == (m_comparissons.size() - 1))
    {
        rawStr = m_comparissons.at(0).raw();
        for(qint32 i = 0; i < m_operators.size(); i++)
        {
            switch(m_operators.at(i))
            {
                case AND:
                    rawStr = AND_TEMPLATE.arg(rawStr).arg(m_comparissons.at(i + 1).raw());
                    break;
                case AND_NOT:
                    rawStr = AND_NOT_TEMPLATE.arg(rawStr).arg(m_comparissons.at(i + 1).raw());
                    break;
                case OR:
                    rawStr = OR_TEMPLATE.arg(rawStr).arg(m_comparissons.at(i + 1).raw());
                    break;
                case OR_NOT:
                    rawStr = OR_NOT_TEMPLATE.arg(rawStr).arg(m_comparissons.at(i + 1).raw());
                    break;
            }
        }
    }

    return rawStr;
}

