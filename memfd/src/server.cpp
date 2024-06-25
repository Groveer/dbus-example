#include "server.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QLoggingCategory>

#include <sys/mman.h>
#include <unistd.h>

Q_LOGGING_CATEGORY(server, "[Server]")

Server::Server(QObject *parent)
    : QObject(parent)
    , m_fd(0)
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

QDBusUnixFileDescriptor Server::Memfd()
{
    if (m_fd == 0) {
        m_fd = memfd_create("SharedMem", MFD_CLOEXEC);
    }
    const char *message = "Hello from memfd!\n";
    write(m_fd, message, strlen(message));
    return QDBusUnixFileDescriptor(m_fd);
}
