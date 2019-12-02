#ifndef XKEY_HPP
#define XKEY_HPP

#include <QObject>


class XKey
{
public:
    explicit XKey();
    XKey(const QString &member);
    XKey(const QString &member, const QString &name);
    XKey(const XKey &other);

    XKey& operator=(const XKey &other);

    QString member() const;
    QString name() const;

    void setMember(const QString &member);
    void setName(const QString &name);

private:
    QString m_member;
    QString m_name;
};

#define X_KEY_1(MODEL_PROPERTY) \
    XKey(#MODEL_PROPERTY)

#define X_KEY_2(MODEL_PROPERTY, KEY_NAME) \
    XKey(#MODEL_PROPERTY, #KEY_NAME)

#define X_KEY_BUILDER(_1, _2, MACRO_NAME, ...) MACRO_NAME

#define X_KEY(...) X_KEY_BUILDER(__VA_ARGS__, X_KEY_2, X_KEY_1)(__VA_ARGS__)

#endif // XKEY_HPP
