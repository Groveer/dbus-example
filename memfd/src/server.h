#pragma once

#include "common.h"

#include <QDBusUnixFileDescriptor>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_INTERFACE);

public:
    explicit Server(QObject *parent = nullptr);

public Q_SLOTS:
    QDBusUnixFileDescriptor Memfd();

private:
    void initDBus();

    int m_fd;
};
