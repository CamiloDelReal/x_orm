#ifndef XSTRINGTEMPLATES_HPP
#define XSTRINGTEMPLATES_HPP

#include <QObject>

static const QString DEFAULT_CONNECTION_NAME = QStringLiteral("X_DATABASE");

static const QString QSQLITE_DRIVER = QStringLiteral("QSQLITE");
static const QString QSQLITE_CIPHER_DRIVER = QStringLiteral("QSQLITE");

static const QString EMPTY_TEMPLATE = QStringLiteral("");
static const QString SPACE_TEMPLATE = QStringLiteral(" ");
static const QString COMMA_LINEBREAK_TEMPLATE = QStringLiteral(",\n");
static const QString COMMA_TEMPLATE = QStringLiteral(", ");
static const QString QUOTE_TEMPLATE = QStringLiteral("'");
static const QString ASTERISK_TEMPLATE = QStringLiteral("*");

static const QString INTEGER_TEMPLATE = QStringLiteral("INTEGER");
static const QString DOUBLE_TEMPLATE = QStringLiteral("DOUBLE");
static const QString BOOLEAN_TEMPLATE = QStringLiteral("BOOLEAN");
static const QString TEXT_TEMPLATE = QStringLiteral("TEXT");
static const QString BLOB_TEMPLATE = QStringLiteral("BLOB");
static const QString DATE_TEMPLATE = QStringLiteral("DATE");
static const QString TIME_TEMPLATE = QStringLiteral("TIME");
static const QString DATETIME_TEMPLATE = QStringLiteral("DATETIME");

static const QString NO_ACTION_TEMPLATE = QStringLiteral("NO ACTION");
static const QString RESTRIC_TEMPLATE = QStringLiteral("RESTRIC");
static const QString CASCADE_TEMPLATE = QStringLiteral("CASCADE");
static const QString SET_NULL_TEMPLATE = QStringLiteral("SET NULL");
static const QString SET_DEFAULT_TEMPLATE = QStringLiteral("SET DEFAULT");

static const QString CREATE_TABLE_TEMPLATE = QStringLiteral("CREATE TABLE IF NOT EXISTS %1 (\n%2\n);");
static const QString FIELD_TEMPLATE = QStringLiteral("%1 %2 %3");

static const QString PRIMARY_KEY_TEMPLATE = QStringLiteral("PRIMARY KEY");
static const QString PRIMARY_KEY_AUTOINCREMENT_TEMPLATE = QStringLiteral("PRIMARY KEY AUTOINCREMENT");
static const QString MULTI_PRIMARY_KEY_TEMPLATE = QStringLiteral("PRIMARY KEY (%1)");

static const QString NOT_NULL_TEMPLATE = QStringLiteral("NOT NULL");

static const QString DEFAULT_TEMPLATE = QStringLiteral("DEFAULT (%1)");

static const QString FOREIGN_NAME_TEMPLATE = QStringLiteral("%1_%2_as_%3_fk");
static const QString FOREIGN_KEY_TEMPLATE = QStringLiteral("CONSTRAINT %1 FOREIGN KEY (%2) REFERENCES %3 (%4) ON UPDATE %5 ON DELETE %6");
static const QString ENABLE_FOREIGN_KEYS_TEMPLATES = QStringLiteral("PRAGMA foreign_keys = ON;");

static const QString UNIQUE_NAME_TEMPLATE = QStringLiteral("%1_%2_unique");
static const QString UNIQUE_TEMPLATE = QStringLiteral("CONSTRAINT %1 UNIQUE (%2)");
static const QString UNIQUE_ASC_TEMPLATE = QStringLiteral("CONSTRAINT %1 UNIQUE (%2 ASC)");
static const QString UNIQUE_DESC_TEMPLATE = QStringLiteral("CONSTRAINT %1 UNIQUE (%2 DESC)");

static const QString CHECK_NAME_TEMPLATE = QStringLiteral("%1_%2_check");
static const QString CHECK_TEMPLATE = QStringLiteral("CONSTRAINT %1 CHECK (%2)");

static const QString INDEX_NAME_TEMPLATE = QStringLiteral("%1_%2_index");
static const QString CREATE_INDEX_ASC_TEMPLATE = QStringLiteral("CREATE INDEX IF NOT EXISTS %1 ON %2 (%3 ASC);");
static const QString CREATE_INDEX_DESC_TEMPLATE = QStringLiteral("CREATE INDEX IF NOT EXISTS %1 ON %2 (%3 DESC);");
static const QString CREATE_INDEX_UNIQUE_ASC_TEMPLATE = QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS %1 ON %2 (%3 ASC);");
static const QString CREATE_INDEX_UNIQUE_DESC_TEMPLATE = QStringLiteral("CREATE UNIQUE INDEX IF NOT EXISTS %1 ON %2 (%3 DESC);");

static const QString SELECT_FROM_WITH_LIMIT_TEMPLATE = QStringLiteral("SELECT * FROM %1 LIMIT %2,%3;");
static const QString INSERT_ROW_TEMPLATE = QStringLiteral("INSERT INTO %1 (%2) VALUES (%3);");
static const QString UPDATE_ROW_TEMPLATE = QStringLiteral("UPDATE %1 SET %2 WHERE %3;");
static const QString DELETE_ROW_TEMPLATE = QStringLiteral("DELETE FROM %1 WHERE %2;");
static const QString SET_FIELD_TEMPLATE = QStringLiteral("%1 = '%2'");
static const QString EQUAL_TEMPLATE = QStringLiteral("%1 = '%2'");
static const QString EQUAL_FIELD_TEMPLATE = QStringLiteral("%1 = %2");
static const QString GREAT_THAN_TEMPLATE = QStringLiteral("%1 > '%2'");
static const QString GREAT_AND_EQUAL_THAN_TEMPLATE = QStringLiteral("%1 >= '%2'");
static const QString LESS_THAN_TEMPLATE = QStringLiteral("%1 < '%2'");
static const QString LESS_AND_EQUAL_THAN_TEMPLATE = QStringLiteral("%1 <= '%2'");
static const QString IN_TEMPLATE = QStringLiteral("%1 IN (%2)");
static const QString BETWEEN_TEMPLATE = QStringLiteral("%1 BETWEEN '%2' AND '%3'");
static const QString LIKE_TEMPLATE = QStringLiteral("%1 LIKE '%%2%'");
static const QString AND_SEPARATOR_TEMPLATE = QStringLiteral(" AND ");
static const QString AND_TEMPLATE = QStringLiteral("(%1 AND %2)");
static const QString AND_NOT_TEMPLATE = QStringLiteral("(%1 AND NOT %2)");
static const QString OR_SEPARATOR_TEMPLATE = QStringLiteral(" OR ");
static const QString OR_TEMPLATE = QStringLiteral("(%1 OR %2)");
static const QString OR_NOT_TEMPLATE = QStringLiteral("(%1 OR NOT %2)");
static const QString FIND_TEMPLATE = QStringLiteral("SELECT * FROM %1 WHERE %2;");
static const QString FIND_WITH_FIELD_SELECTION_TEMPLATE = QStringLiteral("SELECT %1 FROM %2 WHERE %3;");
static const QString FIND_FIRST_TEMPLATE = QStringLiteral("SELECT * FROM %1 WHERE %2 LIMIT 1;");
static const QString SELECT_TEMPLATE = QStringLiteral("SELECT %1 ");
static const QString FROM_TEMPLATE = QStringLiteral("FROM %1 ");
static const QString WHERE_TEMPLATE = QStringLiteral("WHERE %1 ");
static const QString GROUP_BY_TEMPLATE = QStringLiteral("GROUP BY %1 ");
static const QString LIMIT_TEMPLATE = QStringLiteral("LIMIT %1,%2 ");

#endif // XSTRINGTEMPLATES_HPP

