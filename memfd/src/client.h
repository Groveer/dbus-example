#pragma once

#include <QObject>

class QDBusInterface;
class QDBusPendingCallWatcher;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    void method_call_async();

private:
    void initServerInfo();
    void onMemfdFinished(QDBusPendingCallWatcher *);

    QDBusInterface *m_serverInterface;
};
