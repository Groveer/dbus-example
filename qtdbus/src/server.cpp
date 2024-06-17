#include "server.h"

#include <qglobal.h>

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QLoggingCategory>
#include <unistd.h>

Q_LOGGING_CATEGORY(server, "[Server]")

Server::Server(QObject *parent)
    : QObject(parent)
    , m_count(0)
{
    initDBus();
}

void Server::initDBus()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerObject(DBUS_PATH,
                                   this,
                                   QDBusConnection::ExportAllSlots
                                       | QDBusConnection::ExportAllProperties
                                       | QDBusConnection::ExportAllSignals)) {
        qCWarning(server) << "failed to register dbus object: " << connection.lastError().message();
    }
    if (!connection.registerService(DBUS_NAME)) {
        qCWarning(server) << "failed to register dbus service: "
                          << connection.lastError().message();
    }
}

uint Server::IncrementBy(uint value)
{
    // test for timeout
    /*sleep(5);*/

    m_count += value;
    Q_EMIT CountChanged(m_count);
    return m_count;
}
