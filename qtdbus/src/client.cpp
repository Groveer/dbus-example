#include "client.h"

#include "common.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusReply>
#include <QDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(client, "[Client]")

Client::Client(QObject *parent)
    : QObject(parent)
{
    initServerInfo();
}

void Client::initServerInfo()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    m_serverInterface = new QDBusInterface(DBUS_NAME, DBUS_PATH, DBUS_INTERFACE, connection);
    connection.connect(DBUS_NAME,
                       DBUS_PATH,
                       DBUS_INTERFACE,
                       "CountChanged",
                       this,
                       SLOT(onCountChanged(uint)));
    qCInfo(client) << "default timeout: " << m_serverInterface->timeout();
    m_serverInterface->setTimeout(2 * 1000);
}

void Client::onCountChanged(uint value)
{
    qCInfo(client) << "CountChanged: " << value;
}

void Client::method_call()
{
    QDBusReply<uint> reply = m_serverInterface->call("IncrementBy", uint(1));
    if (reply.isValid()) {
        qCInfo(client) << "IncrementBy(1) = " << reply.value();
    } else {
        qCWarning(client) << reply.error().message();
    }
}

void Client::onIncrementByFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<uint> reply = watcher->reply();
    if (reply.isValid()) {
        qCInfo(client) << "IncrementBy(1) = " << reply.value();
    } else {
        qCWarning(client) << reply.error().message();
    }
    watcher->deleteLater();
}

void Client::method_call_async()
{
    QDBusPendingCall call = m_serverInterface->asyncCall("IncrementBy", uint(1));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &Client::onIncrementByFinished);
    qCInfo(client) << "method call async done";
}
