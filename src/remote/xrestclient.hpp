#ifndef XRESTCLIENT_HPP
#define XRESTCLIENT_HPP

#include <QObject>


class XRESTClient : public QObject
{
    Q_OBJECT

public:
    explicit XRESTClient(QObject *parent = nullptr);
};

#endif // XRESTCLIENT_HPP
