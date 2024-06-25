#include "client.h"

#include "common.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingReply>
#include <QDBusUnixFileDescriptor>
#include <QDebug>
#include <QLoggingCategory>

#include <sys/mman.h>
#include <unistd.h>

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
}

void Client::onMemfdFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QDBusUnixFileDescriptor> reply = watcher->reply();
    if (reply.isValid()) {
        auto fd = reply.value().fileDescriptor();
        void *addr = mmap(nullptr, 4096, PROT_READ, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            qWarning() << "Failed to mmap memfd";
            return;
        }

        qCInfo(client) << "mmap done: \n" << static_cast<char *>(addr);
        munmap(addr, 4096);
        close(fd);
    } else {
        qCWarning(client) << reply.error().message();
    }
    watcher->deleteLater();
}

void Client::method_call_async()
{
    QDBusPendingCall call = m_serverInterface->asyncCall("Memfd");
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &Client::onMemfdFinished);
    qCInfo(client) << "method call async done";
}
