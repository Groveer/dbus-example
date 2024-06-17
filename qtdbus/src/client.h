#pragma once

#include <QObject>

class QDBusInterface;
class QDBusPendingCallWatcher;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    void method_call();
    void method_call_async();

public Q_SLOTS:
    void onCountChanged(uint value);

private:
    void initServerInfo();
    void onIncrementByFinished(QDBusPendingCallWatcher *);

    QDBusInterface *m_serverInterface;
};
